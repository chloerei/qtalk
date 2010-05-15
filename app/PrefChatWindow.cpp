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

#include "PrefChatWindow.h"
#include "ui_PrefChatWindow.h"

PrefChatWindow::PrefChatWindow(QWidget *parent) :
    PrefWidget(parent),
    ui(new Ui::PrefChatWindow)
{
    ui->setupUi(this);
}

PrefChatWindow::~PrefChatWindow()
{
    delete ui;
}

void PrefChatWindow::changeEvent(QEvent *e)
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

QIcon PrefChatWindow::sectionIcon() const
{
    return QIcon(":/images/preferences-system-windows.png");
}

QString PrefChatWindow::sectionName() const
{
    return tr("ChatWindow");
}

void PrefChatWindow::readData(Preferences *pref)
{
    if (pref->enterToSendMessage)
        ui->enterRadioButton->setChecked(true);
    else
        ui->ctrlEnterRadioButton->setChecked(true);
}

void PrefChatWindow::writeData(Preferences *pref)
{
    if (ui->enterRadioButton->isChecked())
        pref->enterToSendMessage = true;
    else
        pref->enterToSendMessage = false;
}
