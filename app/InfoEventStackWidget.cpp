#include "InfoEventStackWidget.h"
#include "ui_InfoEventStackWidget.h"

InfoEventStackWidget::InfoEventStackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoEventStackWidget)
{
    ui->setupUi(this);
    connect(ui->closeButton, SIGNAL(clicked()),
            this, SLOT(closeSlot()) );
}

InfoEventStackWidget::~InfoEventStackWidget()
{
    delete ui;
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

void InfoEventStackWidget::closeSlot()
{
    if (ui->stackedWidget->count() == 0) {
        setVisible(false);
    }
}
