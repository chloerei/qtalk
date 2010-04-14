#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <Preferences.h>

namespace Ui {
    class PreferencesDialog;
}

class PrefAccount;
class QAbstractButton;

class PreferencesDialog : public QDialog {
    Q_OBJECT
public:
    PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();
    bool isAccountChanged();

public slots:
    void readData(Preferences *pref);
    void writeData(Preferences *pref);

signals:
    void applied();

protected:
    void changeEvent(QEvent *e);

private slots:
    void changeConfig(int row);

private:
    Ui::PreferencesDialog *ui;
    PrefAccount *m_prefAccount;
    QPushButton *m_applyButton;
};

#endif // PREFERENCESDIALOG_H
