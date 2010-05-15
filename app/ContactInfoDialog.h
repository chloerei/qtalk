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

#ifndef CONTACTINFODIALOG_H
#define CONTACTINFODIALOG_H

#include <QDialog>
#include <QXmppVCard.h>

namespace Ui {
    class ContactInfoDialog;
}

class ContactInfoDialog : public QDialog {
    Q_OBJECT
public:
    explicit ContactInfoDialog(QWidget *parent = 0);
    ~ContactInfoDialog();
    void setData(QString name, QString jid, const QXmppVCard &vCard);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ContactInfoDialog *ui;
};

#endif // CONTACTINFODIALOG_H
