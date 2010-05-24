#ifndef JSONEDITORMAIN_H
#define JSONEDITORMAIN_H

#include <QMainWindow>
#include "jsonc/jsoncxx.h"

namespace Ui {
    class JsonEditorMain;
}

class JsonEditorMain : public QMainWindow {
    Q_OBJECT
public:
    JsonEditorMain(QWidget *parent = 0);
    ~JsonEditorMain();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::JsonEditorMain *ui;
    Json::Value jsonValue;

private slots:
    void toggleFileToolbar(bool checked);
    void toggleEditToolbar(bool checked);
    void toggleFindToolbar(bool checked);
    void toggleCodeToolbar(bool checked);
    void refreshJsonTree();
    void insertTreeNode();
    void deleteTreeNode();
    void insertTreeChild();
    void updateActions();
    void treeViewDataChanged();
    
};

#endif // JSONEDITORMAIN_H
