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

#ifndef TRANSFERMANAGERMODEL_H
#define TRANSFERMANAGERMODEL_H

#include <QAbstractTableModel>
#include <QXmppTransferManager.h>

class TransferManagerModel : public QAbstractTableModel
{
Q_OBJECT
public:
    enum ColumnName
    {
        Direction = 0,
        Jid       = 1,
        FileName  = 2,
        Progress  = 3,
        FileSize  = 4,
        State     = 5,
        Method    = 6
    };
    explicit TransferManagerModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
    void addJobToList(QXmppTransferJob *job);
    void removeJobFromList(QXmppTransferJob *job);
    void stopJobAtRow(int row);
    void clearJob();
    void removeRow(int row, const QModelIndex &parent = QModelIndex());

private slots:
    void jobFinished();
    void jobProgress(qint64 done, qint64 total);
    void jobStateChanged(QXmppTransferJob::State state);

private:
    //QXmppTransferManager *m_transferManager;
    QList<QXmppTransferJob *> m_jobList;
    QList<qint64> m_doneSize;
};

#endif // TRANSFERMANAGERMODEL_H
