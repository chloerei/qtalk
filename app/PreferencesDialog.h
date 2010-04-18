#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <Preferences.h>

namespace Ui {
    class PreferencesDialog;
}

class PrefWidget;
class PrefAccount;
class PrefGeneral;

class PreferencesDialog : public QDialog {
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();
    bool isAccountChanged();
    bool isRosterPrefChanged();

public slots:
    void readData(Preferences *pref);
    void writeData(Preferences *pref);
    void prefRejected();

signals:
    void applied();
    void rosterIconSizeChanged(int);
    void rosterIconReseze();

protected:
    void changeEvent(QEvent *e);

private slots:

private:
    Ui::PreferencesDialog *ui;
    PrefAccount *m_prefAccount;
    PrefGeneral *m_prefGeneral;

    void addSection(PrefWidget *widget);
};

#endif // PREFERENCESDIALOG_H
