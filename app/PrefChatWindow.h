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

#ifndef PREFCHATWINDOW_H
#define PREFCHATWINDOW_H

#include <QWidget>
#include <PrefWidget.h>

namespace Ui {
    class PrefChatWindow;
}

class PrefChatWindow : public PrefWidget
{
    Q_OBJECT
public:
    explicit PrefChatWindow(QWidget *parent = 0);
    ~PrefChatWindow();
    QIcon sectionIcon() const;
    QString sectionName() const;
    void readData(Preferences *pref);
    void writeData(Preferences *pref);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrefChatWindow *ui;
};

#endif // PREFCHATWINDOW_H
