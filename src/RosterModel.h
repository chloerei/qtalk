#ifndef ROSTERMODEL_H
#define ROSTERMODEL_H

#include <QAbstractItemModel>

class QXmppRoster;
class RosterModelTreeItem;

class RosterModel : public QAbstractItemModel
{
    Q_OBJECT

public:
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
    RosterModelTreeItem *m_rootItem;
    RosterModelTreeItem* findOrCreateGroup(QString group);
};

#endif
