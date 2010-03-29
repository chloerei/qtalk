#include "RosterModel.h"
#include "QXmppRoster.h"

class RosterModelTreeItem
{
public:
    enum Type
    {
        root,
        group,
        contact
    };

    RosterModelTreeItem(Type type, QString data, RosterModelTreeItem *parent = 0);
    ~RosterModelTreeItem();
    RosterModelTreeItem* child(int row);
    void appendChild(RosterModelTreeItem *child);
    int childCount() const;
    QString data() const;
    RosterModelTreeItem* parent();
    int row() const;
    Type type() const { return m_type; }
    QList<RosterModelTreeItem *> childItems() const { return m_childItems; }

private:
    Type m_type;
    QString m_data;
    QList<RosterModelTreeItem*> m_childItems;
    RosterModelTreeItem *m_parent;
};

RosterModelTreeItem::RosterModelTreeItem(Type type, QString data, RosterModelTreeItem *parent)
    : m_type(type), m_data(data), m_parent(parent)
{
}

RosterModelTreeItem::~RosterModelTreeItem()
{
    qDeleteAll(m_childItems);
}

RosterModelTreeItem* RosterModelTreeItem::child(int row)
{
    return m_childItems.value(row);
}

void RosterModelTreeItem::appendChild(RosterModelTreeItem *child)
{
    m_childItems.append(child);
}

int RosterModelTreeItem::childCount() const
{
    return m_childItems.count();
}

QString RosterModelTreeItem::data() const
{
    return m_data;
}

RosterModelTreeItem* RosterModelTreeItem::parent()
{
    return m_parent;
}

int RosterModelTreeItem::row() const
{
    if (m_parent)
        return m_parent->childItems().indexOf(const_cast<RosterModelTreeItem*>(this));

    return 0;
}

RosterModel::RosterModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new RosterModelTreeItem(RosterModelTreeItem::root, "root");
}

RosterModel::~RosterModel()
{
    delete m_rootItem;
}

void RosterModel::setRoster(QXmppRoster *roster)
{
    m_roster = roster;
    parseRoster();
    reset();
}

void RosterModel::parseRoster()
{
    foreach (QXmppRoster::QXmppRosterEntry entry, m_roster->getRosterEntries()) {
        qDebug() << entry.groups().count();
        if (entry.groups().isEmpty()) {
            RosterModelTreeItem *groupItem = findOrCreateGroup("nogroup");
            RosterModelTreeItem *item = new RosterModelTreeItem(RosterModelTreeItem::contact, entry.bareJid(), groupItem);
            groupItem->appendChild(item);
        } else {
            foreach (QString group, entry.groups()) {
                if (group.isEmpty())
                    group = "nogroup";
                RosterModelTreeItem *groupItem = findOrCreateGroup(group);
                RosterModelTreeItem *item = new RosterModelTreeItem(RosterModelTreeItem::contact, entry.bareJid(), groupItem);
                groupItem->appendChild(item);
            }
        }
    }
}

RosterModelTreeItem* RosterModel::findOrCreateGroup(QString group)
{
    foreach (RosterModelTreeItem *item, m_rootItem->childItems()) {
        if (item->type() == RosterModelTreeItem::group && item->data() == group) {
            return item;
        }
    }

    // create
    RosterModelTreeItem *newItem = new RosterModelTreeItem(RosterModelTreeItem::group, group, m_rootItem);
    m_rootItem->appendChild(newItem);
    return newItem;
}

QModelIndex RosterModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    RosterModelTreeItem *parentItem;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<RosterModelTreeItem*>(parent.internalPointer());

    RosterModelTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

Qt::ItemFlags RosterModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant RosterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    RosterModelTreeItem *item = static_cast<RosterModelTreeItem*>(index.internalPointer());

    return item->data();
}

RosterModel::RosterModelTreeItem::Type RosterModel::type()
{
    RosterModelTreeItem *item = static_cast<RosterModelTreeItem*>(index.internalPointer());
    return item->type(); 
}

QVariant RosterModel::headerData(int section, Qt::Orientation orientation,
        int role) const
{
    return QVariant();
}


QModelIndex RosterModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    RosterModelTreeItem *childItem = static_cast<RosterModelTreeItem*>(index.internalPointer());
    RosterModelTreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int RosterModel::rowCount(const QModelIndex &parent) const
{
    RosterModelTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<RosterModelTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int RosterModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
