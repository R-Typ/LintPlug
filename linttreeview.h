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

#ifndef LINTTREEVIEW_H
#define LINTTREEVIEW_H

#include <utils/itemviews.h>

namespace LintPlug {
namespace Internal {

class LintTreeView : public Utils::TreeView
{
    Q_OBJECT

public:
    explicit LintTreeView(QWidget *parent = 0);
    ~LintTreeView();
    void resizeEvent(QResizeEvent *event);

private slots:
    void columnResized(int column, int oldSize, int newSize);

private:
    void saveDisplaySettings();
    void loadDisplaySettings();

    QVector<qreal> m_colWidth;

};

} // namespace Internal
} // namespace LintPlug

#endif // LINTTREEVIEW_H
