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

#ifndef ADDCONTACTDIALOG_H
#define ADDCONTACTDIALOG_H

#include <QDialog>
#include <QXmppRosterIq.h>
#include <QSet>

namespace Ui {
    class AddContactDialog;
}

class AddContactDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddContactDialog(QWidget *parent = 0);
    ~AddContactDialog();
    void setGroups(QSet<QString> groups);
    QString jid() const;
    QString name() const;
    QString group() const;

private slots:
    void enableOkButton(const QString &str);
    void enableGroupLineEdit(int index);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AddContactDialog *ui;
};

#endif // ADDCONTACTDIALOG_H
