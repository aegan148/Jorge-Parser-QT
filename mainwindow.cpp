#include "mainwindow.h"
#include "ui_mainwindow.h"


TinyClassObj* parserOBJ;
doc_Razdel* xlsxObject;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_go_Button_clicked()
{
    parserOBJ = new TinyClassObj();
        xlsxObject = new doc_Razdel();
        QString fileNamet="D:\\qt_proj\\build-hello-Desktop_Qt_5_13_1_MinGW_32_bit-Debug\\ReadFrom\\sourse1.xml";
       // QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        //                                                  "/home",
         //                                                 tr("*.xml"));

        if (!parserOBJ->loadXmlSourse(fileNamet)) return ;
        parserOBJ->goToReadXml();
        xlsxObject->GomakeXlsx(parserOBJ->getChapterList(), parserOBJ->getVidRabot(), parserOBJ->getIndex(), parserOBJ->getSMR(),parserOBJ->getProperyes());
}
