#ifndef LINTPLUG_H
#define LINTPLUG_H

#include "lintplug_global.h"

#include <extensionsystem/iplugin.h>

namespace LintPlug {
namespace Internal {

class LintOutputPane;
class LintItem;
class ConfigPage;

class LintPlugPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "LintPlug.json")

public:
    LintPlugPlugin();
    ~LintPlugPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();

private slots:
    void itemClicked(const LintItem &item);

private:
    LintOutputPane* m_lintOutputPane;
    ConfigPage*     m_configPage;
};

} // namespace Internal
} // namespace LintPlug

#endif // LINTPLUG_H

