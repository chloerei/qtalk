#include "RosterModel.h"
#include "QXmppRoster.h"

class TreeItem
{
public:
    TreeItem(RosterModel::ItemType type, QString data, TreeItem *parent = 0);
    ~TreeItem();
    TreeItem* child(int row);
    void appendChild(TreeItem *child);
    int childCount() const;
    QString data() const;
    TreeItem* parent();
    int row() const;
    RosterModel::ItemType type() const { return m_type; }
    QList<TreeItem *> childItems() const { return m_childItems; }

private:
    RosterModel::ItemType m_type;
    QString m_data;
    QList<TreeItem*> m_childItems;
    TreeItem *m_parent;
};


TreeItem::TreeItem(RosterModel::ItemType type, QString data, TreeItem *parent)
    : m_type(type), m_data(data), m_parent(parent)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

TreeItem* TreeItem::child(int row)
{
    return m_childItems.value(row);
}

void TreeItem::appendChild(TreeItem *child)
{
    m_childItems.append(child);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

QString TreeItem::data() const
{
    return m_data;
}

TreeItem* TreeItem::parent()
{
    return m_parent;
}

int TreeItem::row() const
{
    if (m_parent)
        return m_parent->childItems().indexOf(const_cast<TreeItem*>(this));

    return 0;
}

RosterModel::RosterModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TreeItem(root, "root");
}

RosterModel::~RosterModel()
{
    delete m_rootItem;
}

void RosterModel::setRoster(QXmppRoster *roster)
{
    m_roster = roster;
    parseRoster();
    connect(m_roster, SIGNAL(presenceChanged(const QString, const QString)),
            this, SLOT(presenceChanged(const QString, const QString)));
    reset();
}

void RosterModel::parseRoster()
{
    foreach (QXmppRoster::QXmppRosterEntry entry, m_roster->getRosterEntries()) {
        //qDebug() << entry.groups().count();
        if (entry.groups().isEmpty()) {
            TreeItem *groupItem = findOrCreateGroup("nogroup");
            TreeItem *item = new TreeItem(contact, entry.bareJid(), groupItem);
            groupItem->appendChild(item);
        } else {
            foreach (QString group, entry.groups()) {
                if (group.isEmpty())
                    group = "nogroup";
                TreeItem *groupItem = findOrCreateGroup(group);
                TreeItem *item = new TreeItem(contact, entry.bareJid(), groupItem);
                groupItem->appendChild(item);
            }
        }
    }
}

TreeItem* RosterModel::findOrCreateGroup(QString group)
{
    foreach (TreeItem *item, m_rootItem->childItems()) {
        if (item->type() == RosterModel::group && item->data() == group) {
            return item;
        }
    }

    // create
    TreeItem *newItem = new TreeItem(RosterModel::group, group, m_rootItem);
    m_rootItem->appendChild(newItem);
    return newItem;
}

QModelIndex RosterModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
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

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data();
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

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int RosterModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int RosterModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void RosterModel::presenceChanged(const QString &bareJid, const QString &resource)
{
}

RosterModel::ItemType RosterModel::itemTypeAt(const QModelIndex &index) const
{
    return ((TreeItem *)index.internalPointer())->type();
}

QString RosterModel::jidAt(const QModelIndex &index) const
{
    TreeItem *item = (TreeItem *)index.internalPointer();
    if (item->type() == RosterModel::contact) {
        return item->data();
    }
    return "";
}
