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

#include "lintitemsmodel.h"
#include <utils/algorithm.h>
#include <utils/theme/theme.h>
#include <QIcon>

using namespace Utils;


using namespace LintPlug::Internal;

LintItemsModel::LintItemsModel(QObject *parent /*= 0*/) : QAbstractTableModel(parent)
, m_itemsList(NULL)
, m_levels()
, m_currentSortColumn(Constants::OUTPUT_COLUMN_LEVEL)
, m_currentSortOrder(Qt::AscendingOrder)
{
    for(int i=0; i<LintItem::LEVELS_COUNT; i++) m_levels.insert(static_cast<LintItem::LEVELS>(i));
}

LintItemsModel::~LintItemsModel()
{    
}

void LintItemsModel::setItemsList(QList<LintItem> *list)
{
    m_itemsList = list;
    sort(m_currentSortColumn, m_currentSortOrder);
}

int LintItemsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    // There's only one level of hierarchy
    if (parent.isValid() || !m_itemsList)
        return 0;

    int cnt(0);
    for(int i=0; i<m_itemsList->count(); i++)
    {
        if (m_levels.contains(m_itemsList->at(i).level)) cnt++;
    }
    return cnt;
}

int LintItemsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return Constants::OUTPUT_COLUMN_COUNT;
}

QVariant LintItemsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    LintItem item;
    int idx(-1);
    for(int i=0; i<m_itemsList->count(); i++)
    {
        item=m_itemsList->at(i);
        if (m_levels.contains(item.level)) idx++;
        if (idx == index.row()) break;
    }
    if (idx == -1) return QVariant();

    if (role == Qt::BackgroundColorRole)
        return item.color;
    if (role == Qt::TextColorRole || role == Qt::ForegroundRole)
        return creatorTheme()->color(Theme::TextColorNormal);

    switch (index.column()) {

        case Constants::OUTPUT_COLUMN_LEVEL:
            switch (role) {
                case Qt::DisplayRole:
                    return LintItem::levelString(item.level);
                case Qt::DecorationRole:
                    return QVariant::fromValue(QIcon(item.iconResource));
            }
            break;

        case Constants::OUTPUT_COLUMN_CODE:
            switch (role) {
                case Qt::DisplayRole:
                    return item.code;
            }
            break;

        case Constants::OUTPUT_COLUMN_TEXT:
            switch (role) {
                case Qt::DisplayRole:
                    return item.text;
            }
            break;

        case Constants::OUTPUT_COLUMN_FILE:
            if (role == Qt::DisplayRole)
                return item.file;
            break;

        case Constants::OUTPUT_COLUMN_LINE:
            if (role == Qt::DisplayRole)
                return item.line;
            break;
    }

    return QVariant();

}

QVariant LintItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
        case Constants::OUTPUT_COLUMN_LEVEL:
            return tr(Constants::OUTPUT_COLUMN_LEVEL_TITLE);

        case Constants::OUTPUT_COLUMN_CODE:
            return tr(Constants::OUTPUT_COLUMN_CODE_TITLE);

        case Constants::OUTPUT_COLUMN_TEXT:
            return tr(Constants::OUTPUT_COLUMN_TEXT_TITLE);

        case Constants::OUTPUT_COLUMN_FILE:
            return tr(Constants::OUTPUT_COLUMN_FILE_TITLE);

        case Constants::OUTPUT_COLUMN_LINE:
            return tr(Constants::OUTPUT_COLUMN_LINE_TITLE);

        default:
            return QVariant();
    }

}

void LintItemsModel::sort(int column, Qt::SortOrder order)
{
    m_currentSortColumn = Constants::OutputColumnIndex(column);
    m_currentSortOrder = order;

    LintItemSortPredicate predicate(m_currentSortColumn, m_currentSortOrder);
    if (m_itemsList) Utils::sort(*m_itemsList, predicate);
    emit layoutChanged();

}

void LintItemsModel::clear()
{
    if (m_itemsList) m_itemsList->clear();
    sort(m_currentSortColumn, m_currentSortOrder);
}

void LintItemsModel::setFilterLevel(LintItem::LEVELS level, bool isActive)
{
    if (isActive) m_levels.insert(level);
    else m_levels.remove(level);
    emit layoutChanged();
}

