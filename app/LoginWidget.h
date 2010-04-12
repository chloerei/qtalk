#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public QWidget {
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();
    QString jid() const;
    QString password() const;
    QString host() const;
    int port() const;
    bool isRemember() const;

public slots:
    void lock();
    void unlock();
    void showState(QString);

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
