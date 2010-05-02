#include "AddContactDialog.h"
#include "ui_AddContactDialog.h"
#include "QPushButton"

AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableOkButton(QString)) );
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

QString AddContactDialog::jid() const
{
    return ui->lineEdit->text();
}

QXmppRosterIq::Item::SubscriptionType AddContactDialog::subscriptionType() const
{
    switch (ui->comboBox->currentIndex()) {
    case 0:
        return QXmppRosterIq::Item::Both;
    case 1:
        return QXmppRosterIq::Item::To;
    case 2:
        return QXmppRosterIq::Item::From;
    case 3:
        return QXmppRosterIq::Item::None;
    default:
        return QXmppRosterIq::Item::None;
    }
}

void AddContactDialog::enableOkButton(const QString &str)
{
    if (str.isEmpty())
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void AddContactDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
