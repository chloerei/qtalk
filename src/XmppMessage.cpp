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
