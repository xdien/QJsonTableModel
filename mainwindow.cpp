#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList header;
    header << "h1" << "h2"<<"h3"<<"h4"<<"h5";
    TreeModel *m_treemodel = new TreeModel(header,this);
    m_treemodel->setQuery("asd");

    m_model = new JsonTableModel();
    m_model->setJsonString("dasd");
    ui->treeView->setModel(m_treemodel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    model->
}
