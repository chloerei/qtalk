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
    ui->hideOfflineCheckBox->setChecked(pref->hideOffline);
}

void PrefGeneral::writeData(Preferences *pref)
{
    if (pref->hideOffline != ui->hideOfflineCheckBox->isChecked()) {
        m_hideOfflineChange = true;
        pref->hideOffline = ui->hideOfflineCheckBox->isChecked();
    }
}

bool PrefGeneral::isHideOfflineChanged() const
{
    return m_hideOfflineChange;
}
