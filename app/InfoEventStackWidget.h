#ifndef InfoEventStackWidget_H
#define InfoEventStackWidget_H

#include <QWidget>

namespace Ui {
    class InfoEventStackWidget;
}

class QXmppClient;
class QTimeLine;

class InfoEventStackWidget : public QWidget {
    Q_OBJECT
public:
    explicit InfoEventStackWidget(QXmppClient *client, QWidget *parent = 0);
    ~InfoEventStackWidget();
    void addSubscribeRequest(const QString &bareJid);
    void setAnimeVisible(bool visible);

protected:
    void changeEvent(QEvent *e);

private slots:
    void previousSlot();
    void nextSlot();
    void closeSlot();
    void animeSlot(qreal amount);

private:
    Ui::InfoEventStackWidget *ui;
    QXmppClient *m_client;
    QTimeLine *m_timeLine;

    void updatePage();
};

#endif // InfoEventStackWidget_H
