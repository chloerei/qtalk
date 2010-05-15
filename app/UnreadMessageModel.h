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

#ifndef UNREADMESSAGEMODEL
#define UNREADMESSAGEMODEL

#include <QAbstractListModel>
#include <QMap>
#include <QList>
#include "QXmppMessage.h"
#include <QModelIndex>
#include <QVariant>

class UnreadMessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    UnreadMessageModel(QObject *parent = 0);

    void add(const QXmppMessage &message);
    QList<QXmppMessage> take(QString jid);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool hasUnread(const QString &jid) const;
    bool hasAnyUnread() const;
    QString jidAt(const QModelIndex &index) const;
    QList<QString> bareJids() const;

signals:
    void messageCleared();

private:
    QMap<QString, QList<QXmppMessage> > m_messageStore;
};
#endif
