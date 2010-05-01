#include "PrefChatWindow.h"
#include "ui_PrefChatWindow.h"

PrefChatWindow::PrefChatWindow(QWidget *parent) :
    PrefWidget(parent),
    ui(new Ui::PrefChatWindow)
{
    ui->setupUi(this);
}

PrefChatWindow::~PrefChatWindow()
{
    delete ui;
}

void PrefChatWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QIcon PrefChatWindow::sectionIcon() const
{
    return QIcon(":/images/preferences-system-windows.png");
}

QString PrefChatWindow::sectionName() const
{
    return "ChatWindow";
}

void PrefChatWindow::readData(Preferences *pref)
{
    if (pref->enterToSendMessage)
        ui->enterRadioButton->setChecked(true);
    else
        ui->ctrlEnterRadioButton->setChecked(true);
}

void PrefChatWindow::writeData(Preferences *pref)
{
    if (ui->enterRadioButton->isChecked())
        pref->enterToSendMessage = true;
    else
        pref->enterToSendMessage = false;
}
