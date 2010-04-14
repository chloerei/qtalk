#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "PrefAccount.h"
#include <QVBoxLayout>
#include <QPushButton>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    m_prefAccount(new PrefAccount(this))
{
    ui->setupUi(this);
    m_prefAccount->hide();

    connect(ui->configList, SIGNAL(currentRowChanged(int)),
            this, SLOT(changeConfig(int)));

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            this, SIGNAL(applied()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SIGNAL(applied()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_prefAccount);
    ui->configWidget->setLayout(layout);
    m_prefAccount->show();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

bool PreferencesDialog::isAccountChanged()
{
    return m_prefAccount->isChanged();
}

void PreferencesDialog::readData(Preferences *pref)
{
    m_prefAccount->readData(pref);
}

void PreferencesDialog::writeData(Preferences *pref)
{
    m_prefAccount->writeData(pref);
}

void PreferencesDialog::changeEvent(QEvent *e)
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

void PreferencesDialog::changeConfig(int row)
{
    if (ui->configList->currentRow() != row) {
        if (ui->configWidget->layout()->count() != 0 )
            ui->configWidget->layout()->removeItem(ui->configWidget->layout()->takeAt(0));
        switch (row) {
        case 0:
            ui->configWidget->layout()->addWidget(m_prefAccount);
            break;
        default:
            break;
        }
    }
}
