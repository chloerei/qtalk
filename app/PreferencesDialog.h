#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
    class PreferencesDialog;
}

class PrefAccount;
class QAbstractButton;

class PreferencesDialog : public QDialog {
    Q_OBJECT
public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

public slots:
    void readSetting();
    void writeSetting();
    void buttonBoxClicked(QAbstractButton*);

signals:
    void accountSettingChanged();

protected:
    void changeEvent(QEvent *e);

private slots:
    void changeConfig(int row);

private:
    Ui::PreferencesDialog *ui;
    PrefAccount *m_prefAccount;
};

#endif // PREFERENCESDIALOG_H
