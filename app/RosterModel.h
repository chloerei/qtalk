/*
 * Copyright (C) 2010 Rei
 *
 * Author:
 *	Rei
 *
 * Source:
 *	http://github.com/chloerei/qtalk
 *
 * This file is a part of QTalk.
 *
 * QTalk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QTalk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QTalk.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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

    RosterModel(QXmppClient *client, QObject *parent = 0);
    ~RosterModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    ItemType itemTypeAt(const QModelIndex &index) const;
    QString jidAt(const QModelIndex &index) const;
    QString groupAt(const QModelIndex &index) const;
    void messageUnread(const QString &bareJid, const QString &resource);
    void messageReaded(const QString &bareJid, const QString &resource);
    void messageReadedAll(const QString &bareJid);
    void readPref(Preferences *pref);
    QList<QModelIndex> allIndex(const QModelIndex &index = QModelIndex()) const;
    bool isIndexHidden(const QModelIndex &index);
    bool hasVCard(const QString &bareJid) const;
    QXmppVCard getVCard(const QString &bareJid) const; // if no exist, return empty vcard
    void clear();
    QSet<QString> getGroups() const;

signals:
    void lastOneResource(const QModelIndex &contactIndex);
    void parseDone();
    void hiddenUpdate();

public slots:
    void parseRoster();

private slots:
    void presenceChangedSlot(const QString &bareJid, const QString &resource);
    void rosterChangedSlot(const QString &bareJid);
    void vCardRecived(const QXmppVCard&);

private:
    QXmppClient *m_client;
    QXmppRoster *m_roster;
    QXmppVCardManager *m_vCardManager;
    TreeItem* m_rootItem;
    TreeItem* m_noGroupItem;
    bool m_hideOffline;
    bool m_showResources;
    bool m_showSingleResource;

    void removeRow(int row, const QModelIndex &parent = QModelIndex());
    void initNoGroup();
    void setClient(QXmppClient *client);
    QModelIndex findOrCreateGroup(QString group);
    QModelIndex groupIndexFor(const QString &groupName) const;
    QModelIndex nogroupIndex() const;
    void insertRosterToGroup(QString bareJid, QString group);
    void removeRosterFromGroup(QString bareJid, QString group);
    bool hasGroup(const QString &groupName) const;
    void newContact(const QString &bareJid);
    void checkRosources(const QModelIndex &index);
    void parsePresence(const QModelIndex &contactIndex, const QString &resource, const QXmppPresence &presence);
    TreeItem* getItem(const QModelIndex &index) const;
    void sortContact(const QModelIndex &groupIndex);
    QList<QModelIndex> indexsForBareJid(const QString &bareJid); // include all resource
    QMap<QString, QXmppVCard> m_vCards; // <bareJid, vcard>

    // use for data display
    QString displayData(const QModelIndex &index) const;
    QString toolTipData(const QModelIndex &index) const;
    QString statusTextAt(const QModelIndex &index) const;
};

#endif
