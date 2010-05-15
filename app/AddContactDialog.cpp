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

#include "AddContactDialog.h"
#include "ui_AddContactDialog.h"
#include "QPushButton"

AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->jid, SIGNAL(textChanged(QString)),
            this, SLOT(enableOkButton(QString)) );
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(enableGroupLineEdit(int)) );
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

void AddContactDialog::setGroups(QSet<QString> groups)
{
    ui->comboBox->clear();
    ui->comboBox->addItem(tr("None"));
    ui->comboBox->addItem(tr("New Group"));
    foreach (QString group, groups) {
        ui->comboBox->addItem(group);
    }
}

QString AddContactDialog::jid() const
{
    return ui->jid->text();
}

QString AddContactDialog::name() const
{
    return ui->name->text();
}

QString AddContactDialog::group() const
{
    if (ui->comboBox->currentIndex() == 1) {
        // custom
        return ui->group->text();
    } else if (ui->comboBox->currentIndex() == 0) {
        // none
        return QString();
    } else {
        return ui->comboBox->currentText();
    }
}

void AddContactDialog::enableOkButton(const QString &str)
{
    if (str.isEmpty())
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void AddContactDialog::enableGroupLineEdit(int index)
{
    // custom
    if (index == 1) {
        ui->group->setEnabled(true);
    } else {
        ui->group->clear();
        ui->group->setEnabled(false);
    }
}

void AddContactDialog::changeEvent(QEvent *e)
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
