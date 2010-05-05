#ifndef InfoEventStackWidget_H
#define InfoEventStackWidget_H

#include <QWidget>

namespace Ui {
    class InfoEventStackWidget;
}

class InfoEventStackWidget : public QWidget {
    Q_OBJECT
public:
    explicit InfoEventStackWidget(QWidget *parent = 0);
    ~InfoEventStackWidget();

protected:
    void changeEvent(QEvent *e);

private slots:
    void closeSlot();

private:
    Ui::InfoEventStackWidget *ui;
};

#endif // InfoEventStackWidget_H
