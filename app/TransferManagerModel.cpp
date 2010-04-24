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
            case Progress:
                return QString(tr("Process"));
            default:
                break;
            }
        } else {
            return  QVariant();
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
            switch (job->direction()) {
            case QXmppTransferJob::IncomingDirection:
                return QString(tr("Incoming"));
            case QXmppTransferJob::OutgoingDirection:
                return QString(tr("Outgoing"));
            }
        case Jid:
            return job->jid();
        case FileName:
            return job->fileName();
        case State:
            switch (job->state()) {
            case QXmppTransferJob::OfferState:
                return QString(tr("Offer"));
            case QXmppTransferJob::StartState:
                return QString(tr("Start"));
            case QXmppTransferJob::TransferState:
                return QString(tr("Transfer"));
            case QXmppTransferJob::FinishedState:
                return QString(tr("Finished"));
            }
        case Method:
            switch (job->method()) {
            case QXmppTransferJob::NoMethod:
                return QString(tr("No Method"));
            case QXmppTransferJob::InBandMethod:
                return QString(tr("Ibb"));
            case QXmppTransferJob::SocksMethod:
                return QString(tr("Socks"));
            case QXmppTransferJob::AnyMethod:
                return QString(tr("Any"));
            }
        case FileSize:
            return job->fileSize();
        case Progress:
            return QString(" %1 / %2 ").arg(m_doneSize[index.row()]).arg(job->fileSize());
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
    m_doneSize << 0;
    connect(job, SIGNAL(finished()),
            this, SLOT(jobFinished()) );
    connect(job, SIGNAL(progress(qint64,qint64)),
            this, SLOT(jobProgress(qint64,qint64)) );
    connect(job, SIGNAL(stateChanged(QXmppTransferJob::State)),
            this, SLOT(jobStateChanged(QXmppTransferJob::State)) );
    endInsertRows();
}

void TransferManagerModel::removeJobFromList(QXmppTransferJob *job)
{
    int row = m_jobList.indexOf(job);
    beginRemoveRows(QModelIndex(), row, row);
    m_jobList.removeAt(row);
    m_doneSize.removeAt(row);
    endRemoveColumns();
}

void TransferManagerModel::stopJobAtRow(int row)
{
    m_jobList[row]->abort();
}

void TransferManagerModel::clearJob()
{
    QList<QXmppTransferJob *> unfinished;
    foreach (QXmppTransferJob *job, m_jobList) {
        if (job->state() != QXmppTransferJob::FinishedState)
            unfinished << job;
    }
    m_jobList = unfinished;
    reset();
}

void TransferManagerModel::removeRow(int row, const QModelIndex &parent)
{
    if (parent != QModelIndex())
        return;
    beginRemoveRows(QModelIndex(), row, row);
    m_jobList.removeAt(row);
    endRemoveRows();
}

void TransferManagerModel::jobFinished()
{
    int row = m_jobList.indexOf(qobject_cast<QXmppTransferJob *>(sender()));
    dataChanged(index(row, 0), index(row, 6));
}

void TransferManagerModel::jobProgress(qint64 done, qint64 /* total */)
{
    qDebug() << "progress" << done;
    int row = m_jobList.indexOf(qobject_cast<QXmppTransferJob *>(sender()));
    m_doneSize[row] = done;
    dataChanged(index(row,Progress), index(row, Progress));
}

void TransferManagerModel::jobStateChanged(QXmppTransferJob::State /* state */)
{
    int row = m_jobList.indexOf(qobject_cast<QXmppTransferJob *>(sender()));
    dataChanged(index(row, State), index(row, State));
}
