#include "mainwindow.h"
#include "JsonTableModel.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList header;
    header << "h1" << "h2"<<"h3"<<"h4"<<"h5";
    JsonTableModel *m_treemodel = new JsonTableModel(header,this);
    QString str = "["
                  "{\"key1\": \"value1\", \"key2\":\"value2\", \"key3\":\"value3\",\"key4\":\"value4\" ,\"key5\": [{\"sub1\":\"vsub1\",\"sub2\":\"vsub2\",\"sub3\":\"vsub3\",\"sub4arr\":[{\"sub1a\":\"vsub1a\", \"sub1a1\":\"vsub1a1\"},{\"sub1a\":\"vsub1b\"}] }] },"
                  "{\"key1\": \"value1_2\", \"key2\":\"value2_2\", \"key3\":\"value3_2\",\"key4\":\"value4_2\"},"
                  "{\"key1\": \"value1_3\", \"key2\":\"value2_3\", \"key3\":\"value3_3\",\"key4\":\"value4_3\"},"
                  "{\"key1\": \"value1_4\", \"key2\":\"value2_4\", \"key3\":\"value3_4\",\"key4\":\"value4_4\" ,\"key5\": [{\"sub1\":\"vsub1_2\",\"sub2\":\"vsub2_2\",\"sub3\":\"vsub3_2\"}] },"
                  "{\"key1\": \"value1_5\", \"key2\":\"value2_5\", \"key3\":\"value3_5\",\"key4\":\"value4_5\" ,\"key5\": [{\"sub1\":\"vsub1_3\",\"sub2\":\"vsub2_3\",\"sub3\":\"vsub3_3\"}] },"
            "{\"key1\": \"value1_6\", \"key2\":\"value2_6\", \"key3\":\"value3_6\",\"key4\":\"value4_6\"},"
            "{\"key1\": \"value1_7\", \"key2\":\"value2_7\", \"key3\":\"value3_7\",\"key4\":\"value4_7\"}"
                  "]";
    m_treemodel->setJsonArrayString(&str);
    ui->tableView->setModel(m_treemodel);
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
