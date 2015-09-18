#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "feature.h";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    setFeatures();
}

void MainWindow::on_pushButton_2_clicked()
{
    triangulate();
}
