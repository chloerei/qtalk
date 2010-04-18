#include "RosterModel.h"
#include "QXmppRoster.h"
#include "QXmppUtils.h"
#include "QXmppVCardManager.h"
#include "QXmppVCard.h"
#include <QIcon>

class TreeItem
{
public:
    TreeItem(RosterModel::ItemType type, QString data, TreeItem *parent = 0);
    ~TreeItem();
    TreeItem* child(int row, bool hideOnline = false);
    void appendChild(TreeItem *child);
    bool removeOne(TreeItem *child);
    int childCount(bool hideOffline = false) const;
    QString data() const;
    TreeItem* parent();
    int childNumber() const;
    RosterModel::ItemType type() const { return m_type; }
    QList<TreeItem *> childItems() const { return m_childItems; }
    QList<TreeItem *> onlineChildItems() const; // only use for group
    void sortChildren();
    void setUnread(bool unread = true);
    bool isUnread() const;
    bool hasChlidContain(const QString &data) const;
    int childIndexOfData(const QString &data) const;

private:
    RosterModel::ItemType m_type;
    QString m_data;
    QList<TreeItem*> m_childItems;
    TreeItem *m_parent;
    bool m_unread;
};

bool TreeItemCompare(TreeItem *s1, TreeItem *s2)
{
    if (s1->type() == RosterModel::contact && s2->type() == RosterModel::contact) {
        //return s1->childCount() >= s2->childCount();
        if (s1->childCount() == s2->childCount())
            return s1 < s2;
        else
            return s1->childCount() >= s2->childCount();
    }
    return true;
}

TreeItem::TreeItem(RosterModel::ItemType type, QString data, TreeItem *parent)
    : m_type(type), m_data(data), m_parent(parent), m_unread(false)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

TreeItem* TreeItem::child(int row, bool hideOnline)
{
    if (hideOnline && m_type == RosterModel::group)
        return onlineChildItems().value(row);

    return m_childItems.value(row);
}

void TreeItem::appendChild(TreeItem *child)
{
    m_childItems.append(child);
}

bool TreeItem::removeOne(TreeItem *child)
{
    bool check = m_childItems.removeOne(child);
    if (check)
        delete child;
    return check;
}

