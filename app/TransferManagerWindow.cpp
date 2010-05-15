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

#include "TransferManagerWindow.h"
#include "ui_TransferManagerWindow.h"
#include "TransferManagerModel.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QFile>
#include <QMenu>

TransferManagerWindow::TransferManagerWindow(QXmppTransferManager *transferManager, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TransferManagerWindow),
    m_transferManager(transferManager)
{
    ui->setupUi(this);
    m_transferManagerModel = new TransferManagerModel(this);
    ui->tableView->setModel(m_transferManagerModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);
    connect(ui->actionStopTransfer, SIGNAL(triggered()),
            this, SLOT(stopTransferJob()) );
    connect(ui->actionCleanList, SIGNAL(triggered()),
            m_transferManagerModel, SLOT(clearJob()) );

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)) );
    setAttribute(Qt::WA_QuitOnClose, false);
}

TransferManagerWindow::~TransferManagerWindow()
{
    delete ui;
}

void TransferManagerWindow::createTransferJob(const QString &jid, const QString &fileName)
{
    m_transferManagerModel->addJobToList(m_transferManager->sendFile(jid, fileName));
}

void TransferManagerWindow::receivedTransferJob(QXmppTransferJob *offer)
{
    if (QMessageBox::information(this,
                                 tr("File recevied"),
                                 QString(tr("Transfer offer from: %1\nFilename: %2 \nDo your want to recevie?"))
                                 .arg(offer->jid()).arg(offer->fileName()),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_transferManagerModel->addJobToList(offer);
        QString saveFileName =
                QFileDialog::getSaveFileName(this,
                                             tr("Save File"),
                                             QDesktopServices::storageLocation(QDesktopServices::DesktopLocation) + offer->fileName());
        QFile *file = new QFile(saveFileName);
        if (file->open(QIODevice::WriteOnly)) {
            offer->accept(file);
            m_files[offer->sid()] = file;
            show();
            raise();
            activateWindow();
        } else {
            QMessageBox::warning(this,
                                 tr("Writing Error"),
                                 tr("Can no writing file. Transfer abort"));
            offer->abort();
            delete file;
        }
    } else {
        offer->abort();
    }
}

void TransferManagerWindow::deleteFileHandel(QXmppTransferJob *job)
{
    if (m_files.contains(job->sid())) {
        qDebug("delete file handel");
        delete m_files.take(job->sid());
    }
}

void TransferManagerWindow::stopTransferJob()
{
    foreach(QModelIndex index, ui->tableView->selectionModel()->selectedRows()) {
        m_transferManagerModel->stopJobAtRow(index.row());
    }
}

void TransferManagerWindow::showCustomContextMenu(const QPoint &position)
{
    QList<QAction *> actions;
    QModelIndex index = ui->tableView->indexAt(position);
    if (index.isValid()) {
        actions << ui->actionStopTransfer;
        actions << ui->actionCleanList;
    }
    if (!actions.isEmpty())
        QMenu::exec(actions, ui->tableView->mapToGlobal(position));
}

void TransferManagerWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
