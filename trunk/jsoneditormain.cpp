#include "jsoneditormain.h"
#include "ui_jsoneditormain.h"
#include "jsontreemodel.h"

#include <QtGui>

JsonEditorMain::JsonEditorMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JsonEditorMain),
    newInsertText(tr("插入新数据")),
    treeViewColumnKey(tr("节点")),
    treeViewColumnValue(tr("值")),
    treeViewColumnType(tr("类型"))
{
    ui->setupUi(this);
    connect(ui->menuCodeTools, SIGNAL(toggled(bool)), this, SLOT(toggleCodeToolbar(bool)));
    connect(ui->menuFindTools, SIGNAL(toggled(bool)), this, SLOT(toggleFindToolbar(bool)));
    connect(ui->menuFileTools, SIGNAL(toggled(bool)), this, SLOT(toggleFileToolbar(bool)));
    connect(ui->menuEditTools, SIGNAL(toggled(bool)), this, SLOT(toggleEditToolbar(bool)));
    connect(ui->menuRefresh, SIGNAL(triggered()), this, SLOT(refreshJsonTree()));
    connect(ui->menuInsertNode, SIGNAL(triggered()), this, SLOT(insertTreeNode()));
    connect(ui->menuInsertChild, SIGNAL(triggered()), this, SLOT(insertTreeChild()));
    connect(ui->menuDeleteNode, SIGNAL(triggered()), this, SLOT(deleteTreeNode()));
    connect(ui->jsonTree, SIGNAL(clicked(QModelIndex)), this, SLOT(updateActions()));
    connect(ui->menuFormat, SIGNAL(triggered()), this, SLOT(formatCode()));
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
        QByteArray ss = ui->jsonCode->toPlainText().toLocal8Bit();
        std::string json = ui->jsonCode->toPlainText().toStdString();
        Json::Reader jsonReader;
        jsonValue.clear();
        jsonReader.parse(ss.data(), jsonValue);

        QStringList headers;
        headers << treeViewColumnKey << treeViewColumnValue << treeViewColumnType;



        QAbstractItemModel *oldModel = ui->jsonTree->model();
        if (oldModel != NULL)
            oldModel->disconnect(SIGNAL(dataChanged(QModelIndex,QModelIndex)));

        JsonTreeModel *model = new JsonTreeModel(headers, jsonValue);
        QItemSelectionModel *selectionModel = ui->jsonTree->selectionModel();
        connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(treeViewDataChanged()));
        ui->jsonTree->setModel(model);
        ui->jsonTree->reset();
        delete selectionModel;
        delete oldModel;


        ui->jsonTree->expandAll();

        for (int i = 0; i < ui->jsonTree->model()->columnCount(); i++)
            ui->jsonTree->resizeColumnToContents(i);
    }
}

void JsonEditorMain::insertTreeNode()
{
    QModelIndex index = ui->jsonTree->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->jsonTree->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column)
    {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model->setData(child, QVariant(newInsertText), Qt::EditRole);
    }
}

void JsonEditorMain::insertTreeChild()
{
    QModelIndex index = ui->jsonTree->selectionModel()->currentIndex();
    JsonTreeModel *model = (JsonTreeModel*)ui->jsonTree->model();

    if (index.column() == 1)
    {
        QMessageBox::information(this, tr("提示信息"), tr("必须选择一个“节点”才能添加子节点。"));
        return;
    }

    if (model->columnCount(index) == 0)
    {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); column++)
    {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant(newInsertText), Qt::EditRole);
    }

    ui->jsonTree->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                    QItemSelectionModel::ClearAndSelect);
    updateActions();
}

void JsonEditorMain::deleteTreeNode()
{
    QModelIndex index = ui->jsonTree->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->jsonTree->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void JsonEditorMain::updateActions()
 {
     bool hasSelection = !ui->jsonTree->selectionModel()->selection().isEmpty();
     ui->menuDeleteNode->setEnabled(hasSelection);

     bool hasCurrent = ui->jsonTree->selectionModel()->currentIndex().isValid();
     ui->menuInsertNode->setEnabled(hasCurrent);
     ui->menuInsertChild->setEnabled(ui->jsonTree->selectionModel()->currentIndex().column() == 0);
 }

void JsonEditorMain::treeViewDataChanged()
{
    for (int i = 0; i < ui->jsonTree->model()->columnCount(); i++)
        ui->jsonTree->resizeColumnToContents(i);
}

void JsonEditorMain::formatCode()
{
    ui->jsonCode->clear();

    QString codeText = "";
    JsonTreeModel *model;
    model = (JsonTreeModel*)ui->jsonTree->model();
    codeText += treeFormat(model->getRootItem(), "", true);

    ui->jsonCode->setPlainText(codeText);
}

QString JsonEditorMain::treeFormat(JsonTreeItem *treeItem, QString indent, bool noHeader)
{
    QString resultStr;
    QString objectKey = treeItem->data(0).toString();
    QString objectValue = treeItem->data(1).toString();
    QString objectType = treeItem->data(2).toString();

    if (noHeader)
    {
        resultStr = indent;
    }
    else
    {
        resultStr = indent + QString("\"") + objectKey + "\": ";
    }

    if (objectType.compare(tr("对象"), Qt::CaseInsensitive) == 0
        || objectValue.compare(treeViewColumnValue, Qt::CaseInsensitive) == 0)
    {
        resultStr += "{\n";
        JsonTreeItem *subObjectItem;
        for (int i = 0; i < treeItem->childCount(); i++)
        {
            subObjectItem = treeItem->child(i);
            resultStr += treeFormat(subObjectItem, indent + "    ");
        }
        resultStr.remove(resultStr.length() - 2, 1);
        resultStr += indent + "},\n";
    }
    else if (objectType.compare(tr("数组"), Qt::CaseInsensitive) == 0)
    {
        resultStr += "[\n";
        JsonTreeItem *subObjectItem;
        for (int i = 0; i < treeItem->childCount(); i++)
        {
            subObjectItem = treeItem->child(i);
            resultStr += treeFormat(subObjectItem, indent + "    ", true);
        }
        resultStr.remove(resultStr.length() - 2, 1);
        resultStr += indent + "],\n";
    }
    else
    {
        if (objectType.compare(tr("字符串"), Qt::CaseInsensitive) == 0 ||
            objectType.compare(newInsertText, Qt::CaseInsensitive) == 0)
        {
            resultStr += "\"" + objectValue + "\",\n";
        }
        else
        {
            resultStr += objectValue + ",\n";
        }
    }

    return resultStr;
}





