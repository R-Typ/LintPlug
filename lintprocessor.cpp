/**
 *	@file
 *	@author		Roman Nikiforov
 *  @brief
 *
 *	Change History
 * -------------------------------------------------------------------
 *	@date		28.01.2015		header created
 *
 *
 */

#include "lintprocessor.h"

#include <coreplugin/coreconstants.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>
#include <projectexplorer/buildconfiguration.h>
#include <cpptools/cppmodelmanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/editormanager/editormanager.h>
#include <qmakeprojectmanager/qmakeproject.h>
#include <qmakeprojectmanager/qmakenodes.h>

#include <proparser/proitems.h>

#include <QTemporaryFile>
#include <QProcess>
#include <QThread>
#include <QSettings>

using namespace LintPlug::Internal;

using namespace ProjectExplorer;

LintProcessor::LintProcessor()
: m_thread(new QThread())
, m_isRunning(false)
, m_mode(SINGLE_FILE)
, m_lastError()
{
    m_thread->setObjectName(QLatin1String("LintProcessorThread"));
    this->moveToThread(m_thread);
    connect(m_thread, SIGNAL(started()), this, SLOT(process()));
    connect(m_thread, SIGNAL(finished()), this, SIGNAL(finished()));
}

LintProcessor::~LintProcessor()
{
    stop();
    delete m_thread;
    m_thread=NULL;
}

void LintProcessor::start()
{
    if (m_isRunning) stop();    
    m_thread->start();
}

void LintProcessor::stop()
{
    m_isRunning=false;
    m_thread->wait();
}

void LintProcessor::process()
{
    m_lastError.clear();
    QStringList includeDirs, defines, sources;
    QString activeSource;
    if (projectData(includeDirs, defines, sources, activeSource))
    {
        if (m_mode == SINGLE_FILE && activeSource.isEmpty())
        {
            m_lastError=tr("Nothing to check. Please open source file in editor.");
            if (m_thread) m_thread->quit();
            m_isRunning=false;
            return;
        }
        QSettings* settings = Core::ICore::settings();
        const QString lintExe=settings->value(QLatin1String(Constants::SETTINGS_LINT_EXE), QLatin1String("")).toString();
        QTemporaryFile file(QLatin1String("XXXXXX.lnt"));
        if (file.open())
        {
            QTextStream out(&file);
            foreach (QString inc, includeDirs)
            {
                if (!inc.trimmed().isEmpty())
                    out<<"-i\""<<QDir::toNativeSeparators(inc)<<"\"\r\n";
            }
            foreach (QString def, defines)
            {
                out<<"-D"<<def<<"\r\n";
            }
            if (m_mode == SINGLE_FILE)
            {
                out<<activeSource<<"\r\n";
            } else if (m_mode == PROJECT)
            {
                foreach (QString src, sources)
                {
                    out<<QDir::toNativeSeparators(src)<<"\r\n";
                }
            }
        }
        file.flush();
        //qDebug()<<file.fileName();
        QProcess lint;
        QStringList lintParams;
        // define output format
        lintParams<<QLatin1String("-format=%(%f(%l)\\s:\\s%)%t\\s%n:\\s%m")
                  <<QLatin1String("+ffn")
                  <<QLatin1String("-width(0)")
                  <<QLatin1String("-hf1")
                  <<QLatin1String("+e900");// succesfull execution message from lint
        // add lint directory to search path
        QFileInfo fi(lintExe);
        QString lintPath=fi.absolutePath();
        lintParams<<QString(QLatin1String("-i\"%1\"")).arg(lintPath);
        QString userParams=settings->value(QLatin1String(Constants::SETTINGS_LINT_ARGS), QLatin1String("")).toString();
        lintParams.append(userParams.split(QLatin1String(" ")));
        lintParams<<file.fileName();
        //qDebug()<<"start Lint with parameters:"<<lintParams;
        lint.start(lintExe, lintParams);
        if (lint.waitForStarted() && lint.waitForFinished(-1))
        {
            QByteArray result = lint.readAll();
#if 0
            QFile file(QLatin1String("c:/out.txt"));
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                file.write(result);
            }
#endif
            if (!parse(result))
            {
                int idx=result.lastIndexOf("error");
                m_lastError=tr("Some error occured during Lint check.");
                if (idx !=-1) m_lastError.append(tr(" Last error message:\n")).append(QLatin1String(result.mid(idx)));
            }
        }
        //qDebug()<<lint.exitCode();
    }
    if (m_thread) m_thread->quit();
    m_isRunning=false;
}

