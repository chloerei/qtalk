#ifndef ACCOUNTCONFIGWIDGET_H
#define ACCOUNTCONFIGWIDGET_H

#include <QWidget>

namespace Ui {
    class AccountConfigWidget;
}

class AccountConfigWidget : public QWidget {
    Q_OBJECT
public:
    explicit AccountConfigWidget(QWidget *parent = 0);
    ~AccountConfigWidget();
    void readSetting();
    void writeSetting();

signals:
    void settingChanged();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AccountConfigWidget *ui;
};

#endif // ACCOUNTCONFIGWIDGET_H
