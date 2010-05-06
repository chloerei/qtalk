#include "InfoEventSubscribeRequest.h"
#include "ui_InfoEventSubscribeRequest.h"
#include <QXmppClient.h>

InfoEventSubscribeRequest::InfoEventSubscribeRequest(QString bareJid, QXmppClient *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoEventSubscribeRequest),
    m_client(client)
{
    ui->setupUi(this);
    ui->bareJid->setText(bareJid);
    connect(ui->denyButton, SIGNAL(clicked()),
            this, SLOT(denySlot()) );
    connect(ui->acceptButton, SIGNAL(clicked()),
            this, SLOT(acceptSlot()) );
}

InfoEventSubscribeRequest::~InfoEventSubscribeRequest()
{
    delete ui;
}

void InfoEventSubscribeRequest::acceptSlot()
{
    emit needDestory();
}

void InfoEventSubscribeRequest::denySlot()
{
    emit needDestory();
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
