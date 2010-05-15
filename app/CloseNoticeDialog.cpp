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

#include "CloseNoticeDialog.h"
#include "ui_CloseNoticeDialog.h"

CloseNoticeDialog::CloseNoticeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloseNoticeDialog)
{
    ui->setupUi(this);
}

CloseNoticeDialog::~CloseNoticeDialog()
{
    delete ui;
}

void CloseNoticeDialog::changeEvent(QEvent *e)
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

void CloseNoticeDialog::readData(Preferences *pref)
{
    ui->closeToTrayCheckBox->setChecked(pref->closeToTray);
    ui->closeToTrayNoticeCheckBox->setChecked(pref->closeToTrayNotice);
}


void CloseNoticeDialog::writeDate(Preferences *pref)
{
    pref->closeToTray = ui->closeToTrayCheckBox->isChecked();
    pref->closeToTrayNotice = ui->closeToTrayNoticeCheckBox->isChecked();
}
