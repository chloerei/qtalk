#include "TransferManagerModel.h"

TransferManagerModel::TransferManagerModel(QXmppTransferManager *transferManager, QObject *parent) :
    QAbstractTableModel(parent),
    m_transferManager(transferManager)
{
}

int TransferManagerModel::rowCount(const QModelIndex &/* parent */) const
{
    return m_jobList.count();
}

int TransferManagerModel::columnCount(const QModelIndex &/* parent */) const
{
    return 7;
}

QVariant TransferManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return QString(tr("Direction"));
            case 1:
                return QString(tr("Jabber ID"));
            case 2:
                return QString(tr("File Name"));
            case 3:
                return QString(tr("State"));
            case 4:
                return QString(tr("Method"));
            case 5:
                return QString(tr("File Size"));
            case 6:
                return QString(tr("Process"));
            default:
                break;
            }
        } else {
            return  section;
        }
    }
    return QVariant();
}

QVariant TransferManagerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (m_jobList.count() == 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        QXmppTransferJob *job = m_jobList[index.row()];
        switch (index.column()) {
        case 0:
            return job->direction();
        case 1:
            return job->jid();
        case 2:
            return job->fileName();
        case 3:
            return job->state();
        case 4:
            return job->method();
        case 5:
            return job->fileSize();
        case 6:
            return "precess";
        default:
            break;
        }
    }

    return QVariant();
}

void TransferManagerModel::addJobToList(QXmppTransferJob *job)
{
    beginInsertRows(QModelIndex(), m_jobList.count(), m_jobList.count());
    m_jobList << job;
    endInsertRows();
}

void TransferManagerModel::removeJobFromList(QXmppTransferJob *job)
{
    int row = m_jobList.indexOf(job);
    beginRemoveRows(QModelIndex(), row, row);
    m_jobList.removeOne(job);
    endRemoveColumns();
}
