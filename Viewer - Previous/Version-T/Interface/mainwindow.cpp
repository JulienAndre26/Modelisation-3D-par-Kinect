#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setAcceptDrops(true);
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
		listContent.clear();
		
		list = detectPlyFiles(importFileInfo.absoluteDir());
		ui->listWidget->clear();
		ui->listWidget->addItems(listContent.keys());
	}    
}

QStringList MainWindow::detectPlyFiles(QDir dirToImport)
{
    dirToImport.setNameFilters(QStringList()<<"*.ply");
    QFileInfoList listInfo = dirToImport.entryInfoList();

    QStringList res;

    for (int i = 0; i < listInfo.size(); i++)
    {
		listContent.insert(listInfo.at(i).fileName(), listInfo.at(i).absoluteFilePath());
        res.append(listInfo.at(i).fileName());
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
			withColor = false;
			stream.seek(0);
			QString path = stream.readLine().split("\\").last();
			ui->lbCaptureName->setText(path);
			stream.readLine();
			stream.readLine();
			QString color = stream.readLine().split(" ").last();

			int cmp = QString::compare("true", color, Qt::CaseInsensitive);
			if (cmp == 0)
			{
				withColor = true;
			}

			}
    }

	return true;
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = listContent.find(item->data(Qt::DisplayRole).toString()).value();
	cout << path.toLocal8Bit().data() << endl;
	filePath = new char[path.size()+1];
	filePath[path.size()] = '\0';
	strcpy(filePath, path.toLocal8Bit().data());

	std::thread *t_3D_view = new std::thread([this] { this->showPLY(); });
	std::thread *t_2D_lateral_view = new std::thread([this] { this->showPlane(false); });
	std::thread *t_2D_plan_view = new std::thread([this] { this->showPlane(true); });

	t_3D_view->join();
	t_2D_lateral_view->join();
	t_2D_plan_view->join();
}

void MainWindow::showPlane(bool bPlanView)
{
	if (withColor)
		this->showPlaneColor(bPlanView);
	else
		this->showPlaneNoColor(bPlanView);
}

/*
* Show the 3D Model selected (filePath) - Handling Colors
*/
void MainWindow::showPLY() {
	
	pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
	int v(20);
	pv->setWindowName("3D View");
	pv->setPosition(0, 0);
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


void MainWindow::showPlaneColor(bool bPlanView)
{
	// Point cloud in 2D
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr src_projected(new pcl::PointCloud<pcl::PointXYZRGB>);


	// --------- Copied from ShowPLY()

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr src(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPLYFile<pcl::PointXYZRGB>(filePath, *src);
	// color
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> src_rgb(src);

	// --------------------------------

	// Create a set of planar coefficients with X=0,Y|Z=1|0
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	coefficients->values.resize(4);

	if (!bPlanView)
	{
		// LATERAL (from frontside)
		coefficients->values[0] = coefficients->values[1] = 0;	// X,Y = 0
		coefficients->values[2] = 1.0;							// Z = 1
	}
	else
	{
		// PLAN (from upside)
		coefficients->values[0] = coefficients->values[2] = 0;	// X,Z = 0
		coefficients->values[1] = 1.0;							// Y = 1
	}

	coefficients->values[3] = 0;


	// Create the filtering object
	pcl::ProjectInliers<pcl::PointXYZRGB> proj;
	proj.setModelType(pcl::SACMODEL_PLANE);
	proj.setInputCloud(src);
	proj.setModelCoefficients(coefficients);
	proj.filter(*src_projected);

	// Visualizer
	pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
	int v(20);
	pv->setWindowName(bPlanView ? "2D Plan View" : "2D Lateral View");
	int xy_pos = (bPlanView ? 300 : 150);
	pv->setPosition(xy_pos, xy_pos);
	pv->setShowFPS(true);
	pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);

	// point cloud
	pv->addPointCloud<pcl::PointXYZRGB>(src_projected, src_rgb, "v1_source", v);

	//pv->addPointCloud<pcl::PointXYZ>(src_projected, src_rgb, "v1_source", v);

	// Set camera position if plan view
	if (bPlanView)
		pv->setCameraPosition(0, 5, 0, 1, 0, 0, 0);
	else
		pv->setCameraPosition(0, 0, 5, 0, 1, 0, 0);

	// Show
	pv->spin();
}/*
 * Show the 3D Model selected (filePath) in 2D
 */
void MainWindow::showPlaneNoColor(bool bPlanView)
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


	// Create a set of planar coefficients with X=0,Y|Z=1|0
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	coefficients->values.resize(4);
	
	if (!bPlanView)
	{
		// LATERAL (from frontside)
		coefficients->values[0] = coefficients->values[1] = 0;	// X,Y = 0
		coefficients->values[2] = 1.0;							// Z = 1
	}
	else 
	{
		// PLAN (from upside)
		coefficients->values[0] = coefficients->values[2] = 0;	// X,Z = 0
		coefficients->values[1] = 1.0;							// Y = 1
	}

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
	pv->setWindowName(bPlanView ? "2D Plan View" : "2D Lateral View");
	int xy_pos = (bPlanView ? 300 : 150);
	pv->setPosition(xy_pos, xy_pos);
	pv->setShowFPS(true);
	pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);
	pv->addPointCloud<pcl::PointXYZ>(src_projected, src_rgb, "v1_source", v);
	
	// Set camera position if plan view
	if (bPlanView)		
		pv->setCameraPosition(0, 5, 0,  1, 0, 0,  0);
	else
		pv->setCameraPosition(0, 0, 5,  0, 1, 0,  0);

	// Show
	pv->spin();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
}

void MainWindow::dropEvent(QDropEvent *e)
{
	QString filePath;
	foreach(const QUrl &url, e->mimeData()->urls()) {
		QString fileName = url.toLocalFile();
		QFileInfo importFileInfo(fileName);
		if (QString::compare("import", importFileInfo.suffix(), Qt::CaseSensitive) == 0)
		{
			filePath = fileName;
		}
	}

	QFileInfo importFileInfo(filePath);
	if (readImportFile(filePath))
	{
		list.clear();
		listContent.clear();

		list = detectPlyFiles(importFileInfo.absoluteDir());
		ui->listWidget->clear();
		ui->listWidget->addItems(listContent.keys());
	}
}