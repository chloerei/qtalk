#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H

#include "QXmppClient.h"
#include "RosterModel.h"

class QTreeView;
class QXmppMessage;

class XmppClient : public QXmppClient
{
    Q_OBJECT

public:
    XmppClient(QObject *parent = 0);
    ~XmppClient();

private:
    RosterModel *m_rosterModel;
    QTreeView *view;
};

#endif
