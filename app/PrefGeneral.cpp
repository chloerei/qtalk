#include "PrefGeneral.h"
#include "ui_PrefGeneral.h"

PrefGeneral::PrefGeneral(QWidget *parent) :
    PrefWidget(parent),
    ui(new Ui::PrefGeneral)
{
    ui->setupUi(this);
}

PrefGeneral::~PrefGeneral()
{
    delete ui;
}

void PrefGeneral::changeEvent(QEvent *e)
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

QString PrefGeneral::sectionName() const
{
    return QString("General");
}

void PrefGeneral::readData(Preferences *pref)
{
    m_hideOfflineChange = false;
    m_showResourcesChange = false;
    m_showSingleResourceChange = false;
    ui->hideOfflineCheckBox->setChecked(pref->hideOffline);
    ui->showResourcesCheckBox->setChecked(pref->showResources);
    ui->showSingleResourceCheckBox->setEnabled(pref->showResources);
    ui->showSingleResourceCheckBox->setChecked(pref->showSingleResource);
    ui->closeToTrayCheckBox->setChecked(pref->closeToTray);
    ui->closeToTrayNoticeCheckBox->setChecked(pref->closeToTrayNotice);
}

void PrefGeneral::writeData(Preferences *pref)
{
    if (pref->hideOffline != ui->hideOfflineCheckBox->isChecked()) {
        m_hideOfflineChange = true;
        pref->hideOffline = ui->hideOfflineCheckBox->isChecked();
    }

    if (pref->showResources != ui->showResourcesCheckBox->isChecked()) {
        m_showResourcesChange = true;
        pref->showResources = ui->showResourcesCheckBox->isChecked();
    }

    if (pref->showSingleResource != ui->showSingleResourceCheckBox->isChecked()) {
        m_showSingleResourceChange = true;
        pref->showSingleResource= ui->showSingleResourceCheckBox->isChecked();
    }

    pref->closeToTray = ui->closeToTrayCheckBox->isChecked();
    pref->closeToTrayNotice = ui->closeToTrayNoticeCheckBox->isChecked();
}

bool PrefGeneral::isRosterPrefChanged() const
{
    return m_hideOfflineChange
            || m_showResourcesChange
            || m_showSingleResourceChange;
}
