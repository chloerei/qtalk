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
