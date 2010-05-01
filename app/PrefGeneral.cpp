#include "PrefGeneral.h"
#include "ui_PrefGeneral.h"

PrefGeneral::PrefGeneral(QWidget *parent) :
    PrefWidget(parent),
    ui(new Ui::PrefGeneral)
{
    ui->setupUi(this);
    connect(ui->iconSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(rosterIconSizeChanged(int)) );
    connect(ui->iconSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(iconSizeChanged()) );
}

PrefGeneral::~PrefGeneral()
{
    delete ui;
}

QIcon PrefGeneral::sectionIcon() const
{
    return QIcon(":/images/preferences-other.png");
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
    m_rosterIconSizeChange = false;
    ui->hideOfflineCheckBox->setChecked(pref->hideOffline);
    ui->showResourcesCheckBox->setChecked(pref->showResources);
    ui->iconSizeSpinBox->setValue(pref->rosterIconSize);
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

    pref->rosterIconSize = ui->iconSizeSpinBox->value();
    pref->closeToTray = ui->closeToTrayCheckBox->isChecked();
    pref->closeToTrayNotice = ui->closeToTrayNoticeCheckBox->isChecked();
}

bool PrefGeneral::isRosterPrefChanged() const
{
    return m_hideOfflineChange
            || m_showResourcesChange
            || m_showSingleResourceChange;
}

bool PrefGeneral::isRosterIconSizeChanged() const
{
    return m_rosterIconSizeChange;
}

void PrefGeneral::iconSizeChanged()
{
    m_rosterIconSizeChange = true;
}
