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

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "PrefWidget.h"
#include "PrefAccount.h"
#include "PrefGeneral.h"
#include "PrefChatWindow.h"
#include <QPushButton>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    m_prefAccount(new PrefAccount(this)),
    m_prefGeneral(new PrefGeneral(this)),
    m_prefChatWindow(new PrefChatWindow(this))
{
    ui->setupUi(this);
    m_prefAccount->hide();

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            this, SIGNAL(applied()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SIGNAL(applied()));
    connect(m_prefGeneral, SIGNAL(rosterIconSizeChanged(int)),
            this, SIGNAL(rosterIconSizeChanged(int)) );
    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(prefRejected()) );

    setAttribute(Qt::WA_QuitOnClose, false);
    addSection(m_prefAccount);
    addSection(m_prefGeneral);
    addSection(m_prefChatWindow);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

bool PreferencesDialog::isLanguageChanged()
{
    return m_prefGeneral->isLanguageChanged();
}

bool PreferencesDialog::isAccountChanged()
{
    return m_prefAccount->isChanged();
}

bool PreferencesDialog::isRosterPrefChanged()
{
    return m_prefGeneral->isRosterPrefChanged();
}

void PreferencesDialog::readData(Preferences *pref)
{
    foreach (PrefWidget *widget, m_sections) {
        widget->readData(pref);
    }
}

void PreferencesDialog::writeData(Preferences *pref)
{
    foreach (PrefWidget *widget, m_sections) {
        widget->writeData(pref);
    }
}

void PreferencesDialog::prefRejected()
{
    if (m_prefGeneral->isRosterIconSizeChanged()) {
        emit rosterIconReseze();
    }
}

void PreferencesDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        retranslation();
        break;
    default:
        break;
    }
}

void PreferencesDialog::addSection(PrefWidget *widget)
{
    ui->sections->addItem(new QListWidgetItem(widget->sectionIcon(), widget->sectionName()));
    ui->pages->addWidget(widget);
    m_sections << widget;
}

void PreferencesDialog::retranslation()
{
    for (int i = 0; i < ui->sections->count(); i++) {
        ui->sections->item(i)->setText(m_sections.at(i)->sectionName());
    }
}
