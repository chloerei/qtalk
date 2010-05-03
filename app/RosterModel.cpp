#include "RosterModel.h"
#include "QXmppRoster.h"
#include "QXmppUtils.h"
#include "QXmppVCardManager.h"
#include "QXmppVCard.h"
#include <QIcon>
#include <QXmppRosterIq.h>

class TreeItem
{
public:
    TreeItem(RosterModel::ItemType type, QString data, TreeItem *parent = 0);
    ~TreeItem();
    TreeItem* child(int row);
    void appendChild(TreeItem *child);
    bool removeOne(TreeItem *child);
    int childCount(bool hideOffline = false) const;
    QString data() const;
    TreeItem* parent();
    int childNumber() const;
    RosterModel::ItemType type() const { return m_type; }
    QList<TreeItem *> childItems() const { return m_childItems; }
    void sortChildren();
    void setUnread(bool unread = true);
    bool isUnread() const;
    bool hasChlidContain(const QString &data) const;
    int childIndexOfData(const QString &data) const;
    void clear();

private:
    RosterModel::ItemType m_type;
    QString m_data;
    QList<TreeItem*> m_childItems;
    TreeItem *m_parent;
    bool m_unread;

    QList<TreeItem *> onlineChildItems() const; // only use for group
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

TreeItem* TreeItem::child(int row)
{
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

void TreeItem::clear()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

RosterModel::RosterModel(QXmppClient *client, QObject *parent) :
    QAbstractItemModel(parent),
    m_hideOffline(false),
    m_showResources(false)
{
    setClient(client);
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
            this, SLOT(presenceChangedSlot(const QString, const QString)));
    connect(m_roster, SIGNAL(rosterReceived()),
            this, SLOT(parseRoster()) );
    connect(m_roster, SIGNAL(rosterChanged(QString)),
             this, SLOT(rosterChangedSlot(QString)) );
    connect(m_vCardManager, SIGNAL(vCardReceived(const QXmppVCard&)),
            this, SLOT(vCardRecived(const QXmppVCard&)) );
}

void RosterModel::parseRoster()
{
    foreach (QString bareJid, m_roster->getRosterBareJids()) {
        addRoster(bareJid);
    }
    reset();
    emit parseDone();
}

void RosterModel::vCardRecived(const QXmppVCard &vCard)
{
    m_vCards[vCard.from()] = vCard;
    foreach (QModelIndex index, indexsForBareJid(vCard.from())) {
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

    TreeItem *item = getItem(index);
    ItemType type = item->type();

    if (role == Qt::DisplayRole) {
        return displayData(index);
    }

    if (role == Qt::ToolTipRole) {
        return toolTipData(index);

    }

    if (role == Qt::DecorationRole) {
        if (type == group) {
            QImage image(":/images/folder.png");
            return QIcon(QPixmap::fromImage(image.scaled(QSize(24, 24))));
        } else if (type == contact) {
            if (item->isUnread()) {
                return QIcon(":/images/mail-unread-new.png");
            }
            if (m_vCards.contains(jidAt(index))
                && !m_vCards[jidAt(index)].photoAsImage().isNull()) {
                QImage image = m_vCards[jidAt(index)].photoAsImage().scaled(QSize(64, 64));
                return QIcon(QPixmap::fromImage(image));
            } else {
                if (item->childCount() == 0)
                    return QIcon(":/images/user-identity-grey.png");
                else
                    return QIcon(":/images/user-identity.png");
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

    return parentItem->childCount();
}

int RosterModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;
}

void RosterModel::presenceChangedSlot(const QString &bareJid, const QString &resource)
{
    QXmppPresence presence = m_roster->getPresence(bareJid, resource);
    //QXmppRoster::QXmppRosterEntry entry = m_roster->getRosterEntry(bareJid);

    QList<QModelIndex> contactIndexList = indexsForBareJid(bareJid);
    foreach (QModelIndex index, contactIndexList) {
        parsePresence(index, resource, presence);
    }

    // request vcard if no exist
    if (!m_vCards.contains(bareJid)) {
        m_vCardManager->requestVCard(bareJid);
    }
}

void RosterModel::rosterChangedSlot(const QString &bareJid)
{
    QList<QModelIndex> indexs = indexsForBareJid(bareJid);
    if (indexs.isEmpty()) {
        addRoster(bareJid);
        reset();
    } else {
        QXmppRoster::QXmppRosterEntry entry = m_roster->getRosterEntry(bareJid);
        if (entry.subscriptionType() == QXmppRoster::QXmppRosterEntry::Remove) {
            foreach (QModelIndex index, indexs) {
                beginRemoveRows(parent(index), index.row(), index.row());
                TreeItem *item = getItem(index);
                item->parent()->removeOne(item);
                endRemoveRows();
            }
        } else {
            foreach (QModelIndex index, indexs) {
                dataChanged(index, index);
                dataChanged(parent(index), parent(index));
            }
        }
    }
}

void RosterModel::addRoster(const QString &bareJid)
{
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
                //emit dataChanged(contactIndex, contactIndex);
                sortContact(groupIndex);
            }
        }
        emit hiddenUpdate();
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
            emit hiddenUpdate();
        }
    }
    emit dataChanged(contactIndex, contactIndex);
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

QString RosterModel::displayData(const QModelIndex &index) const
{
    TreeItem *item = getItem(index);
    if (item->type() == RosterModel::group) {
        return QString("%1 [ %2 / %3 ]").arg(item->data()).arg(item->childCount(true)).arg(item->childCount());
    } else if (item->type() == RosterModel::contact) {
        QXmppVCard vcard = getVCard(item->data());
        QString name;
        QString rosterName = m_roster->getRosterEntry(item->data()).name();
        QString nickName = vcard.nickName();
        QString fullName = vcard.fullName();
        if (!rosterName.isEmpty())
            name = rosterName;
        else if (!nickName.isEmpty())
            name = nickName;
        else if (!fullName.isEmpty())
            name = fullName;
        else
            name = item->data();

        QString statusText = statusTextAt(index);
        if (statusText.isEmpty())
            return name;
        else
            return QString("%1 \n%2").arg(name).arg(statusTextAt(index));
    } else if (item->type() == RosterModel::resource) {
        QString str = item->data();
        if (item->isUnread())
            str = "[*]" + str;
        QString statusText = statusTextAt(index);
        if (statusText.isEmpty())
            return str;
        else
            return QString("%1 \n%2").arg(str).arg(statusText);
    } else {
        return QString();
    }
}

QString RosterModel::toolTipData(const QModelIndex &index) const
{
    TreeItem *item = getItem(index);
    ItemType type = item->type();

    if (type == group)
        return QString();

    QXmppRoster::QXmppRosterEntry entry = m_roster->getRosterEntry(jidToBareJid(jidAt(index)));
    QString subscriptionStr = "";
    switch (entry.subscriptionType()) {
    case QXmppRoster::QXmppRosterEntry::NotSet:
        subscriptionStr = QString(tr("NotSet"));
        break;
    case QXmppRoster::QXmppRosterEntry::None:
        subscriptionStr = QString(tr("None"));
        break;
    case QXmppRoster::QXmppRosterEntry::Both:
        subscriptionStr = QString(tr("Both"));
        break;
    case QXmppRoster::QXmppRosterEntry::From:
        subscriptionStr = QString(tr("From"));
        break;
    case QXmppRoster::QXmppRosterEntry::To:
        subscriptionStr = QString(tr("To"));
        break;
    case QXmppRoster::QXmppRosterEntry::Remove:
        subscriptionStr = QString(tr("Remove"));
        break;
    }

    if (type == contact) {
        QString resource;
        if (item->childCount() == 0) {
            resource = QString(tr("No Resource"));
        } else if (item->childCount() == 1) {
            resource = item->child(0)->data();
        } else {
            resource = QString(tr("Multi Resources"));
        }
        return QString(tr("Name: ")) + entry.name() + "\n"
                + QString(tr("Jabber ID: ")) + entry.bareJid() + "\n"
                + QString(tr("Status: ")) + statusTextAt(index) + "\n"
                + QString(tr("Resource: ")) + resource + "\n"
                + QString(tr("Subscription Type: ")) + subscriptionStr;
    }

    if (type == resource) {
        return QString(tr("Name: ")) + entry.name() + "\n"
                + QString(tr("Jabber ID: ")) + entry.bareJid() + "\n"
                + QString(tr("Status: ")) + statusTextAt(index) + "\n"
                + QString(tr("Resource: ")) + item->data() + "\n"
                + QString(tr("Subscription Type: ")) + subscriptionStr;
    }

    return QString();
}

QString RosterModel::statusTextAt(const QModelIndex &index) const
{
    TreeItem *item = getItem(index);
    if (item->type() == group)
        return QString();

    if (item->type() == resource) {
        QXmppPresence presence = m_roster->getPresence(jidToBareJid(jidAt(index)),
                                                       jidToResource(jidAt(index)));
        if (presence.getStatus().getTypeStr().isEmpty())
            return QString();
        else
            return QString("%1 %2").arg(presence.getStatus().getTypeStr()).arg(presence.getStatus().getStatusText());
    } else {
        if (item->childCount() == 0) {
            return QString(tr("Offline"));
        } else if (item->childCount() == 1) {
            QXmppPresence presence = m_roster->getPresence(item->data(), item->child(0)->data());
            if (presence.getStatus().getTypeStr().isEmpty())
                return QString();
            else
                return QString("%1 %2").arg(presence.getStatus().getTypeStr()).arg(presence.getStatus().getStatusText());
        } else {
            return QString(tr("Multi Status"));
        }
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
    QList<QModelIndex> contactIndexList = indexsForBareJid(bareJid);
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
    QList<QModelIndex> contactIndexList = indexsForBareJid(bareJid);
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
    QList<QModelIndex> contactIndexList = indexsForBareJid(bareJid);
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
    }
}

QList<QModelIndex> RosterModel::allIndex(const QModelIndex &parent) const
{
    QList<QModelIndex> results;
    for (int i = 0; i < rowCount(parent); i++) {
        QModelIndex child = index(i, 0, parent);
        results << child;
        if (rowCount(child) != 0)
            results << allIndex(child);
    }
    return results;
}

bool RosterModel::isIndexHidden(const QModelIndex &index)
{
    if (index == QModelIndex())
        return false;

    TreeItem *item = getItem(index);
    switch (item->type()) {
    case root:
        return false;
    case group:
        if (item->childCount(m_hideOffline) == 0)
            return true;
        else
            return false;
        // will return
    case contact:
        if (m_hideOffline && item->childCount() == 0) {
            return true;
        } else {
            return false;
        } // will be return
    case resource:
        if (!m_showResources) {
            return true;
        } else if (!m_showSingleResource
                   && item->parent()->childCount() < 2) {
            return true;
        } else {
            return false;
        } // will be return
    }
    return false;
}

bool RosterModel::hasVCard(const QString &bareJid) const
{
    return m_vCards.contains(bareJid);
}

QXmppVCard RosterModel::getVCard(const QString &bareJid) const
{
    if (hasVCard(bareJid))
        return m_vCards[bareJid];
    else
        return QXmppVCard();
}

void RosterModel::clear()
{
    m_vCards.clear();
    m_rootItem->clear();
    reset();
}

QList<QModelIndex> RosterModel::indexsForBareJid(const QString &bareJid)
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
