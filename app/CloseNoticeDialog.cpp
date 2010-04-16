#include "CloseNoticeDialog.h"
#include "ui_CloseNoticeDialog.h"

CloseNoticeDialog::CloseNoticeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloseNoticeDialog)
{
    ui->setupUi(this);
}

CloseNoticeDialog::~CloseNoticeDialog()
{
    delete ui;
}

void CloseNoticeDialog::changeEvent(QEvent *e)
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

void CloseNoticeDialog::readData(Preferences *pref)
{
    ui->closeToTrayCheckBox->setChecked(pref->closeToTray);
    ui->closeToTrayNoticeCheckBox->setChecked(pref->closeToTrayNotice);
}


void CloseNoticeDialog::writeDate(Preferences *pref)
{
    pref->closeToTray = ui->closeToTrayCheckBox->isChecked();
    pref->closeToTrayNotice = ui->closeToTrayNoticeCheckBox->isChecked();
}
