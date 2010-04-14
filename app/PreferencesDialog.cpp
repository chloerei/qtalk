#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "PrefAccount.h"
#include <QVBoxLayout>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    m_prefAccount(new PrefAccount(this))
{
    ui->setupUi(this);
    m_prefAccount->hide();

    connect(m_prefAccount, SIGNAL(settingChanged()),
            this, SIGNAL(accountSettingChanged()));
    connect(ui->configList, SIGNAL(currentRowChanged(int)),
            this, SLOT(changeConfig(int)));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton *)),
            this, SLOT(buttonBoxClicked(QAbstractButton*)));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_prefAccount);
    ui->configWidget->setLayout(layout);
    m_prefAccount->show();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::readSetting()
{
    m_prefAccount->readSetting();
}

void PreferencesDialog::writeSetting()
{
    m_prefAccount->writeSetting();
}

void PreferencesDialog::buttonBoxClicked(QAbstractButton *button)
{
    switch (ui->buttonBox->buttonRole(button)) {
    case QDialogButtonBox::AcceptRole:
    case QDialogButtonBox::ApplyRole :
        writeSetting();
        break;
    default:
        break;
    }
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
