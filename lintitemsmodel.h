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

#ifndef LINTITEMSMODEL_H
#define LINTITEMSMODEL_H

#include <QAbstractTableModel>
#include "lintitem.h"

namespace LintPlug {
namespace Internal {


class LintItemsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit LintItemsModel(QObject *parent = 0);
    ~LintItemsModel();
    void setItemsList(QList<LintItem> *list);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    void clear();
    void setFilterLevel(LintItem::LEVELS level, bool isActive);

private:
    QList<LintItem> *m_itemsList;
    QSet<LintItem::LEVELS> m_levels;
    Constants::OutputColumnIndex m_currentSortColumn;
    Qt::SortOrder m_currentSortOrder;

};

} // namespace Internal
} // namespace LintPlug

#endif // LINTITEMSMODEL_H
