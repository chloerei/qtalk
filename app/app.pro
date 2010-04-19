TEMPLATE = app
#TARGET = app
QT += network xml

# Tmp file
#MOC_DIR = tmp/moc
#RCC_DIR = tmp/rcc
#UI_DIR = tmp/ui
#OBJECTS_DIR = tmp/obj

# Config qxmpp
CONFIG += console debug_and_release
INCLUDEPATH += ../lib/QXmppClient/source

CONFIG(debug, debug|release) {
    QXMPP_LIB = QXmppClient_d
    QXMPP_DIR = ../lib/QXmppClient/source/debug
    TARGET = app_d
} else {
    QXMPP_LIB = QXmppClient
    QXMPP_DIR = ../lib/QXmppClient/source/release
    TARGET = app
}

LIBS += -L$$QXMPP_DIR -l$$QXMPP_LIB
PRE_TARGETDEPS += $${QXMPP_DIR}/lib$${QXMPP_LIB}.a

# Input

RESOURCES = application.qrc

SOURCES += main.cpp \
           MainWindow.cpp \
           ChatWindow.cpp \
           XmppClient.cpp \
           XmppMessage.cpp \
           RosterModel.cpp \
           UnreadMessageWindow.cpp \
           UnreadMessageModel.cpp \
           LoginWidget.cpp \
           PreferencesDialog.cpp \
           PrefAccount.cpp \
           Preferences.cpp \
           PrefWidget.cpp \
           PrefGeneral.cpp \
           CloseNoticeDialog.cpp \
    MessageEditor.cpp
HEADERS += MainWindow.h \
           ChatWindow.h \
           XmppClient.h \
           XmppMessage.h \
           RosterModel.h  \
           UnreadMessageWindow.h \
           UnreadMessageModel.h \
           LoginWidget.h \
           PreferencesDialog.h \
           PrefAccount.h \
           Preferences.h \
           PrefWidget.h \
           PrefGeneral.h \
           CloseNoticeDialog.h \
    MessageEditor.h
FORMS   += MainWindow.ui \
           UnreadMessageWindow.ui \
           LoginWidget.ui \
           ChatWindow.ui \
           PreferencesDialog.ui \
           PrefAccount.ui \
           PrefGeneral.ui \
           CloseNoticeDialog.ui \
    MessageEditor.ui
