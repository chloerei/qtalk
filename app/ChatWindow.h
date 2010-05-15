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

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "ui_ChatWindow.h"
#include "QXmppMessage.h"
#include "Preferences.h"
#include "QXmppVCard.h"
#include <QPointer>

class QXmppClient;
class QXmppMessage;
class QTimer;
class QStatusBar;
class QPushButton;
class MessageEdit;
class QXmppVCard;
class ContactInfoDialog;

class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    ChatWindow(QString jid, QXmppClient *client, QWidget *parent = 0);
    void appendMessage(const QXmppMessage &);
    void readPref(Preferences *pref);
    void setVCard(QXmppVCard vCard);

signals:
    void sendFile(QString jid, QString fileName);
    void viewContactInfo(const QString &jid);

private slots:
    void sendMessage();
    void sendComposing();
    void pausedTimeout();
    void inactiveTimeout();
    void goneTimeout();
    void openContactInfoDialog();
    void sendFileSlot();

protected:
    void closeEvent(QCloseEvent *);
    void changeEvent(QEvent *e);

private:
    Ui::ChatWindow ui;
    QString m_jid;
    QXmppClient *m_client;
    QXmppMessage::State m_selfState; // self state, se for send state message
    QTimer *m_pausedTimer;
    QTimer *m_inactiveTimer;
    QTimer *m_goneTimer;
    MessageEdit *m_editor;
    QStatusBar *m_statusBar;
    QPushButton *m_sendButton;
    QLabel *m_sendTip;
    QXmppVCard m_vCard;
    QPointer<ContactInfoDialog> m_contactInfoDialog;

    void changeState(QXmppMessage::State);
    void changeSelfState(QXmppMessage::State);
};
#endif
