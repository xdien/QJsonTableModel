#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "JsonTableModel.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    JsonTableModel *m_model;
};

#endif // MAINWINDOW_H
