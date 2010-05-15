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

#include "UnreadMessageWindow.h"
#include "QXmppMessage.h"
#include <UnreadMessageModel.h>

UnreadMessageWindow::UnreadMessageWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.listView, SIGNAL(clicked(const QModelIndex&)),
            this, SIGNAL(unreadListClicked(const QModelIndex&)));
    connect(ui.readAllButton, SIGNAL(clicked()),
            this, SIGNAL(readAll()));

}

void UnreadMessageWindow::setModel(QAbstractListModel *model)
{
    ui.listView->setModel(model);
}

void UnreadMessageWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}
