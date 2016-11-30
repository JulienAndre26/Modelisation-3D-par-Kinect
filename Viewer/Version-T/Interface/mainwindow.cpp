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
    dirToImport.setNameFilters(QStringList()<<"*.ply");
    QFileInfoList list = dirToImport.entryInfoList();

    QStringList res;

    for (int i = 0; i < list.size(); i++)
    {
		listContent.insert(list.at(i).fileName(), list.at(i).absoluteFilePath());
        res.append(list.at(i).fileName());
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
			QStringList splitColor = line.split(" ");
			int cmp = QString::compare("[COLOR]", splitColor.at(0), Qt::CaseInsensitive);
			if (cmp == 0)
			{
				withColor = true;
			}
        }

        ui->lwImportInfo->addItems(infos);
    }
}



void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = listContent.find(item->data(Qt::DisplayRole).toString()).value();
	
	filePath = new char[path.size()+1];
	filePath[path.size()] = '\0';
	strcpy(filePath, path.toLocal8Bit().data());

	std::thread *t = new std::thread([this] { this->showPLY(); });
	t->join();
	
}

void MainWindow::showPLY() {
	
	pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
	int v(20);
	pv->setSize(800, 600);
	pv->setShowFPS(true);
	pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);
	
	if (withColor)
	{
		pcl::PointCloud<pcl::PointXYZRGB>::Ptr src(new pcl::PointCloud<pcl::PointXYZRGB>);
		pcl::io::loadPLYFile<pcl::PointXYZRGB>(filePath, *src);
		// color
		pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> src_rgb(src);
		// point cloud
		pv->addPointCloud<pcl::PointXYZRGB>(src, src_rgb, "v1_source", v);
	}
	else
	{
		pcl::PointCloud<pcl::PointXYZRGB>::Ptr src_raw(new pcl::PointCloud<pcl::PointXYZRGB>);
		pcl::PointCloud<pcl::PointXYZ>::Ptr src(new pcl::PointCloud<pcl::PointXYZ>);
		pcl::io::loadPLYFile<pcl::PointXYZRGB>(filePath, *src_raw);
		for (int i = 0; i < src_raw->size(); i++)
			src->push_back(pcl::PointXYZ(src_raw->at(i).x, src_raw->at(i).y, src_raw->at(i).z));
		// color
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_rgb(src, 50, 210, 210);
		// point cloud
		pv->addPointCloud<pcl::PointXYZ>(src, src_rgb, "v1_source", v);
	}
	
	// draw cloud
	pv->spin();
}