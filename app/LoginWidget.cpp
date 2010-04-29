#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include <QSettings>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    QRegExp rx(".+@.+\\..+");
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->jidLineEdit->setValidator(validator);

    ui->portSpinBox->setValue(5222);
    connect(ui->loginButton, SIGNAL(clicked()),
            SLOT(clickedLogin()));
    connect(ui->jidLineEdit, SIGNAL(editingFinished()),
            this, SLOT(getHost()));

}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::readData(Preferences *pref)
{
    ui->jidLineEdit->setText(pref->jid);
    ui->passwordLineEdit->setText(pref->password);
    ui->hostLineEdit->setText(pref->host);
    ui->portSpinBox->setValue(pref->port);
    ui->storePasswordCheckBox->setChecked(pref->storePassword);
    ui->autoLoginCheckBox->setChecked(pref->autoLogin);
}

void LoginWidget::writeData(Preferences *pref)
{
    pref->jid = ui->jidLineEdit->text();
    pref->password = ui->passwordLineEdit->text();
    pref->host = ui->hostLineEdit->text();
    pref->port = ui->portSpinBox->value();
    pref->storePassword = ui->storePasswordCheckBox->isChecked();
    pref->autoLogin = ui->autoLoginCheckBox->isChecked();
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

void LoginWidget::lock()
{
    ui->tabWidget->setEnabled(false);
    ui->loginButton->setEnabled(false);
}

void LoginWidget::unlock()
{
    ui->tabWidget->setEnabled(true);
    ui->loginButton->setEnabled(true);
}

void LoginWidget::showState(QString str)
{
    ui->stateLabel->setText(str);;
}

void LoginWidget::clickedLogin()
{
    if (ui->jidLineEdit->hasAcceptableInput()) {
        emit login();
    } else {
        showState("<b>Notice !</b> Jabber ID is looks like 'username@domain.com'");
    }
}

void LoginWidget::getHost()
{
    if (!ui->advanceCheckBox->isChecked()) {
        QString jid = ui->jidLineEdit->text();
        const int pos = jid.indexOf(QChar('@'));
        if (pos < 0)
            return;
        QString host = jid.mid(pos+1);
        if (host == "gmail.com")
            host = "talk.google.com";
        ui->hostLineEdit->setText(host);
    }
}
