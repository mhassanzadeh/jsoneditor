#include "jsoneditormain.h"
#include "ui_jsoneditormain.h"

JsonEditorMain::JsonEditorMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JsonEditorMain)
{
    ui->setupUi(this);
}

JsonEditorMain::~JsonEditorMain()
{
    delete ui;
}

void JsonEditorMain::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
