#ifndef TRANSFERMANAGERWINDOW_H
#define TRANSFERMANAGERWINDOW_H

#include <QMainWindow>
#include <QXmppTransferManager.h>

class TransferManagerModel;
class QFile;

namespace Ui {
    class TransferManagerWindow;
}

class TransferManagerWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit TransferManagerWindow(QXmppTransferManager *transferManager, QWidget *parent = 0);
    ~TransferManagerWindow();
    void createTransferJob(const QString &jid, const QString &fileName);
    void receivedTransferJob(QXmppTransferJob *job);

public slots:
    void deleteFileHandel(QXmppTransferJob *job);

private slots:
    void stopTransferJob();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TransferManagerWindow *ui;
    QXmppTransferManager *m_transferManager;
    TransferManagerModel *m_transferManagerModel;
    QMap<QString, QFile *> m_files;
};

#endif // TRANSFERMANAGERWINDOW_H
