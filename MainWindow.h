#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TopData.h"
#include "MyDataModel.h"

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
    void on_actionChange_Data_triggered();

private:
    Ui::MainWindow *ui;

    PointerVector<TopData> topData;
    MyDataModel * model;
};

#endif // MAINWINDOW_H
