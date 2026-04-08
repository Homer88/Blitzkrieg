#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "cXml.h"

extern cXml XmlFile;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    XmlFile.openConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}