bool LintProcessor::projectData(QStringList &includeDirs, QStringList &defines, QStringList &sources, QString& activeSource)
{

    Project* project=SessionManager::startupProject();
    if (!project) return false;
    // get main .pro file of active project
    Target* target=project->activeTarget();
    if (!target) return false;
    ProjectExplorer::BuildConfiguration* config=target->activeBuildConfiguration();
    Utils::FileName buildDir=config->buildDirectory();
    includeDirs<<buildDir.toString();    
    if (Core::EditorManager::currentDocument()) activeSource=Core::EditorManager::currentDocument()->filePath().toString();

    // I didn't find reliable way to get active project sources if different configurations are present in .pro file.
    // There is also no way to get directory where MOC files are generated.
    // Therefore we suppose that all includes are always active and moc files are in build directory under configuration.
/*
    Project* project=SessionManager::startupProject();
    QmakeProjectManager::QmakeProject* qProj=dynamic_cast<QmakeProjectManager::QmakeProject*>(project);
    if (qProj)
    {
        QmakeProjectManager::Internal::EvalResult* res=qProj->projectEvalResult();
        defines=res->newVarValues[QmakeProjectManager::DefinesVar];
        QStringList inc=res->newVarValues[QmakeProjectManager::IncludePathVar];
        QStringList uis=res->newVarValues[QmakeProjectManager::UiDirVar];
        QStringList mocs=res->newVarValues[QmakeProjectManager::MocDirVar];
        includeDirs=inc+uis+mocs;
        sources=res->newVarValues[QmakeProjectManager::CppSourceVar];
    }
*/
    QString mocsDir=buildDir.toString();
    if (config->buildType() == BuildConfiguration::Debug) mocsDir.append(QLatin1String("/debug"));
    else if (config->buildType() == BuildConfiguration::Release) mocsDir.append(QLatin1String("/release"));
    includeDirs<<mocsDir;

    CppTools::CppModelManager* modelManager = CppTools::CppModelManager::instance();
    CppTools::ProjectInfo info=modelManager->projectInfo(project);
    CppTools::ProjectPart::HeaderPaths includePath=info.headerPaths();
    QByteArray defs=info.defines();

    const QSet<QString> sourceFiles=info.sourceFiles();
    foreach(CppTools::ProjectPart::HeaderPath inc, includePath)
    {
        includeDirs<<inc.path;
    }
    // in some cases passing project defines to Lint leads to a lot of strange messages  in VC headers
    QSettings* settings = Core::ICore::settings();
    bool useDefs=settings->value(QLatin1String(Constants::SETTINGS_LINT_DEFS), false).toBool();
    if (useDefs)
    {
        QList<QByteArray> defList=defs.split('\n');
        foreach(QByteArray def, defList)
        {
            QList<QByteArray> tmp=def.split(' ');
            if (tmp.size()>1)
            {
                QByteArray d=tmp[1];
                if (tmp.size()>2)
                {
                    // with value
                    if (tmp.size()>3)
                    {
                        QByteArray val=def.mid(def.indexOf(' ', 8)+1);//8 is the length of '#define '
                        if (val.indexOf("\"") == -1)
                            d.append("=\"").append(val).append("\"");
                    } else {
                        d.append("=").append(tmp[2]);
                    }
                }
                defines<<QLatin1String(d);
            }
        }
    }
    foreach(QString src, sourceFiles)
    {
        // check if we have header dir in includes path
        // it seems that we have all sources, but include dirs only for selected configuration
        QFileInfo fi(src);
        QString path=fi.absolutePath();
        if (!includeDirs.contains(path)) includeDirs<<path;
        if (fi.suffix().toLower() == QLatin1String("cpp") || fi.suffix().toLower() == QLatin1String("cc")) sources<<src;
    }
    return true;
}

