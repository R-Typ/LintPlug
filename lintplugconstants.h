#ifndef LINTPLUGCONSTANTS_H
#define LINTPLUGCONSTANTS_H

namespace LintPlug {
namespace Constants {

const char ACTION_ID[] = "LintPlug.Action";
const char MENU_ID[] = "LintPlug.Menu";

const char COLOR_INFO_BG[] = "#ccffcc";
const char COLOR_WARNING_BG[] = "#ffffcc";
const char COLOR_ERROR_BG[] = "#ffcccc";
const char COLOR_NOTE_BG[] = "#e0ebff";

const char SETTINGS_GROUP[] = "LintPlugin";
const char SETTINGS_LEVEL_COLUMN_WIDTH[] = "LevelColumnWidth";
const char SETTINGS_TEXT_COLUMN_WIDTH[] = "TextColumnWidth";
const char SETTINGS_FILE_COLUMN_WIDTH[] = "FileColumnWidth";
const char SETTINGS_LINT_EXE[] = "LintExe";
const char SETTINGS_LINT_ARGS[] = "LintArgs";
const char SETTINGS_LINT_DEFS[] = "LintDefs";

enum OutputColumnIndex {
    OUTPUT_COLUMN_LEVEL,
    OUTPUT_COLUMN_TEXT,
    OUTPUT_COLUMN_FILE,
    OUTPUT_COLUMN_LINE,
    OUTPUT_COLUMN_COUNT
};

const char OUTPUT_PANE_TITLE[] = QT_TRANSLATE_NOOP("Lint::Internal::OutputPane", "PC Lint Check");
const char OUTPUT_COLUMN_LEVEL_TITLE[] = QT_TRANSLATE_NOOP("Lint::Internal::ItemsModel", "Level");
const char OUTPUT_COLUMN_TEXT_TITLE[] = QT_TRANSLATE_NOOP("Lint::Internal::ItemsModel", "Description");
const char OUTPUT_COLUMN_FILE_TITLE[] = QT_TRANSLATE_NOOP("Lint::Internal::ItemsModel", "File");
const char OUTPUT_COLUMN_LINE_TITLE[] = QT_TRANSLATE_NOOP("Lint::Internal::ItemsModel", "Line");

const char ICON_RUN[] = ":/images/run_small.png";
const char ICON_STOP[] = ":/images/stop_small.png";

const int OUTPUT_TOOLBAR_SPACER_WIDTH = 25;

} // namespace LintPlug
} // namespace Constants

#endif // LINTPLUGCONSTANTS_H

