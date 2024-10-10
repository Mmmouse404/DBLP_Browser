QT       += core gui \
            multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    finder.cpp \
    main.cpp \
    mainwindow.cpp \
    parser.cpp \
    parserdialog.cpp \
    pivoter.cpp \
    port.cpp \
    recorder.cpp \
    settingdialog.cpp \
    util.cpp

HEADERS += \
    finder.h \
    mainwindow.h \
    parser.h \
    parserdialog.h \
    pivoter.h \
    port.h \
    recorder.h \
    settingdialog.h \
    util.h

FORMS += \
    mainwindow.ui \
    parserDialog.ui \
    settingdialog.ui

TRANSLATIONS += \
    translation/DBLPBrowser_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons/background.qrc \
    translation/translation.qrc

DISTFILES +=
