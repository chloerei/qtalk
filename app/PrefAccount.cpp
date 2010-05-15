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

#include "PrefAccount.h"
#include "ui_PrefAccount.h"
#include <QSettings>

PrefAccount::PrefAccount(QWidget *parent) :
    PrefWidget(parent),
    ui(new Ui::PrefAccount)
{
    ui->setupUi(this);
}

PrefAccount::~PrefAccount()
{
    delete ui;
}

QIcon PrefAccount::sectionIcon() const
{
    return QIcon(":/images/preferences-desktop-user-password.png");
}

QString PrefAccount::sectionName() const
{
    return tr("Account");
}

void PrefAccount::changeEvent(QEvent *e)
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

void PrefAccount::readData(Preferences *pref)
{
    m_changed = false;
    ui->jidLineEdit->setText(pref->jid);
    ui->passwordLineEdit->setText(pref->password);
    ui->hostLineEdit->setText(pref->host);
    ui->portSpinBox->setValue(pref->port);
    ui->storePasswordCheckBox->setChecked(pref->storePassword);
    ui->autoLoginCheckBox->setChecked(pref->autoLogin);
}

void PrefAccount::writeData(Preferences *pref)
{
    if ( pref->jid != ui->jidLineEdit->text() ||
         pref->password != ui->passwordLineEdit->text() ||
         pref->host != ui->hostLineEdit->text() ||
         pref->port != ui->portSpinBox->value() ||
         pref->storePassword != ui->storePasswordCheckBox->isChecked() ||
         pref->autoLogin != ui->autoLoginCheckBox->isChecked() ) {
        m_changed = true;
        pref->jid = ui->jidLineEdit->text();
        pref->password = ui->passwordLineEdit->text();
        pref->host = ui->hostLineEdit->text();
        pref->port = ui->portSpinBox->value();
        pref->storePassword = ui->storePasswordCheckBox->isChecked();
        pref->autoLogin = ui->autoLoginCheckBox->isChecked();
    }
}

