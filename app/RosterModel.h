#ifndef ROSTERMODEL_H
#define ROSTERMODEL_H

#include <QAbstractItemModel>
#include "Preferences.h"

class TreeItem;
class QXmppClient;
class QXmppRoster;
class QXmppPresence;
class QXmppVCardManager;
class QXmppVCard;

class RosterModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum ItemType
    {
        root,
        group,   // data => group
        contact, // data => breaJid
        resource // data => resource
    };

    RosterModel(QObject *parent = 0);
    ~RosterModel();

    void setClient(QXmppClient *client);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    ItemType itemTypeAt(const QModelIndex &index) const;
    QString jidAt(const QModelIndex &index) const;
    void messageUnread(const QString &bareJid, const QString &resource);
    void messageReaded(const QString &bareJid, const QString &resource);
    void messageReadedAll(const QString &bareJid);
    void readPref(Preferences *pref);
    QList<QModelIndex> allIndex(const QModelIndex &index = QModelIndex()) const;
    bool isIndexHidden(const QModelIndex &index);
    bool hasVCard(const QString &bareJid);
    QXmppVCard getVCard(const QString &bareJid); // if no exist, return empty vcard
    void clear();

signals:
    void lastOneResource(const QModelIndex &contactIndex);
    void parseDone();
    void hiddenUpdate();

public slots:
    void parseRoster();

private slots:
    void presenceChanged(const QString &bareJid, const QString &resource);
    void vCardRecived(const QXmppVCard&);

private:
    QXmppClient *m_client;
    QXmppRoster *m_roster;
    QXmppVCardManager *m_vCardManager;
    TreeItem* m_rootItem;
    TreeItem* findOrCreateGroup(QString group);
    bool m_hideOffline;
    bool m_showResources;
    bool m_showSingleResource;

    void parsePresence(const QModelIndex &contactIndex, const QString &resource, const QXmppPresence &presence);
    TreeItem* getItem(const QModelIndex &index) const;
    QString displayData(const QModelIndex &index) const;
    void sortContact(const QModelIndex &groupIndex);
    QList<QModelIndex> findContactIndexListForBareJid(const QString &bareJid); // include all resource
    QMap<QString, QXmppVCard> m_vCards; // <bareJid, vcard>
};

#endif
