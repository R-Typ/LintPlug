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

#include "lintoutputpane.h"
#include "lintplugconstants.h"
#include "lintitemsmodel.h"
#include "linttreeview.h"
#include "lintprocessor.h"

#include <coreplugin/icore.h>

#include <QComboBox>
#include <QToolButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QVBoxLayout>

#include <projectexplorer/projectexplorerconstants.h>
using namespace ProjectExplorer;

using namespace LintPlug::Internal;

LintOutputPane::LintOutputPane(QObject *parent) : IOutputPane(parent)
, m_mainWidget(new QWidget())
, m_treeView(new LintTreeView())
, m_layout(new QVBoxLayout())
, m_outputConsole(new QTextEdit())
, m_cbTool(new QComboBox())
, m_btnRun(new QToolButton())
, m_btnStop(new QToolButton())
, m_itemsModel(new LintItemsModel(this))
, m_processor(new LintProcessor())
{    
    m_outputConsole->setReadOnly(true);
    m_outputConsole->hide();
    m_treeView->setModel(m_itemsModel);
    connect(m_treeView, SIGNAL(activated(QModelIndex)), SLOT(treeViewClicked(QModelIndex)));
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->addWidget(m_outputConsole);
    m_layout->addWidget(m_treeView);
    m_mainWidget->setLayout(m_layout);

    m_cbTool->addItem(tr("Lint File"));
    m_cbTool->addItem(tr("Lint Project"));

    m_btnRun->setIcon(QIcon(QLatin1String(ProjectExplorer::Constants::ICON_RUN_SMALL)));
    m_btnRun->setToolTip(tr("Execute PC Lint"));

    m_btnStop->setIcon(QIcon(QLatin1String(ProjectExplorer::Constants::ICON_STOP_SMALL)));
    m_btnStop->setToolTip(tr("Abort PC Lint check"));
    m_btnStop->setEnabled(false);

    //TODO: button to switch between output and treeview

    connect(m_btnRun, SIGNAL(clicked()), SLOT(runLint()));
    connect(m_btnStop, SIGNAL(clicked()), SLOT(stopLint()));

    connect(m_cbTool, SIGNAL(currentIndexChanged(int)), SLOT(setMode(int)));

    connect(m_itemsModel, SIGNAL(layoutChanged()), SIGNAL(navigateStateUpdate()));
    connect(m_itemsModel, SIGNAL(layoutChanged()), SLOT(updateCount()));

    connect(m_processor, SIGNAL(finished()), SLOT(showResults()));
    connect(m_processor, SIGNAL(outputData(QString)), m_outputConsole, SLOT(append(QString)));
}

LintOutputPane::~LintOutputPane()
{
    if (m_processor->isRunning()) m_processor->stop();
    delete m_processor;
    delete m_itemsModel;
    delete m_treeView;    
    delete m_outputConsole;
    delete m_layout;
    delete m_mainWidget;
    delete m_cbTool;
    delete m_btnRun;
}

QWidget *LintOutputPane::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent)
    return m_mainWidget;
}

QList<QWidget *> LintOutputPane::toolBarWidgets() const
{
    return QList<QWidget*>()<< m_cbTool<<m_btnRun<<m_btnStop;
}

QString LintOutputPane::displayName() const
{
    return tr(Constants::OUTPUT_PANE_TITLE);
}

int LintOutputPane::priorityInStatusBar() const
{
    return 1;
}

void LintOutputPane::clearContents()
{
    if (m_processor->isRunning())
    {
        m_outputConsole->clear();
    } else {
        m_itemsModel->clear();
    }
}

void LintOutputPane::visibilityChanged(bool visible)
{
    Q_UNUSED(visible)
}

void LintOutputPane::setFocus()
{
    m_treeView->setFocus();
}

bool LintOutputPane::hasFocus() const
{
    return m_treeView->window()->focusWidget() == m_treeView;
}

