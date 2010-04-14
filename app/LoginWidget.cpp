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
    if (pref->storePassword)
        ui->passwordLineEdit->setText(pref->password);
    else
        ui->passwordLineEdit->clear();
    ui->hostLineEdit->setText(pref->host);
    ui->portSpinBox->setValue(pref->port);
    ui->storePasswordCheckBox->setChecked(pref->storePassword);
    ui->autoLoginCheckBox->setChecked(pref->autoLogin);
}

void LoginWidget::writeData(Preferences *pref)
{
    pref->jid = ui->jidLineEdit->text();
    if (ui->storePasswordCheckBox->isChecked())
        pref->password = ui->passwordLineEdit->text();
    else
        pref->password.clear();
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

QString LoginWidget::jid() const
{
    return ui->jidLineEdit->text();
}

QString LoginWidget::password() const
{
    return ui->passwordLineEdit->text();
}

QString LoginWidget::host() const
{
    return ui->hostLineEdit->text();
}

int LoginWidget::port() const
{
    return ui->portSpinBox->value();
}

bool LoginWidget::isStorePassword() const
{
    return ui->storePasswordCheckBox->isChecked();
}

bool LoginWidget::isAutoLogin() const
{
    return ui->autoLoginCheckBox->isChecked();
}

void LoginWidget::setJid(QString jid)
{
    ui->jidLineEdit->setText(jid);
}

void LoginWidget::setPassword(QString password)
{
    ui->passwordLineEdit->setText(password);
}

void LoginWidget::setHost(QString host)
{
    ui->hostLineEdit->setText(host);
}

void LoginWidget::setPort(int port)
{
    ui->portSpinBox->setValue(port);
}

void LoginWidget::setStorePassword(bool b)
{
    ui->storePasswordCheckBox->setChecked(b);
}

void LoginWidget::setAutoLogin(bool b)
{
    ui->autoLoginCheckBox->setChecked(b);
}

void LoginWidget::lock()
{
    ui->tabWidget->setEnabled(false);
}

void LoginWidget::unlock()
{
    ui->tabWidget->setEnabled(true);
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
        const int pos = jid().indexOf(QChar('@'));
        if (pos < 0)
            return;
        ui->hostLineEdit->setText(jid().mid(pos + 1));
    }
}
