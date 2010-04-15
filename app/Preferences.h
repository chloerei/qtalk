#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

class Preferences
{
public:
    Preferences();
    void load();
    void save();

    // Account
    QString jid;
    QString password;
    QString host;
    int port;
    bool storePassword;
    bool autoLogin;

};

#endif // PREFERENCES_H
