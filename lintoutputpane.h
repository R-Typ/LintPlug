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

#ifndef LINTOUTPUTPANE_H
#define LINTOUTPUTPANE_H

#include <coreplugin/ioutputpane.h>

QT_BEGIN_NAMESPACE
class QToolButton;
class QComboBox;
class QModelIndex;
class QAbstractButton;
class QVBoxLayout;
class QLabel;
class QTextEdit;
QT_END_NAMESPACE

namespace LintPlug {
namespace Internal {

class LintItem;
class LintItemsModel;
class LintTreeView;
class LintProcessor;

class LintOutputPane : public Core::IOutputPane
{
    Q_OBJECT

public:
    LintOutputPane(QObject *parent = 0);
    ~LintOutputPane();
    QWidget *outputWidget(QWidget *parent);
    QList<QWidget*> toolBarWidgets() const;
    QString displayName() const;
    int priorityInStatusBar() const;
    void clearContents();
    void visibilityChanged(bool visible);
    void setFocus();
    bool hasFocus() const;
    bool canFocus() const;
    bool canNavigate() const;
    bool canNext() const;
    bool canPrevious() const;
    void goToNext();
    void goToPrev();

signals:
    void itemClicked(const LintItem &item);

private slots:
    void treeViewClicked(const QModelIndex &index);
    void updateCount();
    void runLint();
    void stopLint();
    void switchView();
    void showResults();
    void setMode(int index);

private:
    QWidget*        m_mainWidget;
    LintTreeView*   m_treeView;
    QVBoxLayout*    m_layout;
    QTextEdit*      m_outputConsole;
    QComboBox*      m_cbTool;
    QToolButton*    m_btnRun;
    QToolButton*    m_btnStop;
    QToolButton*    m_btnSwitch;
    LintItemsModel* m_itemsModel;
    LintProcessor*  m_processor;

    void createTreeView();
    void freeTreeView();
    void createScopeButtons();
    void freeScopeButtons();

    QModelIndex selectedModelIndex();
    QModelIndex nextModelIndex();
    QModelIndex previousModelIndex();

};

} // namespace Internal
} // namespace LintPlug

#endif // LINTOUTPUTPANE_H
