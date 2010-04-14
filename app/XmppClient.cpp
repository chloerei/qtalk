#include "XmppClient.h"
#include "QXmppLogger.h"
#include "QXmppMessage.h"
#include "QXmppRoster.h"
#include <iostream>
#include <QTreeView>

XmppClient::XmppClient(QObject *parent)
    : QXmppClient(parent)
{
    QXmppLogger::getLogger()->setLoggingType(QXmppLogger::STDOUT);
}

XmppClient::~XmppClient()
{

}

void XmppClient::disconnect()
{
    QXmppClient::disconnect();
}
