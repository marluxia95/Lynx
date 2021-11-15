#include "editorwindow.h"
#include "ui_editorwindow.h"
#include "enginedisplay.h"

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);

    ui->mdiArea->addSubWindow(new EngineDisplay());


}

EditorWindow::~EditorWindow()
{
    delete ui;
}

