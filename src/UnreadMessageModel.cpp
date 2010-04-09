#include "UnreadMessageModel.h"
#include "QXmppUtils.h"

UnreadMessageModel::UnreadMessageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void UnreadMessageModel::add(const QXmppMessage &message)
{
    m_messageStore[jidToBareJid(message.from())] << message;
    reset();
}

QList<QXmppMessage> UnreadMessageModel::take(QString jid)
{
    QString resource = jidToResource(jid);
    QString bareJid = jidToBareJid(jid);
    if (resource.isEmpty()) {
        reset();
        return m_messageStore.take(bareJid);
    } else {
        QList<QXmppMessage> mid = m_messageStore[bareJid];
        QList<QXmppMessage> results;
        m_messageStore.remove(bareJid);
        foreach (QXmppMessage message, mid) {
            if (jidToResource(message.from()) == resource) {
                results << message;
            } else {
                add(message);
            }
        }
        reset();
        return results;
    }
}

int UnreadMessageModel::rowCount(const QModelIndex &parent) const
{
    if (parent != QModelIndex())
        return 0;
    return m_messageStore.keys().count();
}

QVariant UnreadMessageModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_messageStore.keys().at(index.row());
    }
    return QVariant();
}

bool UnreadMessageModel::hasUnread(const QString &jid) const
{
    return m_messageStore.contains(jidToBareJid(jid));
}

QString UnreadMessageModel::jidAt(const QModelIndex &index) const
{
    return m_messageStore.keys().at(index.row());
}
