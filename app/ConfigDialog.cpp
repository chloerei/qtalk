#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"
#include "AccountConfigWidget.h"
#include <QVBoxLayout>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog),
    m_accountConfigWidget(new AccountConfigWidget(this))
{
    ui->setupUi(this);
    m_accountConfigWidget->hide();

    connect(m_accountConfigWidget, SIGNAL(settingChanged()),
            this, SIGNAL(accountSettingChanged()));
    connect(ui->configList, SIGNAL(currentRowChanged(int)),
            this, SLOT(changeConfig(int)));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton *)),
            this, SLOT(buttonBoxClicked(QAbstractButton*)));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_accountConfigWidget);
    ui->configWidget->setLayout(layout);
    m_accountConfigWidget->show();
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::readSetting()
{
    m_accountConfigWidget->readSetting();
}

void ConfigDialog::writeSetting()
{
    m_accountConfigWidget->writeSetting();
}

void ConfigDialog::buttonBoxClicked(QAbstractButton *button)
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

void ConfigDialog::changeEvent(QEvent *e)
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

void ConfigDialog::changeConfig(int row)
{
    if (ui->configList->currentRow() != row) {
        if (ui->configWidget->layout()->count() != 0 )
            ui->configWidget->layout()->removeItem(ui->configWidget->layout()->takeAt(0));
        switch (row) {
        case 0:
            ui->configWidget->layout()->addWidget(m_accountConfigWidget);
            break;
        default:
            break;
        }
    }
}
