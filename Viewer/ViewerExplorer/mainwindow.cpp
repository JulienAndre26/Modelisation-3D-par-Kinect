#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	setAcceptDrops(true);
	ui->progressBar->setVisible(false);

	ui->qvtkWidget3D->setVisible(false);
	ui->qvtkWidgetLateral->setVisible(false);
	ui->qvtkWidgetPlan->setVisible(false);

	ui->gifLoading3D->setVisible(false);
	ui->gifLoadingLateral->setVisible(false);
	ui->gifLoadingPlan->setVisible(false);

	ui->gifInit3D->setAlignment(Qt::AlignCenter);
	ui->gifInitLateral->setAlignment(Qt::AlignCenter);
	ui->gifInitPlan->setAlignment(Qt::AlignCenter);

	ui->gifLoading3D->setAlignment(Qt::AlignCenter);
	ui->gifLoadingLateral->setAlignment(Qt::AlignCenter);
	ui->gifLoadingPlan->setAlignment(Qt::AlignCenter);

	movieInit = new QMovie("box_init.gif");
	ui->gifInit3D->setMovie(movieInit);
	ui->gifInitLateral->setMovie(movieInit);
	ui->gifInitPlan->setMovie(movieInit);
	movieInit->start();

	movieLoading = new QMovie("box_loading.gif");
	ui->gifLoading3D->setMovie(movieLoading);
	ui->gifLoadingLateral->setMovie(movieLoading);
	ui->gifLoadingPlan->setMovie(movieLoading);
	movieLoading->start();

	renderLock = vtkMutexLock::New();
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

		ui->btnMerge->setEnabled(true);
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

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
	ui->btnOpen->setEnabled(true);
	selectedFile = listContent.find(item->data(Qt::DisplayRole).toString()).value();
	cout << selectedFile.toLocal8Bit().data() << endl;
}

void MainWindow::on_btnOpen_clicked()
{
	loadWidgets(selectedFile);
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
	
	loadWidgets(path);
}

void MainWindow::loadWidgets(QString path)
{
	filePath = new char[path.size() + 1];
	filePath[path.size()] = '\0';
	strcpy(filePath, path.toLocal8Bit().data());

	setViewDisplay(VIEW_3D, false);
	setViewDisplay(VIEW_LATERAL, false);
	setViewDisplay(VIEW_PLAN, false);
	
	Thread * t_3D = new Thread(this, VIEW_3D);
	QObject::connect(t_3D, SIGNAL(finished()), t_3D, SLOT(onEnd()));
	t_3D->start();

	Thread * t_Lat = new Thread(this, VIEW_LATERAL);
	QObject::connect(t_Lat, SIGNAL(finished()), t_Lat, SLOT(onEnd()));
	t_Lat->start();

	Thread * t_Plan = new Thread(this, VIEW_PLAN);
	QObject::connect(t_Plan, SIGNAL(finished()), t_Plan, SLOT(onEnd()));
	t_Plan->start();
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
	int v(20);
	pcl::visualization::PCLVisualizer * pv;

	if (withColor)
	{
		pcl::PointCloud<pcl::PointXYZRGB>::Ptr src(new pcl::PointCloud<pcl::PointXYZRGB>);
		pcl::io::loadPLYFile<pcl::PointXYZRGB>(filePath, *src);
		// color
		pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> src_rgb(src);

		// point cloud
		pv = new pcl::visualization::PCLVisualizer("Not the QVTKWidget", false);
		pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);
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
		pv = new pcl::visualization::PCLVisualizer("Not the QVTKWidget", false);
		pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);
		pv->addPointCloud<pcl::PointXYZ>(src, src_rgb, "v1_source", v);
	}

	renderLock->Lock();
	vtkSmartPointer<vtkRenderWindow> renderWindow = pv->getRenderWindow();
	ui->qvtkWidget3D->SetRenderWindow(renderWindow);
	renderLock->Unlock();
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
	pcl::visualization::PCLVisualizer * pv = new pcl::visualization::PCLVisualizer("Not the QVTKWidget", false);
	int v(20);
	pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);
	pv->addPointCloud<pcl::PointXYZRGB>(src_projected, src_rgb, "v1_source", v);

	//pv->addPointCloud<pcl::PointXYZ>(src_projected, src_rgb, "v1_source", v);
	vtkSmartPointer<vtkRenderWindow> renderWindow = pv->getRenderWindow();

	renderLock->Lock();
	// Set camera position if plan view
	if (bPlanView)
	{
		ui->qvtkWidgetPlan->SetRenderWindow(renderWindow);
		vtkSmartPointer<vtkCamera> tmpCam = ui->qvtkWidgetPlan->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
		tmpCam->SetPosition(0, 5, 0);
		tmpCam->SetViewUp(1, 0, 0);
		ui->qvtkWidgetPlan->update();
	}
	else {
		ui->qvtkWidgetLateral->SetRenderWindow(renderWindow);
		vtkSmartPointer<vtkCamera> tmpCam = ui->qvtkWidgetLateral->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
		tmpCam->SetPosition(0, 0, 5);
		ui->qvtkWidgetLateral->update();
	}
	renderLock->Unlock();
}

