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
    void showCustomContextMenu(const QPoint &position);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TransferManagerWindow *ui;
    QXmppTransferManager *m_transferManager;
    TransferManagerModel *m_transferManagerModel;
    QMap<QString, QFile *> m_files;
};

#endif // TRANSFERMANAGERWINDOW_H
