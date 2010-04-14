#ifndef PREFACCOUNT_H
#define PREFACCOUNT_H

#include <QWidget>
#include <PrefWidget.h>
#include <Preferences.h>

namespace Ui {
    class PrefAccount;
}

class PrefAccount : public PrefWidget
{
    Q_OBJECT
public:
    explicit PrefAccount(QWidget *parent = 0);
    ~PrefAccount();
    QString sectionName();
    void readData(Preferences *pref);
    void writeData(Preferences *pref);
    bool isChanged() { return m_changed; }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrefAccount *ui;
    bool m_changed;
};

#endif // PREFACCOUNT_H
