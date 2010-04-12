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
    bool isRemember() const;

public slots:
    void lock();

signals:
    void login();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWINDOW_H
