#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

class Preferences
{
public:
    Preferences();
    void load();
    void save();

    // General
    bool hideOffline;
    bool showResources;
    bool showSingleResource;
    int rosterIconSize;
    bool closeToTray;
    bool closeToTrayNotice;

    // Account
    QString jid;
    QString password;
    QString host;
    int port;
    bool storePassword;
    bool autoLogin;

    // ChatWindow
    bool enterToSendMessage;

    // Mainwindow
    QByteArray mainWindowGeometry;
    QByteArray mainWindowState;
};

#endif // PREFERENCES_H