int TreeItem::childCount(bool hideOffline) const
{
    if (hideOffline && m_type == RosterModel::group) {
        return onlineChildItems().count();
    }

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

int TreeItem::childNumber() const
{
    if (m_parent)
        return m_parent->childItems().indexOf(const_cast<TreeItem*>(this));

    return 0;
}

QList<TreeItem *> TreeItem::onlineChildItems() const
{
    QList<TreeItem *> items;
    if (m_type != RosterModel::group)
        return items;
    foreach (TreeItem *item, m_childItems) {
        if (item->childCount() != 0) {
            items.append(item);
        }
    }

    return items;
}

void TreeItem::sortChildren()
{
    qSort(m_childItems.begin(), m_childItems.end(), TreeItemCompare);
}

void TreeItem::setUnread(bool unread)
{
    m_unread = unread;
}

bool TreeItem::isUnread() const
{
    if (m_type == RosterModel::contact) {
        if (m_childItems.count() != 0) {
            foreach (TreeItem *childItem, m_childItems) {
                if (childItem->isUnread())
                    return true;
            }
        } 
    } else if (m_type == RosterModel::resource) {
        return m_unread;
    }
    return false;
}

bool TreeItem::hasChlidContain(const QString &data) const
{
    foreach (TreeItem *resourceItem, childItems()) {
        if (resourceItem->data() == data) {
            return true;
        }
    }
    return false;
}

int TreeItem::childIndexOfData(const QString &data) const
{
    if (hasChlidContain(data)) {
        foreach (TreeItem *resourceItem, childItems()) {
            if (resourceItem->data() == data) {
                return m_childItems.indexOf(resourceItem);
            }
        }
    }
    return -1;
}

RosterModel::RosterModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_hideOffline(false),
    m_showResources(false)
{
    m_rootItem = new TreeItem(root, "root");
}

RosterModel::~RosterModel()
{
    delete m_rootItem;
}

void RosterModel::setClient(QXmppClient *client)
{
    m_client = client;
    m_roster = &client->getRoster();
    m_vCardManager = &client->getVCardManager();
    connect(m_roster, SIGNAL(presenceChanged(const QString, const QString)),
            this, SLOT(presenceChanged(const QString, const QString)));
    connect(m_roster, SIGNAL(rosterReceived()),
            this, SLOT(parseRoster()) );
    connect(m_vCardManager, SIGNAL(vCardReceived(const QXmppVCard&)),
            this, SLOT(vCardRecived(const QXmppVCard&)) );
    reset();
}

void RosterModel::parseRoster()
{
    foreach (QString bareJid, m_roster->getRosterBareJids()) {
        QXmppRoster::QXmppRosterEntry entry = m_roster->getRosterEntry(bareJid);
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
    emit parseDone();
}

void RosterModel::vCardRecived(const QXmppVCard &vCard)
{
    m_vCards[vCard.from()] = vCard;
    foreach (QModelIndex index, findContactIndexListForBareJid(vCard.from())) {
        dataChanged(index, index);
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
    if (column != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row, m_hideOffline);
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

    if (role == Qt::DisplayRole) {

        TreeItem *item = getItem(index);

        QString output = item->data() + presenceStatusTypeStrFor(index);
        if (item->isUnread())
            output = QString("[*] ") + output;
        return output;
    }

    if (role == Qt::DecorationRole) {
        ItemType type = getItem(index)->type();
        if (type == group) {
            QImage image(":/image/folder.png");
            return QIcon(QPixmap::fromImage(image.scaled(QSize(24, 24))));
        } else if (type == contact) {
            if (m_vCards.contains(jidAt(index))
                && !m_vCards[jidAt(index)].photoAsImage().isNull()) {
                QImage image = m_vCards[jidAt(index)].photoAsImage().scaled(QSize(64, 64));
                return QIcon(QPixmap::fromImage(image));
            } else {
                return QIcon(":/image/contact.png");
            }
        } else {
            return QVariant();
        }
    }

    return QVariant();
}

QVariant RosterModel::headerData(int /* section */, Qt::Orientation /* orientation */,
        int /* role */) const
{
    return QVariant();
}


QModelIndex RosterModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int RosterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;

    TreeItem *parentItem = getItem(parent);

    // What to hide resource when there is only one resource?
    //if (parentItem->type() == RosterModel::contact && parentItem->childCount() < 2) 
    //    return 0;

    if (parentItem->type() == contact) {
        if (!m_showResources)
            return 0;

        if (m_showResources
            && !m_showSingleResource
            && parentItem->childCount() < 2) {
            return 0;
        }
    }

    return parentItem->childCount();
}

int RosterModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;
}

void RosterModel::presenceChanged(const QString &bareJid, const QString &resource)
{
    QXmppPresence presence = m_roster->getPresence(bareJid, resource);
    //QXmppRoster::QXmppRosterEntry entry = m_roster->getRosterEntry(bareJid);

    QList<QModelIndex> contactIndexList = findContactIndexListForBareJid(bareJid);
    foreach (QModelIndex index, contactIndexList) {
        parsePresence(index, resource, presence);
    }

    // request vcard if no exist
    if (!m_vCards.contains(bareJid)) {
        m_vCardManager->requestVCard(bareJid);
    }
}

void RosterModel::parsePresence(const QModelIndex &contactIndex, const QString &resource, const QXmppPresence &presence)
{
    QModelIndex groupIndex = contactIndex.parent();
    TreeItem *contactItem = getItem(contactIndex);

    if (presence.from().isEmpty()) {
        // Unavaliable
        foreach (TreeItem *resourceItem, contactItem->childItems()) {
            if (resourceItem->data() == resource) {
                beginRemoveRows(contactIndex, resourceItem->childNumber(), resourceItem->childNumber());
                contactItem->removeOne(resourceItem);
                endRemoveRows();
                //if (contactItem->childCount() == 1) {
                //    emit lastOneResource(contactIndex);
                //}
                emit dataChanged(contactIndex, contactIndex);
                sortContact(groupIndex);
            }
        }
    } else {
        if (contactItem->hasChlidContain(resource)){
            // update resource
            QModelIndex currentIndex = index(contactItem->childIndexOfData(resource), 0, contactIndex);
            emit dataChanged(currentIndex, currentIndex);
        } else {
            // add resource
            int row = contactItem->childCount();
            beginInsertRows(contactIndex, row, row);
            TreeItem *resourceItem = new TreeItem(RosterModel::resource, resource, contactItem);
            contactItem->appendChild(resourceItem);
            endInsertRows();
            sortContact(groupIndex);
        }
    }
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
    } else if (item->type() == RosterModel::resource) {
        return item->parent()->data() + "/" + item->data();
    }
    return "";
}


