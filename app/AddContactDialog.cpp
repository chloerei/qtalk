#include "AddContactDialog.h"
#include "ui_AddContactDialog.h"
#include "QPushButton"

AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->jid, SIGNAL(textChanged(QString)),
            this, SLOT(enableOkButton(QString)) );
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(enableGroupLineEdit(int)) );
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

void AddContactDialog::setGroups(QSet<QString> groups)
{
    ui->comboBox->clear();
    ui->comboBox->addItem(tr("None"));
    ui->comboBox->addItem(tr("New Group"));
    foreach (QString group, groups) {
        ui->comboBox->addItem(group);
    }
}

QString AddContactDialog::jid() const
{
    return ui->jid->text();
}

QString AddContactDialog::name() const
{
    return ui->name->text();
}

QString AddContactDialog::group() const
{
    if (ui->comboBox->currentIndex() == 1) {
        // custom
        return ui->group->text();
    } else if (ui->comboBox->currentIndex() == 0) {
        // none
        return QString();
    } else {
        return ui->comboBox->currentText();
    }
}

void AddContactDialog::enableOkButton(const QString &str)
{
    if (str.isEmpty())
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void AddContactDialog::enableGroupLineEdit(int index)
{
    // custom
    if (index == 1) {
        ui->group->setEnabled(true);
    } else {
        ui->group->clear();
        ui->group->setEnabled(false);
    }
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
