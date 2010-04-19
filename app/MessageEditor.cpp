#include "MessageEditor.h"
#include "ui_MessageEditor.h"

MessageEditor::MessageEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageEditor)
{
    ui->setupUi(this);
    connect(ui->textEdit, SIGNAL(textChanged()),
            this, SIGNAL(textChanged()) );
}

MessageEditor::~MessageEditor()
{
    delete ui;
}

void MessageEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString MessageEditor::toPlainText() const
{
    return ui->textEdit->toPlainText();
}

QString MessageEditor::toHtml() const
{
    return ui->textEdit->toHtml();
}

void MessageEditor::clear()
{
    ui->textEdit->clear();
}
