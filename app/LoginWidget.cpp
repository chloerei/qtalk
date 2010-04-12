#include "LoginWidget.h"
#include "ui_LoginWidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    connect(ui->loginButton, SIGNAL(clicked()),
            SIGNAL(login()));
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::changeEvent(QEvent *e)
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

QString LoginWidget::jid() const
{
    return ui->jidLineEdit->text();
}

QString LoginWidget::password() const
{
    return ui->passwordLabel->text();
}

bool LoginWidget::isRemember() const
{
    return ui->rememberCheckBox->isChecked();
}

void LoginWidget::lock()
{
    ui->jidLineEdit->setEnabled(false);
    ui->passwordLineEdit->setEnabled(false);
    ui->rememberCheckBox->setEnabled(false);
    ui->loginButton->setEnabled(false);
}
