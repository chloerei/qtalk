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

#include "XmppMessage.h"
#include <QDomDocument>
#include <QXmlStreamWriter>

XmppMessage::XmppMessage(const QString& from, const QString& to, const 
                         QString& body, const QString& thread)
    : QXmppMessage(from, to, body, thread)
{
}

XmppMessage::XmppMessage(const QXmppMessage &other)
    : QXmppMessage(other)
{
}

void XmppMessage::setHtml(const QString &html)
{
    QDomDocument doc;
    doc.setContent(html);
    QXmppElement element(doc.documentElement());
    QXmppElementList elementList = extensions();
    elementList << element;
    setExtensions(elementList);
}

QString XmppMessage::html() const
{
    foreach (QXmppElement element, extensions()) {
        if (element.tagName() == "html") {
            QString html;
            QXmlStreamWriter writer(&html);
            element.toXml(&writer);
            return html;
        }
    }
    return "";
}