bool LintOutputPane::canFocus() const
{
    return (!m_processor->isRunning());
}

bool LintOutputPane::canNavigate() const
{
    return (!m_processor->isRunning());
}

bool LintOutputPane::canNext() const
{
    return (!m_processor->isRunning() && m_treeView->model()->rowCount() > 1);
}

bool LintOutputPane::canPrevious() const
{
    return (!m_processor->isRunning() && m_treeView->model()->rowCount() > 1);
}

void LintOutputPane::goToNext()
{
    const QModelIndex nextIndex = nextModelIndex();
    m_treeView->selectionModel()->setCurrentIndex(nextIndex, QItemSelectionModel::SelectCurrent
                                                      | QItemSelectionModel::Rows);
    treeViewClicked(nextIndex);
}

void LintOutputPane::goToPrev()
{
    const QModelIndex prevIndex = previousModelIndex();
    m_treeView->selectionModel()->setCurrentIndex(prevIndex, QItemSelectionModel::SelectCurrent
                                                      | QItemSelectionModel::Rows);
    treeViewClicked(prevIndex);
}

void LintOutputPane::treeViewClicked(const QModelIndex &index)
{
    int row = index.row();

    LintItem item;
    item.text = index.sibling(row, Constants::OUTPUT_COLUMN_TEXT).data().toString();
    item.file = index.sibling(row, Constants::OUTPUT_COLUMN_FILE).data().toString();
    item.line = index.sibling(row, Constants::OUTPUT_COLUMN_LINE).data().toInt();
    item.color = index.data(Qt::BackgroundColorRole).value<QColor>();
    item.iconResource = index.sibling(row, Constants::OUTPUT_COLUMN_TEXT).data(Qt::DecorationRole).toString();

    emit itemClicked(item);
}

void LintOutputPane::updateCount()
{
    setBadgeNumber(m_itemsModel->rowCount());
}

void LintOutputPane::runLint()
{
    m_itemsModel->clear();
    m_treeView->hide();
    m_btnRun->setEnabled(false);
    m_btnStop->setEnabled(true);
    m_outputConsole->clear();
    m_outputConsole->show();
    m_processor->start();
}

void LintOutputPane::stopLint()
{
    m_processor->stop();
    showResults();
}

void LintOutputPane::showResults()
{
    m_treeView->show();
    m_outputConsole->hide();
    m_btnRun->setEnabled(true);
    m_btnStop->setEnabled(false);
    m_itemsModel->setItemsList(m_processor->items());
    if (!m_processor->lastError().isEmpty())
        QMessageBox::critical(Core::ICore::instance()->mainWindow(), tr("Error"), m_processor->lastError());
}

void LintOutputPane::setMode(int index)
{
    LintProcessor::MODES mode;
    switch(index)
    {
        case 0: mode=LintProcessor::SINGLE_FILE; break;
        case 1: mode=LintProcessor::PROJECT; break;
        default: mode=LintProcessor::SINGLE_FILE; break;
    }
    m_processor->setMode(mode);
}

QModelIndex LintOutputPane::selectedModelIndex()
{
    QModelIndexList selectedIndexes = m_treeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty())
        return QModelIndex();
    else
        // There is only one item selected
        return selectedIndexes.first();
}

QModelIndex LintOutputPane::nextModelIndex()
{
    QModelIndex indexToBeSelected = m_treeView->indexBelow(selectedModelIndex());
    if (!indexToBeSelected.isValid())
        return m_treeView->model()->index(0, 0);
    else
        return indexToBeSelected;

}

QModelIndex LintOutputPane::previousModelIndex()
{
    QModelIndex indexToBeSelected = m_treeView->indexAbove(selectedModelIndex());
    if (!indexToBeSelected.isValid())
        return m_treeView->model()->index(m_treeView->model()->rowCount() - 1, 0);
    else
        return indexToBeSelected;
}

