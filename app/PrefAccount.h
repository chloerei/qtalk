#ifndef PREFACCOUNT_H
#define PREFACCOUNT_H

#include <QWidget>

namespace Ui {
    class PrefAccount;
}

class PrefAccount : public QWidget {
    Q_OBJECT
public:
    explicit PrefAccount(QWidget *parent = 0);
    ~PrefAccount();
    void readSetting();
    void writeSetting();

signals:
    void settingChanged();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrefAccount *ui;
};

#endif // PREFACCOUNT_H
