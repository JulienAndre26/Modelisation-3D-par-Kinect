#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QMainWindow::showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    QString importName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Import files (*.import)"));

    QFileInfo importFileInfo(importName);

    list = detectPlyFiles(importFileInfo.absoluteDir());

    ui->listWidget->addItems(list);

    readImportFile(importName);
}

QStringList MainWindow::detectPlyFiles(QDir dirToImport)
{
    dirToImport.setNameFilters(QStringList()<<"*.ply"<<"*.obj"<<"*.stl");
    QFileInfoList list = dirToImport.entryInfoList();

    QStringList res;

    for (int i = 0; i < list.size(); i++)
    {
        res.append(list.at(i).absoluteFilePath());
    }

    return res;
}

void MainWindow::on_btnMerge_clicked()
{
    if (list.size() < 2)
    {
        QMessageBox::warning(this, "Empty list", "Please select .import file with at least 2 models");
    }
    else
    {
        for (int i = 0; i < list.size()-1; i++)
        {
            ui->lbCurrentMerge->setText("Current merge : " + list.at(i) + " with " + list.at(i+1));
            ui->progressBar->setValue(((float)i/(list.size()-1)*100));
            QThread::sleep(1);
            QCoreApplication::processEvents();
        }

        ui->progressBar->setValue(100);
        ui->lbCurrentMerge->setText("Finished");
    }
}

void MainWindow::readImportFile(QString import)
{
    QFile importFile(import);
    if(importFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&importFile);
        QStringList infos;

        while(!stream.atEnd())
        {
            QString line = stream.readLine();
//            QStringList split = QString(line).split(" ");
//            infos.append(split.at(1));
            infos.append(line);
        }

        ui->lwImportInfo->addItems(infos);
    }
}



void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = item->data(Qt::DisplayRole).toString();

//    pcl::io::loadPLYFile(path);
}
