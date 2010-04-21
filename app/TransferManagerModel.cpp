#include "TransferManagerModel.h"

TransferManagerModel::TransferManagerModel(QObject *parent) :
    QAbstractTableModel(parent)
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
            case Direction:
                return QString(tr("Direction"));
            case Jid:
                return QString(tr("Jabber ID"));
            case FileName:
                return QString(tr("File Name"));
            case State:
                return QString(tr("State"));
            case Method:
                return QString(tr("Method"));
            case FileSize:
                return QString(tr("File Size"));
            case Process:
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
        case Direction:
            return job->direction();
        case Jid:
            return job->jid();
        case FileName:
            return job->fileName();
        case State:
            return job->state();
        case Method:
            return job->method();
        case FileSize:
            return job->fileSize();
        case Process:
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
