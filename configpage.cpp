/**
 *	@file
 *	@author		Roman Nikiforov
 *  @brief
 *
 *	Change History
 * -------------------------------------------------------------------
 *	@date		29.01.2015		header created
 *
 *
 */

#include "configpage.h"
#include "config.h"

using namespace LintPlug::Internal;

ConfigPage::ConfigPage(QObject *parent): IOptionsPage(parent)
, m_widget(NULL)
{
    setId("LintPlugSettings");
    setDisplayName(tr("PC Lint"));
    setCategory("T.Analyzer");
}

ConfigPage::~ConfigPage()
{

}

QWidget *ConfigPage::widget()
{
    if (!m_widget)
    {
        m_widget = new Config();
    }
    return m_widget;
}

void ConfigPage::apply()
{
    if (m_widget) m_widget->apply();
}

void ConfigPage::finish()
{
    delete m_widget;
}

