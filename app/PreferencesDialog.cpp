#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "PrefWidget.h"
#include "PrefAccount.h"
#include "PrefGeneral.h"
#include "PrefChatWindow.h"
#include <QPushButton>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    m_prefAccount(new PrefAccount(this)),
    m_prefGeneral(new PrefGeneral(this)),
    m_prefChatWindow(new PrefChatWindow(this))
{
    ui->setupUi(this);
    m_prefAccount->hide();

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            this, SIGNAL(applied()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SIGNAL(applied()));
    connect(m_prefGeneral, SIGNAL(rosterIconSizeChanged(int)),
            this, SIGNAL(rosterIconSizeChanged(int)) );
    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(prefRejected()) );

    addSection(m_prefAccount);
    addSection(m_prefGeneral);
    addSection(m_prefChatWindow);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

bool PreferencesDialog::isAccountChanged()
{
    return m_prefAccount->isChanged();
}

bool PreferencesDialog::isRosterPrefChanged()
{
    return m_prefGeneral->isRosterPrefChanged();
}

void PreferencesDialog::readData(Preferences *pref)
{
    foreach (PrefWidget *widget, m_sections) {
        widget->readData(pref);
    }
}

void PreferencesDialog::writeData(Preferences *pref)
{
    foreach (PrefWidget *widget, m_sections) {
        widget->writeData(pref);
    }
}

void PreferencesDialog::prefRejected()
{
    if (m_prefGeneral->isRosterIconSizeChanged()) {
        emit rosterIconReseze();
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

void PreferencesDialog::addSection(PrefWidget *widget)
{
    ui->sections->addItem(new QListWidgetItem(widget->sectionIcon(), widget->sectionName()));
    ui->pages->addWidget(widget);
    m_sections << widget;
}
