/**
 *	@file
 *	@author		Roman Nikiforov
 *  @brief
 *
 *	Change History
 * -------------------------------------------------------------------
 *	@date		28.01.2015		header created
 *
 *
 */

#ifndef LINTPROCESSOR_H
#define LINTPROCESSOR_H

#include "lintitem.h"
#include <QList>

namespace LintPlug {
namespace Internal {


class LintProcessor : public QObject
{
    Q_OBJECT

public:
    enum MODES
    {
        SINGLE_FILE,
        PROJECT
    };

    LintProcessor();
    ~LintProcessor();

    void start();
    void stop();
    inline bool isRunning() { return m_isRunning; }
    inline void setMode(MODES mode) { m_mode=mode; }
    inline QString lastError() { return m_lastError; }

    QList<LintItem>* const items();

signals:
    void finished();

private slots:
    void process();

private:
    bool projectData(QStringList& includeDirs, QStringList& defines, QStringList& sources, QString &activeSource);
    bool parse(const QByteArray& data);

    QList<LintItem> m_items;
    QThread*  m_thread;
    bool      m_isRunning;
    MODES     m_mode;
    QString   m_lastError;
};

} // namespace Internal
} // namespace LintPlug

#endif // LINTPROCESSOR_H
