#ifndef CONTACTINFODIALOG_H
#define CONTACTINFODIALOG_H

#include <QDialog>
#include <QXmppVCard.h>

namespace Ui {
    class ContactInfoDialog;
}

class ContactInfoDialog : public QDialog {
    Q_OBJECT
public:
    explicit ContactInfoDialog(QWidget *parent = 0);
    ~ContactInfoDialog();
    void setData(QString name, QString jid, const QXmppVCard &vCard);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ContactInfoDialog *ui;
};

#endif // CONTACTINFODIALOG_H
