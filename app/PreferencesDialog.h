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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <Preferences.h>

namespace Ui {
    class PreferencesDialog;
}

class PrefWidget;
class PrefAccount;
class PrefGeneral;
class PrefChatWindow;

class PreferencesDialog : public QDialog {
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();
    bool isLanguageChanged();
    bool isAccountChanged();
    bool isRosterPrefChanged();

public slots:
    void readData(Preferences *pref);
    void writeData(Preferences *pref);
    void prefRejected();

signals:
    void applied();
    void rosterIconSizeChanged(int);
    void rosterIconReseze();

protected:
    void changeEvent(QEvent *e);

private slots:

private:
    Ui::PreferencesDialog *ui;
    QList<PrefWidget *> m_sections;
    PrefAccount *m_prefAccount;
    PrefGeneral *m_prefGeneral;
    PrefChatWindow *m_prefChatWindow;

    void addSection(PrefWidget *widget);
    void retranslation();
};

#endif // PREFERENCESDIALOG_H
