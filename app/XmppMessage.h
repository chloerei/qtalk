#ifndef XMPPCLIENT_H
#define XMPPCLIENT_H value

#include "QXmppMessage.h"

class XmppMessage : public QXmppMessage
{
public:
    XmppMessage(const QString& from = "", const QString& to = "",
                const QString& body = "", const QString& thread = "");
    XmppMessage(const QXmppMessage &other);
    void setHtml(const QString &html);
    QString html() const;
};
#endif
