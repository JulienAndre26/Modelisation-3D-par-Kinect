#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->progressBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    QString importName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Import files (*.import)"));

    QFileInfo importFileInfo(importName);
	if (readImportFile(importName))
	{
		list.clear();
		list = detectPlyFiles(importFileInfo.absoluteDir());
		ui->listWidget->addItems(list);
	}    
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
		ui->progressBar->setVisible(true);
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

bool MainWindow::readImportFile(QString import)
{
    QFile importFile(import);
    if(importFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&importFile);
        QStringList infos;

//        while(!stream.atEnd())
//        {
//            QString line = stream.readLine();
////            QStringList split = QString(line).split(" ");
////            infos.append(split.at(1));
//            infos.append(line);
//			//QStringList splitColor = line.split(" ");
//			int cmp = QString::compare("[COLOR] true", line, Qt::CaseInsensitive);
//			if (cmp == 0)
//			{
//				withColor = true;
//			}
//        }
//
//        ui->lwImportInfo->addItems(infos);

		int linesCount = 0;
		while (!stream.atEnd())
		{
			stream.readLine();
			linesCount++;
		}

		if (linesCount != 8)
		{
			QMessageBox::critical(this, "Bad file import", "Please select a correct import file");
			return false;
		}
		else
		{
			stream.seek(0);
			QString path = stream.readLine().split("\\").last();
			ui->lbPath->setText("Capture name : " + path);

			QString count = stream.readLine().split(" ").last();
			ui->lbCount->setText("Files count : " + count);

			QString format = stream.readLine().split(" ").last();
			ui->lbFormat->setText("Format : " + format);

			QString color = stream.readLine().split(" ").last();

			int cmp = QString::compare("true", color, Qt::CaseInsensitive);
			if (cmp == 0)
			{
				ui->lbColor->setText("Color : yes");
				withColor = true;
			}
			else
			{
				ui->lbColor->setText("Color : no");
			}

			QString voxelsPerMeter = stream.readLine().split(" ").last();
			ui->lbVoxels->setText("Voxels/meter : " + voxelsPerMeter);
			int vpmValue = voxelsPerMeter.toInt();

			QString x = stream.readLine().split(" ").last();
			int xValue = x.toInt();
			ui->lbWidth->setText("Width : " + QString::number(xValue / vpmValue) + " meter(s)");

			QString y = stream.readLine().split(" ").last();
			int yValue = y.toInt();
			ui->lbHeight->setText("Height : " + QString::number(yValue / vpmValue) + " meter(s)");

			QString z = stream.readLine().split(" ").last();
			int zValue = z.toInt();
			ui->lbDepth->setText("Depth : " + QString::number(zValue / vpmValue) + " meter(s)");
		}
    }

	return true;
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = listContent.find(item->data(Qt::DisplayRole).toString()).value();
	
	filePath = new char[path.size()+1];
	filePath[path.size()] = '\0';
	strcpy(filePath, path.toLocal8Bit().data());

	std::thread *t = new std::thread([this] { this->showPlane(); }); // THIBAUUUUUUUUUUUUUUUUUUUUUT 2D
	t->join(); // Essayer detach pour voir si on a la main sur l'autre fenêtre en même temps
}

/*
* Show the 3D Model selected (filePath) - Handling Colors
*/
void MainWindow::showPLY() {
	
	pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
	int v(20);
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

/*
 * Show the 3D Model selected (filePath) in 2D
 */
void MainWindow::showPlane()
{
	// Point cloud in 2D
	pcl::PointCloud<pcl::PointXYZ>::Ptr src_projected(new pcl::PointCloud<pcl::PointXYZ>);
	

	// --------- Copied from ShowPLY()

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr src_raw(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr src(new pcl::PointCloud<pcl::PointXYZ>);
	
	pcl::io::loadPLYFile<pcl::PointXYZRGB>(filePath, *src_raw);

	for (int i = 0; i < src_raw->size(); i++)
		src->push_back(pcl::PointXYZ(src_raw->at(i).x, src_raw->at(i).y, src_raw->at(i).z));

	// Color
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_rgb(src, 50, 210, 210);

	// --------------------------------


	// Create a set of planar coefficients with X=Y=0,Z=1
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	coefficients->values.resize(4);
	coefficients->values[0] = coefficients->values[1] = 0;
	coefficients->values[2] = 1.0;
	coefficients->values[3] = 0;

	// Create the filtering object
	pcl::ProjectInliers<pcl::PointXYZ> proj;
	proj.setModelType(pcl::SACMODEL_PLANE);
	proj.setInputCloud(src);
	proj.setModelCoefficients(coefficients);
	proj.filter(*src_projected);

	// Visualizer
	pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
	
	int v(20);
	pv->setShowFPS(true);
	pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);
	pv->addPointCloud<pcl::PointXYZ>(src_projected, src_rgb, "v1_source", v);
	
	// Show
	pv->spin();
}