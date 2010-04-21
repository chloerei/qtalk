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
    return 4;
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
            break;
        case 1:
            return job->jid();
            break;
        case 2:
            return job->fileName();
            break;
        case 3:
            return job->state();
            break;
        case 4:
            return job->method();
            break;
        case 5:
            return job->fileSize();
            break;
        case 6:
            return "precess";
            break;
        default:
            break;
        }
    }

    return QVariant();
}

void TransferManagerModel::addJobToList(QXmppTransferJob *job)
{
    m_jobList << job;
}

void TransferManagerModel::removeJobFromList(QXmppTransferJob *job)
{
    m_jobList.removeOne(job);
}
