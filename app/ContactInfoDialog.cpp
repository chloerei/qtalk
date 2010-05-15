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

#include "ContactInfoDialog.h"
#include "ui_ContactInfoDialog.h"

ContactInfoDialog::ContactInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactInfoDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

ContactInfoDialog::~ContactInfoDialog()
{
    delete ui;
}

void ContactInfoDialog::setData(QString name, QString jid, const QXmppVCard &vCard)
{
    ui->name->setText(name);
    ui->jid->setText(jid);
    if (vCard.photo().isEmpty())
        ui->photo->setPixmap(QPixmap(":/images/user-identity-100.png"));
    else
        ui->photo->setPixmap(QPixmap::fromImage(vCard.photoAsImage()));
    ui->nickName->setText(vCard.nickName());
    ui->fullName->setText(vCard.fullName());
    ui->firstName->setText(vCard.firstName());
    ui->midName->setText(vCard.middleName());
    ui->lastName->setText(vCard.lastName());
    ui->url->setText(vCard.url());
}

void ContactInfoDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
