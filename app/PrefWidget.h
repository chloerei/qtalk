#ifndef PREFWIDGET_H
#define PREFWIDGET_H

#include <QWidget>
#include <Preferences.h>

class PrefWidget : public QWidget
{
public:
    explicit PrefWidget(QWidget *parent = 0);
    virtual QString sectionName() = 0;
    virtual void readData(Preferences *pref) = 0;
    virtual void writeData(Preferences *pref) = 0;
};

#endif // PREFWIDGET_H
