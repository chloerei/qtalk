TEMPLATE = app
TARGET = app
QT += network xml

# Tmp file
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj

# Config qxmpp
QXMPP_DIR = ../lib/QXmppClient/source
INCLUDEPATH += $${QXMPP_DIR}
LIBS += -L $${QXMPP_DIR}/release -lQXmppClient

# Input
SOURCES += main.cpp \
           MainWindow.cpp \
           ChatDialog.cpp \
           XmppClient.cpp \
           RosterModel.cpp
HEADERS += MainWindow.h \
           ChatDialog.h \
           XmppClient.h \
           RosterModel.h
FORMS   += ui/MainWindow.ui \
           ui/ChatDialog.ui
