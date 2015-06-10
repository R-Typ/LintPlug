/**
 *	@file
 *	@author		Roman Nikiforov
 *  @brief
 *
 *	Change History
 * -------------------------------------------------------------------
 *	@date		27.01.2015		header created
 *
 *
 */

#include "linttreeview.h"
#include "lintplugconstants.h"
#include <coreplugin/icore.h>

#include <QHeaderView>
#include <QSettings>
#include <QString>

using namespace LintPlug::Internal;

LintTreeView::LintTreeView(QWidget *parent) : Utils::TreeView(parent)
{
    setRootIsDecorated(false);
    setFrameStyle(QFrame::NoFrame);
    setSortingEnabled(true);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setSelectionBehavior(QTreeView::SelectRows);

    header()->setSectionResizeMode(QHeaderView::Interactive);
    header()->setStretchLastSection(true);
    header()->setSectionsMovable(false);    
    loadDisplaySettings();
    connect(header(), SIGNAL(sectionResized(int,int,int)), SLOT(columnResized(int,int,int)));
}

LintTreeView::~LintTreeView()
{
    saveDisplaySettings();
}

void LintTreeView::resizeEvent(QResizeEvent *event)
{
    if (m_colWidth.size() == 0) loadDisplaySettings();
    qreal scale;
    if ((event->oldSize().width() == 0) || (event->oldSize().width() == -1)) scale=1;
    else scale = static_cast<qreal>(event->size().width()) / static_cast<qreal>(event->oldSize().width());
    for(int i=0; i<m_colWidth.size(); i++)
    {
        if (m_colWidth[i] == 0) m_colWidth[i]=0.45*event->size().width();
        else m_colWidth[i]*=scale;
        if (m_colWidth[i]>event->size().width()) m_colWidth[i]=0.45*event->size().width();//do not allow one colum take whole place
        setColumnWidth(i, m_colWidth[i]);
    }
}

void LintTreeView::columnResized(int column, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    if (column < m_colWidth.size())
        m_colWidth[column] = newSize;
}

void LintTreeView::saveDisplaySettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(Constants::SETTINGS_GROUP));
    settings->setValue(QLatin1String(Constants::SETTINGS_LEVEL_COLUMN_WIDTH),
                       columnWidth(Constants::OUTPUT_COLUMN_LEVEL));
    settings->setValue(QLatin1String(Constants::SETTINGS_CODE_COLUMN_WIDTH),
                       columnWidth(Constants::OUTPUT_COLUMN_CODE));
    settings->setValue(QLatin1String(Constants::SETTINGS_TEXT_COLUMN_WIDTH),
                       columnWidth(Constants::OUTPUT_COLUMN_TEXT));
    settings->setValue(QLatin1String(Constants::SETTINGS_FILE_COLUMN_WIDTH),
                       columnWidth(Constants::OUTPUT_COLUMN_FILE));
    settings->endGroup();

}

void LintTreeView::loadDisplaySettings()
{
    m_colWidth.clear();
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String(Constants::SETTINGS_GROUP));
    m_colWidth.append(settings->value(QLatin1String(Constants::SETTINGS_LEVEL_COLUMN_WIDTH), 50).toInt());
    m_colWidth.append(settings->value(QLatin1String(Constants::SETTINGS_CODE_COLUMN_WIDTH), 50).toInt());
    m_colWidth.append(settings->value(QLatin1String(Constants::SETTINGS_TEXT_COLUMN_WIDTH), 200).toInt());
    m_colWidth.append(settings->value(QLatin1String(Constants::SETTINGS_FILE_COLUMN_WIDTH), 100).toInt());
    settings->endGroup();
}
