#include <iostream>

#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto toolbar = ui->mainToolBar;
    removeToolBar(toolbar);
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->show();

    TopData * td1 = new TopData("Joe", "Awesome");
    TopData * td2 = new TopData("Mary", "Sweet");		// My sister
    topData.push_back(td1);
    topData.push_back(td2);

    MiddleData * md1a = td1->createMiddleData("Childhood", "Brooklyn Center");
    MiddleData * md1b = td1->createMiddleData("High School", "Park Center");
    MiddleData * md1c = td1->createMiddleData("Current", "Minnesota");

    MiddleData * md2a = td2->createMiddleData("Childhood", "Brooklyn Center");
    MiddleData * md2b = td2->createMiddleData("Somewhere", "She'd Shoot Me");
    MiddleData * md2c = td2->createMiddleData("Current", "I mean it. She would.");

    md1a->createChildData("From", 0);
    md1a->createChildData("Until", 22);
    md2a->createChildData("From", 14);
    md2a->createChildData("Until", 18);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionChange_Data_triggered()
{
    cout << "Triggered!" << endl;
}
