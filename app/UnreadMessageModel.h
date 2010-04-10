#ifndef UNREADMESSAGEMODEL
#define UNREADMESSAGEMODEL

#include <QAbstractListModel>
#include <QMap>
#include <QList>
#include "QXmppMessage.h"
#include <QModelIndex>
#include <QVariant>

class UnreadMessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    UnreadMessageModel(QObject *parent = 0);

    void add(const QXmppMessage &message);
    QList<QXmppMessage> take(QString jid);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool hasUnread(const QString &jid) const;
    QString jidAt(const QModelIndex &index) const;

signals:
    void messageCleared();

private:
    QMap<QString, QList<QXmppMessage> > m_messageStore;
};
#endif
