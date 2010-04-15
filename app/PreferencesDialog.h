#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <Preferences.h>

namespace Ui {
    class PreferencesDialog;
}

class PrefWidget;
class PrefAccount;
class QAbstractButton;

class PreferencesDialog : public QDialog {
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = 0);
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

private:
    Ui::PreferencesDialog *ui;
    PrefAccount *m_prefAccount;

    void addSection(PrefWidget *widget);
};

#endif // PREFERENCESDIALOG_H
