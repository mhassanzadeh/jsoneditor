#ifndef JSONEDITORMAIN_H
#define JSONEDITORMAIN_H

#include <QMainWindow>

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
};

#endif // JSONEDITORMAIN_H
