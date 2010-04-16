#ifndef CLOSENOTICEDIALOG_H
#define CLOSENOTICEDIALOG_H

#include <QDialog>
#include "Preferences.h"

namespace Ui {
    class CloseNoticeDialog;
}

class CloseNoticeDialog : public QDialog {
    Q_OBJECT
public:
    explicit CloseNoticeDialog(QWidget *parent = 0);
    ~CloseNoticeDialog();
    void readData(Preferences *pref);
    void writeDate(Preferences *pref);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CloseNoticeDialog *ui;
};

#endif // CLOSENOTICEDIALOG_H