TreeItem* RosterModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return m_rootItem;
}

QString RosterModel::presenceStatusTypeStrFor(const QModelIndex &index) const
{
    TreeItem *item = getItem(index);
    if (item->type() == RosterModel::group) {
        return QString("[ %1 / %2 ]").arg(item->childCount(true)).arg(item->childCount());
    } else if (item->type() == RosterModel::contact) {
        if (item->childCount() >0) {
            return " [Available]";
        } else {
            return " [Unavailable]";
        }
    } else if (item->type() == RosterModel::resource) {
        return m_roster->getPresence(item->parent()->data(), item->data()).getStatus().getTypeStr();
    } else {
        return QString();
    }
}

void RosterModel::sortContact(const QModelIndex &groupIndex)
{
    emit layoutAboutToBeChanged();
    TreeItem *item = getItem(groupIndex);

    if (item->type() == RosterModel::group) {
        item->sortChildren();
    }
    emit layoutChanged();
}

// a message recevie, mark the reaource unread. if resource is unknow, let the contact mark unread
void RosterModel::messageUnread(const QString &bareJid, const QString &resource)
{
    QList<QModelIndex> contactIndexList = findContactIndexListForBareJid(bareJid);
    foreach (QModelIndex contactIndex, contactIndexList) {
        if (getItem(contactIndex)->hasChlidContain(resource)){
            QModelIndex resourceIndex = index(getItem(contactIndex)->childIndexOfData(resource), 0, contactIndex);
            getItem(resourceIndex)->setUnread();
            dataChanged(resourceIndex, resourceIndex);
        } else {
            // resource unknow, let contact unread.
            getItem(contactIndex)->setUnread();
        }
        dataChanged(contactIndex, contactIndex);
    }
}

// when open the jid chatWindow, clean the resource unread state
void RosterModel::messageReaded(const QString &bareJid, const QString &resource)
{
    QList<QModelIndex> contactIndexList = findContactIndexListForBareJid(bareJid);
    foreach (QModelIndex contactIndex, contactIndexList) {
        if (getItem(contactIndex)->hasChlidContain(resource)){
            QModelIndex resourceIndex = index(getItem(contactIndex)->childIndexOfData(resource), 0, contactIndex);
            getItem(resourceIndex)->setUnread(false);
            dataChanged(resourceIndex, resourceIndex);
        }
        // resource unknow, do nothng.
        dataChanged(contactIndex, contactIndex);
    }
}

// when open the bareJid chatWindow, clean all unread state
void RosterModel::messageReadedAll(const QString &bareJid)
{
    QList<QModelIndex> contactIndexList = findContactIndexListForBareJid(bareJid);
    foreach (QModelIndex contactIndex, contactIndexList) {
        getItem(contactIndex)->setUnread(false);
        foreach(TreeItem *resourceItem, getItem(contactIndex)->childItems()) {
            resourceItem->setUnread(false);
        }

        if (getItem(contactIndex)->childCount() != 0) {
            dataChanged(index(0, 0, contactIndex),
                        index(rowCount(contactIndex), 0, contactIndex));
        }

        dataChanged(contactIndex, contactIndex);
    }
}

void RosterModel::readPref(Preferences *pref)
{
    if (pref->hideOffline != m_hideOffline
        || pref->showResources != m_showResources
        || pref->showSingleResource != m_showSingleResource) {
        m_hideOffline = pref->hideOffline;
        m_showResources = pref->showResources;
        m_showSingleResource = pref->showSingleResource;
        reset();
    }
}

QList<QModelIndex> RosterModel::findContactIndexListForBareJid(const QString &bareJid) 
{
    QXmppRoster::QXmppRosterEntry entry = m_roster->getRosterEntry(bareJid);
    QList<QModelIndex> results;
    if (entry.groups().isEmpty()) {
        TreeItem *groupItem = findOrCreateGroup("nogroup");

        foreach (TreeItem *contactItem, groupItem->childItems()) {
            if (contactItem->data() == bareJid) {
                results << createIndex(contactItem->childNumber(), 0, contactItem);
                break;
            }
        }
    } else {
        foreach (QString group, entry.groups()) {
            if (group.isEmpty())
                group = "nogroup";
            TreeItem *groupItem = findOrCreateGroup(group);

            foreach (TreeItem *contactItem, groupItem->childItems()) {
                if (contactItem->data() == bareJid) {
                    results << createIndex(contactItem->childNumber(), 0, contactItem);
                    break;
                }
            }
        }
    }
    return results;
}
