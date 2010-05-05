#ifndef InfoEventStackWidget_H
#define InfoEventStackWidget_H

#include <QWidget>

namespace Ui {
    class InfoEventStackWidget;
}

class QXmppClient;

class InfoEventStackWidget : public QWidget {
    Q_OBJECT
public:
    explicit InfoEventStackWidget(QXmppClient *client, QWidget *parent = 0);
    ~InfoEventStackWidget();
    void addSubscribeRequest(const QString &bareJid);

protected:
    void changeEvent(QEvent *e);

private slots:
    void previousSlot();
    void nextSlot();
    void closeSlot();

private:
    Ui::InfoEventStackWidget *ui;
    QXmppClient *m_client;

    void updatePage();
};

#endif // InfoEventStackWidget_H
