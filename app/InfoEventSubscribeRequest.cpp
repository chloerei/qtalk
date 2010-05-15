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

#include "InfoEventSubscribeRequest.h"
#include "ui_InfoEventSubscribeRequest.h"
#include <QXmppClient.h>
#include <QXmppPresence.h>

InfoEventSubscribeRequest::InfoEventSubscribeRequest(QString bareJid, QXmppClient *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoEventSubscribeRequest),
    m_client(client)
{
    ui->setupUi(this);
    ui->bareJid->setText(bareJid);
    connect(ui->denyButton, SIGNAL(clicked()),
            this, SLOT(denySlot()) );
    connect(ui->acceptButton, SIGNAL(clicked()),
            this, SLOT(acceptSlot()) );
}

InfoEventSubscribeRequest::~InfoEventSubscribeRequest()
{
    delete ui;
}

void InfoEventSubscribeRequest::acceptSlot()
{
    QXmppPresence presence;
    presence.setTo(ui->bareJid->text());
    presence.setType(QXmppPresence::Subscribed);
    m_client->sendPacket(presence);
    emit needDestory();
}

void InfoEventSubscribeRequest::denySlot()
{
    QXmppPresence presence;
    presence.setTo(ui->bareJid->text());
    presence.setType(QXmppPresence::Unsubscribed);
    m_client->sendPacket(presence);
    emit needDestory();
}

void InfoEventSubscribeRequest::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
