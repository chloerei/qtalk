#include "InfoEventStackWidget.h"
#include "ui_InfoEventStackWidget.h"
#include <QXmppClient.h>
#include <InfoEventSubscribeRequest.h>

InfoEventStackWidget::InfoEventStackWidget(QXmppClient *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoEventStackWidget),
    m_client(client)
{
    ui->setupUi(this);
    connect(ui->closeButton, SIGNAL(clicked()),
            this, SLOT(closeSlot()) );
    connect(ui->previousButton, SIGNAL(clicked()),
            this, SLOT(previousSlot()) );
    connect(ui->nextButton , SIGNAL(clicked()),
            this, SLOT(nextSlot()) );
}

InfoEventStackWidget::~InfoEventStackWidget()
{
    delete ui;
}

void InfoEventStackWidget::addSubscribeRequest(const QString &bareJid)
{
    InfoEventSubscribeRequest *widget = new InfoEventSubscribeRequest(bareJid);
    ui->stackedWidget->addWidget(widget);
    updatePage();
}

void InfoEventStackWidget::changeEvent(QEvent *e)
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

void InfoEventStackWidget::previousSlot()
{
    if (ui->stackedWidget->count() != 0) {
        if (ui->stackedWidget->currentIndex() != 0) {
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
            updatePage();
        }
    }
}

void InfoEventStackWidget::nextSlot()
{
    if (ui->stackedWidget->count() != 0) {
        if (ui->stackedWidget->currentIndex() != ui->stackedWidget->count() - 1) {
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() + 1);
            updatePage();
        }
    }
}

void InfoEventStackWidget::closeSlot()
{
    if (isVisible()) {
        setVisible(false);
    }
}

void InfoEventStackWidget::updatePage()
{
    ui->pageLabel->setText(QString("%1 / %2")
                           .arg(ui->stackedWidget->currentIndex() + 1)
                           .arg(ui->stackedWidget->count()));

    if (ui->stackedWidget->currentIndex() == 0)
        ui->previousButton->setEnabled(false);
    else if (!ui->previousButton->isEnabled())
        ui->previousButton->setEnabled(true);

    if (ui->stackedWidget->currentIndex() == ui->stackedWidget->count() - 1)
        ui->nextButton->setEnabled(false);
    else if (!ui->previousButton->isEnabled())
        ui->nextButton->setEnabled(true);
}
