#ifndef PREFGENERAL_H
#define PREFGENERAL_H

#include <QWidget>
#include <PrefWidget.h>

namespace Ui {
    class PrefGeneral;
}

class PrefGeneral : public PrefWidget
{
    Q_OBJECT
public:
    explicit PrefGeneral(QWidget *parent = 0);
    ~PrefGeneral();
    QString sectionName() const;
    void readData(Preferences *pref);
    void writeData(Preferences *pref);
    bool isHideOfflineChanged() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrefGeneral *ui;
    bool m_hideOfflineChange;
};

#endif // PREFGENERAL_H
