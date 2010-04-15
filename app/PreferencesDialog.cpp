#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "PrefAccount.h"
#include "PrefWidget.h"
#include <QPushButton>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    m_prefAccount(new PrefAccount(this))
{
    ui->setupUi(this);
    m_prefAccount->hide();

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            this, SIGNAL(applied()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SIGNAL(applied()));

    addSection(m_prefAccount);
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

void PreferencesDialog::addSection(PrefWidget *widget)
{
    ui->sections->addItem(new QListWidgetItem(widget->sectionName()));
    ui->pages->addWidget(widget);
}
