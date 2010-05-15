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

#include "MessageEdit.h"
#include <QKeyEvent>

MessageEdit::MessageEdit(QWidget *parent) :
    QTextEdit(parent),
    m_ignoreEnter(false)
{
}

void MessageEdit::setIgnoreEnter(bool ignore)
{
    m_ignoreEnter = ignore;
}

bool MessageEdit::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride) {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return) {
            if (m_ignoreEnter && !(keyEvent->modifiers() & Qt::ShiftModifier)) {
                return false;
            }
        }
    }
    return QTextEdit::event(event);
}