//QStringList sources1=project->files(ProjectExplorer::Project::ExcludeGeneratedFiles);
//switch (CppTools::ProjectFile::classify(src))
//{
//    case CppTools::ProjectFile::CSource:
//    case CppTools::ProjectFile::CXXSource: sources<<src; break;
//    case CppTools::ProjectFile::CHeader:
//    {
//        break;
//    }
//    default: break;
//}

bool LintProcessor::parse(const QByteArray &data)
{
    bool success(false);
    QString lintOutput=QLatin1String(data);
    QStringList lines=lintOutput.split(QLatin1String("\n"));
    QString type, level, code;
    LintItem itm;
    m_items.clear();
#if defined (Q_OS_LINUX)
    QRegExp lineRex(QLatin1String("^((?:/[/a-zA-Z0-9_\\.\\-\\s]+)+\\.(?:h|cpp|cc))\\((\\d+)\\)*\\s\\:*\\s(\\w+)*\\s(\\d+)\\:*\\s((?:[\\S*\\s*])+)"));
#else
    QRegExp lineRex(QLatin1String("^((?:[a-zA-Z]:)?(?:\\\\[\\(\\)a-zA-Z0-9_\\.\\-\\s]+)+\\.(?:h|cpp|cc))\\((\\d+)\\)\\:\\s(\\w+)\\s(\\d+)\\:\\s\\((\\b\\w+\\b)\\s--\\s(.*)"));
#endif
    const QString comment(QLatin1String("---"));
    foreach(QString line, lines)
    {
        if (line.trimmed().isEmpty() || line.startsWith(comment)) continue;
#if defined (Q_OS_LINUX)
        if (line.indexOf(QLatin1String("Note 900:")) != -1) success=true;
#else
        if (line.indexOf(QLatin1String("error 900:")) != -1) success=true;
#endif
        if (lineRex.indexIn(line) != -1)
        {
            itm.file = lineRex.cap(1);
            itm.line = lineRex.cap(2).toInt();
#if defined (Q_OS_LINUX)
            level = lineRex.cap(3);
            code = lineRex.cap(4);//TODO: may be use it for description tooltip
            itm.text = lineRex.cap(5).trimmed();
#else
            type = lineRex.cap(3);
            code = lineRex.cap(4);//TODO: may be use it for description tooltip
            level = lineRex.cap(5);
            itm.text = lineRex.cap(6).trimmed();
            itm.text.chop(1);// remove last ')'
#endif
            if (level == QLatin1String("Warning"))
            {
                itm.iconResource=QLatin1String(Core::Constants::ICON_WARNING);
                itm.color=QColor(QLatin1String(Constants::COLOR_WARNING_BG));
                itm.level=LintItem::WARNING;
            } else if (level == QLatin1String("Info")) {
                itm.iconResource=QLatin1String(Core::Constants::ICON_INFO);
                itm.color=QColor(QLatin1String(Constants::COLOR_INFO_BG));
                itm.level=LintItem::INFO;
            } else if (level == QLatin1String("Note")) {
                itm.iconResource=QLatin1String(Core::Constants::ICON_INFO);
                itm.color=QColor(QLatin1String(Constants::COLOR_NOTE_BG));
                itm.level=LintItem::NOTE;
            } else {
                itm.iconResource=QLatin1String(Core::Constants::ICON_ERROR);
                itm.color=QColor(QLatin1String(Constants::COLOR_ERROR_BG));
                itm.level=LintItem::ERROR;
            }
            if (!m_items.contains(itm))
            {
                m_items.append(itm);
            } else {
                //qDebug()<<QLatin1String("duplicated message:")<<itm.text;
            }

        } else {
            //qDebug()<<QLatin1String("unmatched message:")<<line;
        }
    }
    return success;
}

QList<LintItem> * const LintProcessor::items()
{
    return &m_items;
}
