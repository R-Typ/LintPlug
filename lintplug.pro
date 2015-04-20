# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=C:/qtcreator/qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
#debug
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=C:/qtcreator/qt-creator-build
#release
#isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=C:/qtcreator/qt-creator-build_release

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = LintPlug
QTC_LIB_DEPENDS += \
    cplusplus \
    extensionsystem \
    utils
#\
#    qmlprojectmanager

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    projectexplorer \
    cpptools

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

DEFINES += LINTPLUG_LIBRARY

INCLUDEPATH += $$QTCREATOR_SOURCES/src/shared

LIBS += -L"C:/qtcreator/qt-creator-build/lib" -L"C:/qtcreator/qt-creator-build/lib/qtcreator" -L"C:/qtcreator/qt-creator-build/lib/qtcreator/plugins" -lQmlProjectManagerd
# LintPlug files

SOURCES += linttreeview.cpp \
    lintoutputpane.cpp \
    lintitemsmodel.cpp \
    lintplugplugin.cpp \
    lintprocessor.cpp \
    config.cpp \
    configpage.cpp

HEADERS +=  lintplug_global.h \
            lintplugconstants.h \
            linttreeview.h \
            lintitem.h \
            lintoutputpane.h \
            lintitemsmodel.h \
            lintplugplugin.h \
            lintprocessor.h \
    config.h \
    configpage.h

FORMS += \
    config.ui

