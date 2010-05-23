#include "jsoneditormain.h"
#include "ui_jsoneditormain.h"
#include "jsontreemodel.h"

#include <QtGui>

JsonEditorMain::JsonEditorMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JsonEditorMain)
{
    ui->setupUi(this);
    connect(ui->menuCodeTools, SIGNAL(toggled(bool)), this, SLOT(toggleCodeToolbar(bool)));
    connect(ui->menuFindTools, SIGNAL(toggled(bool)), this, SLOT(toggleFindToolbar(bool)));
    connect(ui->menuFileTools, SIGNAL(toggled(bool)), this, SLOT(toggleFileToolbar(bool)));
    connect(ui->menuEditTools, SIGNAL(toggled(bool)), this, SLOT(toggleEditToolbar(bool)));
    connect(ui->menuRefresh, SIGNAL(triggered()), this, SLOT(refreshJsonTree()));

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

void JsonEditorMain::toggleCodeToolbar(bool checked)
{
    checked ? ui->codeToolBar->show() : ui->codeToolBar->hide();
}

void JsonEditorMain::toggleEditToolbar(bool checked)
{
    checked ? ui->editToolBar->show() : ui->editToolBar->hide();
}

void JsonEditorMain::toggleFileToolbar(bool checked)
{
    checked ? ui->fileToolBar->show() : ui->fileToolBar->hide();
}

void JsonEditorMain::toggleFindToolbar(bool checked)
{
    checked ? ui->findToolBar->show() : ui->findToolBar->hide();
}

void JsonEditorMain::refreshJsonTree()
{
    if (!ui->jsonCode->document()->isEmpty())
    {
        QByteArray ss = ui->jsonCode->toPlainText().toUtf8();
        std::string json = ui->jsonCode->toPlainText().toStdString();
        Json::Reader jsonReader;
        jsonValue.clear();
        jsonReader.parse(ss.data(), jsonValue);

        QStringList headers;
        headers << tr("½Úµã") << tr("Öµ");

        JsonTreeModel *model = new JsonTreeModel(headers, jsonValue);
        ui->jsonTree->setModel(model);
        ui->jsonTree->reset();
    }
}














