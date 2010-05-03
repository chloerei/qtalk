#ifndef ADDCONTACTDIALOG_H
#define ADDCONTACTDIALOG_H

#include <QDialog>
#include <QXmppRosterIq.h>
#include <QSet>

namespace Ui {
    class AddContactDialog;
}

class AddContactDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddContactDialog(QWidget *parent = 0);
    ~AddContactDialog();
    void setGroups(QSet<QString> groups);
    QString jid() const;
    QString name() const;
    QString group() const;

private slots:
    void enableOkButton(const QString &str);
    void enableGroupLineEdit(int index);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AddContactDialog *ui;
};

#endif // ADDCONTACTDIALOG_H
