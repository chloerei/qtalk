#ifndef MESSAGEEDITOR_H
#define MESSAGEEDITOR_H

#include <QWidget>

namespace Ui {
    class MessageEditor;
}

class MessageEditor : public QWidget {
    Q_OBJECT
public:
    explicit MessageEditor(QWidget *parent = 0);
    ~MessageEditor();
    QString toPlainText() const;
    QString toHtml() const;
    void clear();

signals:
    void textChanged();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::MessageEditor *ui;
};

#endif // MESSAGEEDITOR_H
