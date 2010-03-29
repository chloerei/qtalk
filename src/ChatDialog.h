#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "ui_ChatDialog.h"
#include <QWidget>

class ChatDialog : public QDialog
{
    Q_OBJECT
public:
    ChatDialog(QWidget *parent = 0);
private:
    Ui::ChatDialog ui;
};
#endif
