#include "lintplugplugin.h"
#include "lintplugconstants.h"
#include "lintitem.h"
#include "lintoutputpane.h"
#include "configpage.h"

#include <coreplugin/icore.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/editormanager.h>

#include <QFileInfo>

#include <QtPlugin>

using namespace LintPlug::Internal;

LintPlugPlugin::LintPlugPlugin() : m_lintOutputPane(NULL)
{
    qRegisterMetaType<LintItem>("LintItem");
}

LintPlugPlugin::~LintPlugPlugin()
{
}

bool LintPlugPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    m_lintOutputPane = new LintOutputPane(NULL);
    addAutoReleasedObject(m_lintOutputPane);
    connect(m_lintOutputPane, SIGNAL(itemClicked(LintItem)), SLOT(itemClicked(LintItem)));
    m_configPage=new ConfigPage();
    addAutoReleasedObject(m_configPage);
    return true;
}

void LintPlugPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

void LintPlugPlugin::itemClicked(const LintItem &item)
{
    if (QFileInfo::exists(item.file))
    {
        Core::IEditor *editor = Core::EditorManager::openEditor(item.file);
        editor->gotoLine(item.line);
    }
}

