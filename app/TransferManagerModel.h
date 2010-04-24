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
