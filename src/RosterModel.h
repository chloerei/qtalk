#ifndef ROSTERMODEL_H
#define ROSTERMODEL_H

#include <QAbstractItemModel>

class QXmppRoster;
class TreeItem;
class QXmppPresence;

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

    void setRoster(QXmppRoster *roster);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    ItemType itemTypeAt(const QModelIndex &index) const;
    QString jidAt(const QModelIndex &index) const;

signals:
    void lastOneResource(const QModelIndex &contactIndex);

public slots:
    void parseRoster();

private slots:
    void presenceChanged(const QString &bareJid, const QString &resource);

private:
    QXmppRoster *m_roster;
    TreeItem* m_rootItem;
    TreeItem* findOrCreateGroup(QString group);

    void parsePresence(TreeItem *groupItem, TreeItem *contactItem, const QString &resource, const QXmppPresence &presence);
    TreeItem* getItem(const QModelIndex &index) const;
};

#endif
