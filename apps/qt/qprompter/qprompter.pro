CONFIG       += release
HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp
RESOURCES     = qprompter.qrc

QT           += widgets
CONFIG       += qscintilla2

CONFIG += link_pkgconfig
PKGCONFIG += presage

# install
target.path = $$[QT_INSTALL_EXAMPLES]/mainwindows/qprompter
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS qprompter.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/mainwindows/qprompter
INSTALLS += target sources

symbian: include($$PWD/../../symbianpkgrules.pri)
maemo5: include($$PWD/../../maemo5pkgrules.pri)

symbian: warning(This example might not fully work on Symbian platform)
simulator: warning(This example might not fully work on Simulator platform)
