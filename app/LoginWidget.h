#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <Preferences.h>

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public QWidget {
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

public slots:
    void lock();
    void unlock();
    void showState(QString);
    void readData(Preferences *pref);
    void writeData(Preferences *pref);

private slots:
    void clickedLogin();
    void getHost();

signals:
    void login();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWINDOW_H
