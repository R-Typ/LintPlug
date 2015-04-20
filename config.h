/**
 *	@file 		config.h
 *	@author		Roman Nikiforov
 *  @brief
 *
 *	Change History
 * -------------------------------------------------------------------
 *	@date		29.01.2015		header created
 *
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class Config;
}

namespace LintPlug {
namespace Internal {

class Config : public QWidget
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();
    void setSettings(QSettings* settings);
    QSettings* settings();
    void apply();

private slots:
    void openFileDlg();

private:
    Ui::Config *ui;
    QSettings*  m_settings;
};

} // namespace LintPlug
} // namespace Internal

#endif // CONFIG_H
