#include "PrefAccount.h"
#include "ui_PrefAccount.h"
#include <QSettings>

PrefAccount::PrefAccount(QWidget *parent) :
    PrefWidget(parent),
    ui(new Ui::PrefAccount)
{
    ui->setupUi(this);
}

PrefAccount::~PrefAccount()
{
    delete ui;
}

QIcon PrefAccount::sectionIcon() const
{
    return QIcon(":/images/preferences-desktop-user-password.png");
}

QString PrefAccount::sectionName() const
{
    return "Account";
}

void PrefAccount::changeEvent(QEvent *e)
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

void PrefAccount::readData(Preferences *pref)
{
    m_changed = false;
    ui->jidLineEdit->setText(pref->jid);
    ui->passwordLineEdit->setText(pref->password);
    ui->hostLineEdit->setText(pref->host);
    ui->portSpinBox->setValue(pref->port);
    ui->storePasswordCheckBox->setChecked(pref->storePassword);
    ui->autoLoginCheckBox->setChecked(pref->autoLogin);
}

void PrefAccount::writeData(Preferences *pref)
{
    if ( pref->jid != ui->jidLineEdit->text() ||
         pref->password != ui->passwordLineEdit->text() ||
         pref->host != ui->hostLineEdit->text() ||
         pref->port != ui->portSpinBox->value() ||
         pref->storePassword != ui->storePasswordCheckBox->isChecked() ||
         pref->autoLogin != ui->autoLoginCheckBox->isChecked() ) {
        m_changed = true;
        pref->jid = ui->jidLineEdit->text();
        pref->password = ui->passwordLineEdit->text();
        pref->host = ui->hostLineEdit->text();
        pref->port = ui->portSpinBox->value();
        pref->storePassword = ui->storePasswordCheckBox->isChecked();
        pref->autoLogin = ui->autoLoginCheckBox->isChecked();
    }
}

