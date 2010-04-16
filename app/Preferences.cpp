#include "Preferences.h"
#include <QSettings>

Preferences::Preferences()
{
}

void Preferences::load()
{
    QSettings settings;

    // General
    settings.beginGroup("general");
    hideOffline = settings.value("hideOffline").toBool();
    settings.endGroup();

    // Account
    settings.beginGroup("account");
    jid = settings.value("jid").toString();
    storePassword = settings.value("storePassword", false).toBool();
    if (storePassword)
        password = settings.value("password").toString();
    else
        password.clear();
    host = settings.value("host").toString();
    port = settings.value("port", 5222).toInt();
    autoLogin = settings.value("autoLogin", false).toBool();
    settings.endGroup();
}

void Preferences::save()
{
    QSettings settings;

    // General
    settings.beginGroup("general");
    settings.setValue("hideOffline", hideOffline);
    settings.endGroup();

    settings.beginGroup("account");
    settings.setValue("jid", jid);
    if (storePassword)
        settings.setValue("password", password);
    else
        settings.remove("password");
    settings.setValue("host", host);
    settings.setValue("port", port);
    settings.setValue("storePassword", storePassword);
    settings.setValue("autoLogin", autoLogin);
    settings.endGroup();
}
