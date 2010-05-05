#include "InfoEventSubscribeRequest.h"
#include "ui_InfoEventSubscribeRequest.h"

InfoEventSubscribeRequest::InfoEventSubscribeRequest(QString bareJid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoEventSubscribeRequest)
{
    ui->setupUi(this);
    ui->bareJid->setText(bareJid);
}

InfoEventSubscribeRequest::~InfoEventSubscribeRequest()
{
    delete ui;
}

void InfoEventSubscribeRequest::acceptSlot()
{
    emit accept(ui->bareJid->text());
}

void InfoEventSubscribeRequest::denySlot()
{
    emit deny(ui->bareJid->text());
}

void InfoEventSubscribeRequest::changeEvent(QEvent *e)
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
