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
    language = settings.value("language").toString();
    hideOffline = settings.value("hideOffline", false).toBool();
    showResources = settings.value("showResources", true).toBool();
    showSingleResource = settings.value("showSingleResource", false).toBool();
    rosterIconSize = settings.value("rosterIconSize", 32).toInt();
    closeToTray = settings.value("closeToTray", true).toBool();
    closeToTrayNotice = settings.value("closeToTrayNotice", true).toBool();
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

    // ChatWindow
    settings.beginGroup("chatwindow");
    enterToSendMessage = settings.value("enterToSendMessage", false).toBool();
    settings.endGroup();

    // mainWindow
    settings.beginGroup("mainWindow");
    mainWindowGeometry = settings.value("geometry").toByteArray();
    mainWindowState = settings.value("state").toByteArray();
    settings.endGroup();
}

void Preferences::save()
{
    QSettings settings;

    // General
    settings.beginGroup("general");
    settings.setValue("language", language);
    settings.setValue("hideOffline", hideOffline);
    settings.setValue("showResources", showResources);
    settings.setValue("showSingleResource", showSingleResource);
    settings.setValue("rosterIconSize", rosterIconSize);
    settings.setValue("closeToTray", closeToTray);
    settings.setValue("closeToTrayNotice", closeToTrayNotice);
    settings.endGroup();

    // Account
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

    // ChatWindow
    settings.beginGroup("chatwindow");
    settings.setValue("enterToSendMessage", enterToSendMessage);
    settings.endGroup();

    // mainWindow
    settings.beginGroup("mainWindow");
    settings.setValue("geometry", mainWindowGeometry);
    settings.setValue("state", mainWindowState);
    settings.endGroup();
}