/*
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
	pcl::visualization::PCLVisualizer * pv = new pcl::visualization::PCLVisualizer("Not the QVTKWidget", false);
	int v(20);
	pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);
	pv->addPointCloud<pcl::PointXYZ>(src_projected, src_rgb, "v1_source", v);


	//pv->addPointCloud<pcl::PointXYZ>(src_projected, src_rgb, "v1_source", v);
	vtkSmartPointer<vtkRenderWindow> renderWindow = pv->getRenderWindow();

	renderLock->Lock();
	// Set camera position if plan view
	if (bPlanView)
	{
		ui->qvtkWidgetPlan->SetRenderWindow(renderWindow);
		vtkSmartPointer<vtkCamera> tmpCam = ui->qvtkWidgetPlan->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
		tmpCam->SetPosition(0, 5, 0);
		tmpCam->SetViewUp(1, 0, 0);
		ui->qvtkWidgetPlan->update();
	}
	else {
		ui->qvtkWidgetLateral->SetRenderWindow(renderWindow);
		vtkSmartPointer<vtkCamera> tmpCam = ui->qvtkWidgetLateral->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
		tmpCam->SetPosition(0, 0, 5);
		ui->qvtkWidgetLateral->update();
	}
	renderLock->Unlock();
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

void MainWindow::setWidgetBorderRadius(QWidget* widget, int radius) {

	// cache widget with and height
	int width = widget->width();
	int height = widget->height();

	// Initialize a rectangular masked region
	QRegion region(0, 0, width, height, QRegion::Rectangle);

	// now clip off the sharp edges

	// top left
	QRegion round(0, 0, 2 * radius, 2 * radius, QRegion::Ellipse);
	QRegion corner(0, 0, radius, radius, QRegion::Rectangle);
	region = region.subtracted(corner.subtracted(round));

	// top right
	round = QRegion(width - 2 * radius, 0, 2 * radius, 2 * radius, QRegion::Ellipse);
	corner = QRegion(width - radius, 0, radius, radius, QRegion::Rectangle);
	region = region.subtracted(corner.subtracted(round));

	// bottom right
	round = QRegion(width - 2 * radius, height - 2 * radius, 2 * radius, 2 * radius, QRegion::Ellipse);
	corner = QRegion(width - radius, height - radius, radius, radius, QRegion::Rectangle);
	region = region.subtracted(corner.subtracted(round));

	// bottom left
	round = QRegion(0, height - 2 * radius, 2 * radius, 2 * radius, QRegion::Ellipse);
	corner = QRegion(0, height - radius, radius, radius, QRegion::Rectangle);
	region = region.subtracted(corner.subtracted(round));

	// Set mask
	widget->setMask(region);
}

void MainWindow::setViewDisplay(int nView, bool bShowWidget)
{
	QVTKWidget * qw;
	QLabel * qlInit;
	QLabel * qlLoading;

	switch (nView)
	{
	case VIEW_3D:
		qw = this->ui->qvtkWidget3D;
		qlInit= this->ui->gifInit3D;
		qlLoading = this->ui->gifLoading3D;
		break;

	case VIEW_LATERAL:
		qw = this->ui->qvtkWidgetLateral;
		qlInit = this->ui->gifInitLateral;
		qlLoading = this->ui->gifLoadingLateral;
		break;

	case VIEW_PLAN:
		qw = this->ui->qvtkWidgetPlan;
		qlInit = this->ui->gifInitPlan;
		qlLoading = this->ui->gifLoadingPlan;
		break;

	default:
		cout << "MainWindow::setViewDisplay : Invalid view number " + nView << endl;
		return;
	}

	movieInit->stop();
	qlInit->setVisible(false);
	qlLoading->setVisible(!bShowWidget);
	qw->setVisible(bShowWidget);

	if (bShowWidget)
		setWidgetBorderRadius(qw, 6);

}

void MainWindow::processView(int nView)
{
	switch (nView)
	{
	case VIEW_3D:
		showPLY();
		break;
	case VIEW_LATERAL:
		showPlane(false);
		break;
	case VIEW_PLAN:
		showPlane(true); 
		break;
	default:
		cout << "MainWindow::processView : Invalid view number " + nView << endl;
	}
}

