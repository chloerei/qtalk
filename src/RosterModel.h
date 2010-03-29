#ifndef ROSTERMODEL_H
#define ROSTERMODEL_H

#include <QAbstractItemModel>

class QXmppRoster;
class TreeItem;

class RosterModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    class TreeItem
    {
    public:
        enum Type
        {
            root,
            group,
            contact
        };

        TreeItem(Type type, QString data, TreeItem *parent = 0);
        ~TreeItem();
        TreeItem* child(int row);
        void appendChild(TreeItem *child);
        int childCount() const;
        QString data() const;
        TreeItem* parent();
        int row() const;
        Type type() const { return m_type; }
        QList<TreeItem *> childItems() const { return m_childItems; }

    private:
        Type m_type;
        QString m_data;
        QList<TreeItem*> m_childItems;
        TreeItem *m_parent;
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

public slots:
    void parseRoster();

private:
    QXmppRoster *m_roster;
    TreeItem *m_rootItem;
    TreeItem* findOrCreateGroup(QString group);
};

#endif
