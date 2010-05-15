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

#include "PrefGeneral.h"
#include "ui_PrefGeneral.h"
#include <QDir>
#include <QDebug>

PrefGeneral::PrefGeneral(QWidget *parent) :
    PrefWidget(parent),
    ui(new Ui::PrefGeneral)
{
    ui->setupUi(this);
    connect(ui->iconSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(rosterIconSizeChanged(int)) );
    connect(ui->iconSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(iconSizeChanged()) );

    const QString trPath = QCoreApplication::applicationDirPath() + "/translations";
    const QStringList languageFiles = QDir(trPath).entryList(QStringList("qtalk*.qm"));

    ui->languageComboBox->addItem(QString("en"), QString("en"));
    foreach (const QString languageFile, languageFiles) {
        int start = languageFile.indexOf("_") + 1;
        int end = languageFile.lastIndexOf('.');
        const QString language = languageFile.mid(start, end - start);
        ui->languageComboBox->addItem(language, language);
    }
}

PrefGeneral::~PrefGeneral()
{
    delete ui;
}

QIcon PrefGeneral::sectionIcon() const
{
    return QIcon(":/images/preferences-other.png");
}

void PrefGeneral::changeEvent(QEvent *e)
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

QString PrefGeneral::sectionName() const
{
    return tr("General");
}

void PrefGeneral::readData(Preferences *pref)
{
    QString language;
    if (pref->language.isEmpty()) {
        language = "en";
    } else {
        language = pref->language;
    }

    int index = ui->languageComboBox->findData(language);
    if (index != -1) {
        ui->languageComboBox->setCurrentIndex(index);
    } else {
        ui->languageComboBox->setCurrentIndex(0);
    }

    m_languageChange = false;
    m_hideOfflineChange = false;
    m_showResourcesChange = false;
    m_showSingleResourceChange = false;
    m_rosterIconSizeChange = false;
    ui->hideOfflineCheckBox->setChecked(pref->hideOffline);
    ui->showResourcesCheckBox->setChecked(pref->showResources);
    ui->iconSizeSpinBox->setValue(pref->rosterIconSize);
    ui->showSingleResourceCheckBox->setEnabled(pref->showResources);
    ui->showSingleResourceCheckBox->setChecked(pref->showSingleResource);
    ui->closeToTrayCheckBox->setChecked(pref->closeToTray);
    ui->closeToTrayNoticeCheckBox->setChecked(pref->closeToTrayNotice);
}

void PrefGeneral::writeData(Preferences *pref)
{
    QString language = ui->languageComboBox->itemData(ui->languageComboBox->currentIndex()).toString();
    if (pref->language != language) {
        m_languageChange = true;
        pref->language = language;
    }

    if (pref->hideOffline != ui->hideOfflineCheckBox->isChecked()) {
        m_hideOfflineChange = true;
        pref->hideOffline = ui->hideOfflineCheckBox->isChecked();
    }

    if (pref->showResources != ui->showResourcesCheckBox->isChecked()) {
        m_showResourcesChange = true;
        pref->showResources = ui->showResourcesCheckBox->isChecked();
    }

    if (pref->showSingleResource != ui->showSingleResourceCheckBox->isChecked()) {
        m_showSingleResourceChange = true;
        pref->showSingleResource= ui->showSingleResourceCheckBox->isChecked();
    }

    pref->rosterIconSize = ui->iconSizeSpinBox->value();
    pref->closeToTray = ui->closeToTrayCheckBox->isChecked();
    pref->closeToTrayNotice = ui->closeToTrayNoticeCheckBox->isChecked();
}

bool PrefGeneral::isLanguageChanged() const
{
    return m_languageChange;
}

bool PrefGeneral::isRosterPrefChanged() const
{
    return m_hideOfflineChange
            || m_showResourcesChange
            || m_showSingleResourceChange;
}

bool PrefGeneral::isRosterIconSizeChanged() const
{
    return m_rosterIconSizeChange;
}

void PrefGeneral::iconSizeChanged()
{
    m_rosterIconSizeChange = true;
}
