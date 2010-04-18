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
    bool isRosterPrefChanged() const;
    bool isRosterIconSizeChanged() const;

signals:
    void rosterIconSizeChanged(int);

private slots:
    void iconSizeChanged();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrefGeneral *ui;
    bool m_hideOfflineChange;
    bool m_showResourcesChange;
    bool m_showSingleResourceChange;
    bool m_rosterIconSizeChange;
};

#endif // PREFGENERAL_H
