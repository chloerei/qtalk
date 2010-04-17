#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include "QXmppClient.h"

class QTreeView;
class QXmppMessage;

class XmppClient : public QXmppClient
{
    Q_OBJECT

public:
    XmppClient(QObject *parent = 0);
    ~XmppClient();

public slots:
    void disconnect();

private:
    QTreeView *view;
};

#endif
