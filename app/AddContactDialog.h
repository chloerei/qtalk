#ifndef ADDCONTACTDIALOG_H
#define ADDCONTACTDIALOG_H

#include <QDialog>
#include <QXmppRosterIq.h>

namespace Ui {
    class AddContactDialog;
}

class AddContactDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddContactDialog(QWidget *parent = 0);
    ~AddContactDialog();
    QString jid() const;

private slots:
    void enableOkButton(const QString &str);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AddContactDialog *ui;
};

#endif // ADDCONTACTDIALOG_H
