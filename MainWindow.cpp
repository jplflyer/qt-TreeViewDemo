#include <iostream>

#include <QTimer>

#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new MyDataModel(nullptr, &topData))

{
    ui->setupUi(this);
    ui->treeView->setModel(model);

    auto toolbar = ui->mainToolBar;
    removeToolBar(toolbar);
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->show();

    TopData * td1 = new TopData("Joe", "Awesome");
    TopData * td2 = new TopData("MySis", "Sweet");		// My sister
    topData.push_back(td1);
    topData.push_back(td2);

    MiddleData * md1a = td1->createMiddleData("Childhood", "Brooklyn Center");
    td1->createMiddleData("High School", "Park Center");
    td1->createMiddleData("Current", "Minnesota");

    MiddleData * md2a = td2->createMiddleData("Childhood", "Brooklyn Center");
    td2->createMiddleData("Somewhere", "She'd Shoot Me");
    td2->createMiddleData("Current", "I mean it. She would.");

    md1a->createChildData("From", 0);
    md1a->createChildData("Until", 22);
    md2a->createChildData("From", 14);
    md2a->createChildData("Until", 18);

    // Columns are allocated evenly, except the last which by default gets the stretch. This lets me make two of them wider.
    ui->treeView->setColumnWidth(0, ui->treeView->columnWidth(0) * 2);
    ui->treeView->setColumnWidth(2, ui->treeView->columnWidth(2) * 2);

    // Set up to automatically force a data change.
    QTimer::singleShot(5000, this, SLOT(updateData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * This does a data update then tells the form.
 */
void MainWindow::on_actionChange_Data_triggered()
{
    TopData * td = topData.at(0);
    MiddleData * md = td->middleData.at(0);
    md->name = md->name + ".";

    md->createChildData("Update", 10);

    //ui->treeView->dataChanged(QModelIndex(), QModelIndex());
    model->layoutChanged();
}

/**
 * Timer version of automatically changing the data.
 */
void MainWindow::updateData() {
    on_actionChange_Data_triggered();
    QTimer::singleShot(5000, this, SLOT(updateData()));
}
