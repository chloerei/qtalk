#ifndef PREFCHATWINDOW_H
#define PREFCHATWINDOW_H

#include <QWidget>
#include <PrefWidget.h>

namespace Ui {
    class PrefChatWindow;
}

class PrefChatWindow : public PrefWidget
{
    Q_OBJECT
public:
    explicit PrefChatWindow(QWidget *parent = 0);
    ~PrefChatWindow();
    QString sectionName() const;
    void readData(Preferences *pref);
    void writeData(Preferences *pref);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrefChatWindow *ui;
};

#endif // PREFCHATWINDOW_H
