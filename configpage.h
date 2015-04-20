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

#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>
#include <QPointer>

namespace LintPlug {
namespace Internal {

class Config;

class ConfigPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    ConfigPage(QObject *parent = 0);
    ~ConfigPage();
    QWidget *widget();
    void apply();
    void finish();

private:
    QPointer<Config> m_widget;
};

} // namespace LintPlug
} // namespace Internal

#endif // CONFIGPAGE_H
