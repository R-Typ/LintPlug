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

#ifndef LINTITEM_H
#define LINTITEM_H

#include <QMetaType>
#include <QColor>

#include "lintplugconstants.h"

namespace LintPlug {
namespace Internal {

class LintItem
{
public:
    enum LEVELS
    {
        ERROR,
        WARNING,
        INFO,
        NOTE
    };

    LintItem() : level(ERROR), text(), file(), line(-1), code(0) {}
    LEVELS level;
    QString text;
    QString file;
    int line;
    int code;
    QString iconResource;
    QColor color;
    inline bool operator==(const LintItem& other)
    {
        return (this->file==other.file) && (this->line==other.line)
                && (this->code==other.code) && (this->text == other.text) && (this->level == other.level);
    }

    QString levelString(LEVELS level)
    {
        QString res;
        switch (level)
        {
            case ERROR:     res=QObject::tr("Error");   break;
            case WARNING:   res=QObject::tr("Warning"); break;
            case INFO:      res=QObject::tr("Info");    break;
            case NOTE:      res=QObject::tr("Note");    break;
            default:        res=QObject::tr("Unknown"); break;
        }
        return res;
    }

};

class LintItemSortPredicate
{
public:
    explicit LintItemSortPredicate(Constants::OutputColumnIndex columnIndex, Qt::SortOrder order) :
        m_columnIndex(columnIndex),
        m_order(order)
    {}

    inline bool operator()(const LintItem &t1, const LintItem &t2)
    {
        if (m_order == Qt::AscendingOrder)
            return lessThan(t1, t2);
        else
            return lessThan(t2, t1);
    }

    inline bool lessThan(const LintItem &t1, const LintItem &t2)
    {
        switch (m_columnIndex) {
            case Constants::OUTPUT_COLUMN_LEVEL:
            {
                return t1.level < t2.level;
            }

            case Constants::OUTPUT_COLUMN_CODE:
                return t1.code < t2.code;

            case Constants::OUTPUT_COLUMN_TEXT:
                return t1.text < t2.text;

            case Constants::OUTPUT_COLUMN_LINE:
                return t1.line < t2.line;

            case Constants::OUTPUT_COLUMN_FILE:
                return t1.file < t2.file;

            default:
                Q_ASSERT(false);
                return false;
        }
    }

private:
    Constants::OutputColumnIndex m_columnIndex;
    Qt::SortOrder m_order;
};

} // namespace Internal
} // namespace LintPlug


Q_DECLARE_METATYPE(LintPlug::Internal::LintItem)

#endif // LINTITEM_H
