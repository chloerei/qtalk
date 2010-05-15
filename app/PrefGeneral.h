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

#ifndef PREFGENERAL_H
#define PREFGENERAL_H

#include <QWidget>
#include <PrefWidget.h>

namespace Ui {
    class PrefGeneral;
}

class PrefGeneral : public PrefWidget
{
    Q_OBJECT
public:
    explicit PrefGeneral(QWidget *parent = 0);
    ~PrefGeneral();
    QIcon sectionIcon() const;
    QString sectionName() const;
    void readData(Preferences *pref);
    void writeData(Preferences *pref);
    bool isLanguageChanged() const;
    bool isRosterPrefChanged() const;
    bool isRosterIconSizeChanged() const;

signals:
    void rosterIconSizeChanged(int);

private slots:
    void iconSizeChanged();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrefGeneral *ui;
    bool m_languageChange;
    bool m_hideOfflineChange;
    bool m_showResourcesChange;
    bool m_showSingleResourceChange;
    bool m_rosterIconSizeChange;
};

#endif // PREFGENERAL_H
