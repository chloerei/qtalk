#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class ConfigDialog;
}

class AccountConfigWidget;
class QAbstractButton;

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

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
    Ui::ConfigDialog *ui;
    AccountConfigWidget *m_accountConfigWidget;
};

#endif // CONFIGDIALOG_H
