#ifndef MESSAGEEDIT_H
#define MESSAGEEDIT_H

#include <QTextEdit>

class MessageEdit : public QTextEdit
{
Q_OBJECT
public:
    explicit MessageEdit(QWidget *parent = 0);
    void setIgnoreEnter(bool ignore);

protected:
    bool event(QEvent *event);

private:
    bool m_ignoreEnter;

};

#endif // MESSAGEEDIT_H
