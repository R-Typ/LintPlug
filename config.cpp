/**
 *	@file 		config.cpp
 *	@author		Roman Nikiforov
 *  @brief
 *
 *	Change History
 * -------------------------------------------------------------------
 *	@date		29.01.2015		header created
 *
 *
 */

#include "config.h"
#include "ui_config.h"
#include "lintplugconstants.h"
#include <QFileDialog>
#include <coreplugin/icore.h>

using namespace LintPlug::Internal;

Config::Config(QWidget *parent) : QWidget(parent)
, ui(new Ui::Config)
, m_settings(Core::ICore::settings())
{
    ui->setupUi(this);
    connect(ui->btnOpen, SIGNAL(clicked()), SLOT(openFileDlg()));

    ui->edLint->setText(m_settings->value(QLatin1String(Constants::SETTINGS_LINT_EXE), QLatin1String("")).toString());
    ui->edArgs->setText(m_settings->value(QLatin1String(Constants::SETTINGS_LINT_ARGS), QLatin1String("")).toString());
    ui->chkDefines->setChecked(m_settings->value(QLatin1String(Constants::SETTINGS_LINT_DEFS), false).toBool());
}

Config::~Config()
{
    delete ui;
}

QSettings *Config::settings()
{
    apply();
    return m_settings;
}

void Config::apply()
{
    if (m_settings)
    {
        m_settings->setValue(QLatin1String(Constants::SETTINGS_LINT_EXE), ui->edLint->text());
        m_settings->setValue(QLatin1String(Constants::SETTINGS_LINT_ARGS), ui->edArgs->text());
        m_settings->setValue(QLatin1String(Constants::SETTINGS_LINT_DEFS), ui->chkDefines->isChecked());
    }
}

void Config::openFileDlg()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select PC Lint executable"), QLatin1String("."), tr("Exe Files (*.exe)"));
    if (!fileName.isNull())
    {
        ui->edLint->setText(fileName);
    }
}
