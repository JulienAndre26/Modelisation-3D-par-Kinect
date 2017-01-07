#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "custom_qthreads.h"
#include "metric_visualizer.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	/* Manual init of UI */

	// Drag'N'Drop
	setAcceptDrops(true);

	// Visibility
	ui->progressBar->setVisible(false);
	ui->qvtkWidget3D->setVisible(false);
	ui->qvtkWidgetLateral->setVisible(false);
	ui->qvtkWidgetPlan->setVisible(false);
	ui->btnAdd->setVisible(false);
	ui->btnDelete->setVisible(false);
	ui->lbP1x->setVisible(false);
	ui->lbP1y->setVisible(false);
	ui->lbP1z->setVisible(false);
	ui->lbP2x->setVisible(false);
	ui->lbP2y->setVisible(false);
	ui->lbP2z->setVisible(false);
	ui->lbDistance->setVisible(false);
	ui->lbPin1->setVisible(false);
	ui->lbPin2->setVisible(false);
	ui->lbDistanceIcon->setVisible(false);

	// Alignements
	ui->gif3D->setAlignment(Qt::AlignCenter);
	ui->gifLateral->setAlignment(Qt::AlignCenter);
	ui->gifPlan->setAlignment(Qt::AlignCenter);

	// Icons
	QPixmap pixAdd(":/icons/add");
	QIcon iconAdd(pixAdd);
	ui->btnAdd->setIcon(iconAdd);
	ui->btnAdd->setIconSize(pixAdd.rect().size());

	QPixmap pixDel(":/icons/delete");
	QIcon iconDel(pixDel);
	ui->btnDelete->setIcon(iconDel);
	ui->btnDelete->setIconSize(pixDel.rect().size());

	QPixmap pixPin(":/icons/pin");
	ui->lbPin1->setPixmap(pixPin);
	ui->lbPin2->setPixmap(pixPin);

	QPixmap pixDist(":/icons/distance");
	ui->lbDistanceIcon->setPixmap(pixDist);

	QPixmap pixHelp(":/icons/help");
	QIcon iconHelp(pixHelp);
	ui->btnHelp->setIcon(iconHelp);
	ui->btnHelp->setIconSize(pixHelp.rect().size());

	// Gifs
	movieInit = new QMovie(":/gifs/init");
	movieLoad = new QMovie(":/gifs/load");
	movieMerge = new QMovie(":/gifs/merge");

	ui->gif3D->setMovie(movieInit);
	ui->gifLateral->setMovie(movieInit);
	ui->gifPlan->setMovie(movieInit);
	movieInit->start();

	// Mutex
	renderLock = vtkMutexLock::New();

	// Variables
	setLoadedFile("");
	ui->lbLoadedFile->setText("Please browse a capture and open a file");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    QString importName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Import files (*.import)"));

	if (!importName.isEmpty())
		importFileOpened(importName);
	    
}

void MainWindow::importFileOpened(QString fileName)
{
	QFileInfo importFileInfo(fileName);
	if (readImportFile(fileName))
	{
		stopOpenThreads();
		stopMergeThread();

		captureDirectory = importFileInfo.absoluteDir();
		updateFileList();
		ui->lbLoadedFile->setText("Please open a file");
	}
}

void MainWindow::updateFileList()
{
	list.clear();
	listContent.clear();

	list = detectPlyFiles(captureDirectory);
	ui->listWidget->clear();
	ui->listWidget->addItems(listContent.keys());

	ui->btnAdd->setVisible(true);
	ui->btnDelete->setVisible(true);

	ui->progressBar->setVisible(false);
	ui->lbCurrentMerge->setText("");

	ui->btnMerge->setEnabled(list.size() > 1);
	ui->btnShow->setEnabled(false);

	setAllViewDisplay(false, MOVIE_INIT);

	modelLoading = false;
	selectedFile = "";
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
	onMerge();
}

void MainWindow::onMerge()
{
	if (list.size() < 2)
	{
		QMessageBox::warning(this, "Empty list", "Please select .import file with at least 2 models");
	}
	else
	{
		ui->progressBar->setVisible(true);
		ui->progressBar->setValue(0);

		ui->progressBar->setMaximum(0);
		ui->progressBar->setMinimum(0);

		if (!modelLoading)
		{
			setAllViewDisplay(false, MOVIE_MERGE);
		}
		

		threadMerge = new ThreadMerge(this);
		QObject::connect(threadMerge, SIGNAL(finished()), threadMerge, SLOT(onEnd()));
		threadMerge->start();
	}
}

void MainWindow::processMerge()
{
	QString sTotal = QString::number(list.size() - 1);
	for (int i = 0; i < list.size() - 1; i++)
	{
		QString sCurrent = QString::number(i + 1);
		ui->lbCurrentMerge->setText("Merging... (" + sCurrent + "/" + (sTotal) + ")");
		
		//ui->progressBar->setValue(((float)n / (list.size() - 1) * 100));
		
		QThread::sleep(5);
	}
}

void MainWindow::onMergeEnd()
{
	ui->progressBar->setMaximum(100);
	ui->progressBar->setValue(100);
	ui->lbCurrentMerge->setText("Finished");

	if (!modelLoading)
	{
		setAllViewDisplay(false, MOVIE_INIT);
	}
	
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
	ui->btnShow->setEnabled(true);
	selectedFile = listContent.find(item->data(Qt::DisplayRole).toString()).value();
	cout << selectedFile.toLocal8Bit().data() << endl;

	if (QString::compare(selectedFile, loadedFile, Qt::CaseInsensitive) == 0)
		ui->btnShow->setEnabled(false);
	else
		ui->btnShow->setEnabled(true);
}

void MainWindow::on_btnShow_clicked()
{
	onLoad(selectedFile);
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
			hasColor = false;
			stream.seek(0);
			QString path = stream.readLine().split("\\").last();
			ui->lbCaptureName->setText("<b>" + path + "<\\b>");
			stream.readLine();
			stream.readLine();
			QString color = stream.readLine().split(" ").last();

			int cmp = QString::compare("true", color, Qt::CaseInsensitive);
			if (cmp == 0)
			{
				hasColor = true;
			}

		}
    }

	return true;
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = listContent.find(item->data(Qt::DisplayRole).toString()).value();
	cout << path.toLocal8Bit().data() << endl;
	
	onLoad(path);
}

void MainWindow::onLoad(QString path)
{
	if (QString::compare(path, loadedFile, Qt::CaseInsensitive) == 0)
		return;

	stopOpenThreads();

	setLoadedFile(path);

	filePath = new char[path.size() + 1];
	filePath[path.size()] = '\0';
	strcpy(filePath, path.toLocal8Bit().data());

	setAllViewDisplay(false, MOVIE_LOAD);

	modelLoading = true;

	thread3D = new ThreadOpen(this, VIEW_3D);
	QObject::connect(thread3D, SIGNAL(finished()), thread3D, SLOT(onEnd()));
	thread3D->start();

	threadLateral = new ThreadOpen(this, VIEW_LATERAL);
	QObject::connect(threadLateral, SIGNAL(finished()), threadLateral, SLOT(onEnd()));
	threadLateral->start();

	threadPlan = new ThreadOpen(this, VIEW_PLAN);
	QObject::connect(threadPlan, SIGNAL(finished()), threadPlan, SLOT(onEnd()));
	threadPlan->start();
}

void MainWindow::showPlane(bool bPlanView)
{
	// Point cloud in 2D
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr src_projected(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr src(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPLYFile<pcl::PointXYZRGB>(filePath, *src);

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
	MetricVisualizer * pv = new MetricVisualizer(src_projected, hasColor, this);
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
* Show the 3D Model selected (filePath) - Handling Colors
*/
void MainWindow::showPLY() {

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr src(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPLYFile<pcl::PointXYZRGB>(filePath, *src);
	MetricVisualizer * pv = new MetricVisualizer(src, hasColor, this);

	renderLock->Lock();
	vtkSmartPointer<vtkRenderWindow> renderWindow = pv->getRenderWindow();
	ui->qvtkWidget3D->SetRenderWindow(renderWindow);
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

	importFileOpened(filePath);
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

void MainWindow::setViewDisplay(int nView, bool bShowWidget, int nStatus)
{
	QVTKWidget * qw;
	QLabel * ql;
	QMovie * qm;
	
	// Getting Qt elements to set
	switch (nView)
	{
	case VIEW_3D:
		qw = this->ui->qvtkWidget3D;
		ql= this->ui->gif3D;
		break;

	case VIEW_LATERAL:
		qw = this->ui->qvtkWidgetLateral;
		ql = this->ui->gifLateral;
		break;

	case VIEW_PLAN:
		qw = this->ui->qvtkWidgetPlan;
		ql = this->ui->gifPlan;
		break;

	default:
		cout << "MainWindow::setViewDisplay : Invalid view number " + nView << endl;
		return;
	}

	// Stopping previous movie
	ql->movie()->stop();

	// Set the new movie
	if (nStatus != NULL)
	{
		switch (nStatus) {
		case MOVIE_INIT:
			qm = movieInit;
			break;
		case MOVIE_LOAD:
			qm = movieLoad;
			break;
		case MOVIE_MERGE:
			qm = movieMerge;
			break;
		default:
			qm = nullptr;
		}

		ql->setMovie(qm);
		qm->start();
	}

	ql->setVisible(!bShowWidget);
	qw->setVisible(bShowWidget);

	if (bShowWidget)
		setWidgetBorderRadius(qw, 6);

}

void MainWindow::setAllViewDisplay(bool bShowWidget, int nStatus)
{
	setViewDisplay(VIEW_3D, bShowWidget, nStatus);
	setViewDisplay(VIEW_LATERAL, bShowWidget, nStatus);
	setViewDisplay(VIEW_PLAN, bShowWidget, nStatus);
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

void MainWindow::on_btnAdd_clicked()
{
	QString fileToAdd = QFileDialog::getOpenFileName(this, tr("Add 3D File"), QDir::homePath(), tr("3D files (*.ply)"));

	QFileInfo infoToAdd(fileToAdd);
	QFileInfo infoDir(captureDirectory.absolutePath());

	cout << "Selected : " << infoToAdd.absoluteFilePath().toLocal8Bit().data() << endl;
	
	QString finalPath = infoDir.absoluteFilePath() + "/" + infoToAdd.baseName() + ".ply";

	cout << "Final Path : " << finalPath.toLocal8Bit().data() << endl;

	QFile::copy(fileToAdd, finalPath);

	updateFileList();
}

void MainWindow::on_btnDelete_clicked()
{
	if (!selectedFile.isEmpty())
	{
		QFileInfo infoToDel(selectedFile);
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, "Warning", "Are you sure you want to delete " + infoToDel.baseName() + " ?",
			QMessageBox::Yes | QMessageBox::No);
		
		if (reply == QMessageBox::Yes) {
			QFile::remove(selectedFile);
			selectedFile = "";
			updateFileList();
		}		
	}
}

void MainWindow::updateMetrics(int nPoint, float x, float y, float z)
{
	QLabel *qlbX, *qlbY, *qlbZ, *qlPin;
	
	switch (nPoint)
	{
	case METRIC_P1:
		qlbX = ui->lbP1x;
		qlbY = ui->lbP1y;
		qlbZ = ui->lbP1z;
		qlPin = ui->lbPin1;
		
		ui->lbP2x->setVisible(false);
		ui->lbP2y->setVisible(false);
		ui->lbP2z->setVisible(false);
		ui->lbPin2->setVisible(false);
		ui->lbDistance->setVisible(false);
		ui->lbDistanceIcon->setVisible(false);
		break;

	case METRIC_P2:
		qlbX = ui->lbP2x;
		qlbY = ui->lbP2y;
		qlbZ = ui->lbP2z;
		qlPin = ui->lbPin2;
		break;

	default:
		return;
	}

	qlbX->setText("X: " + QString::number(x));
	qlbX->setVisible(true);

	qlbY->setText("Y: " + QString::number(y));
	qlbY->setVisible(true);
	
	qlbZ->setText("Z: " + QString::number(z));
	qlbZ->setVisible(true);

	qlPin->setVisible(true);
}

void MainWindow::updateMetrics(double distance)
{
	char szDistanceM[8];
	char szDistanceCM[8];

	sprintf(szDistanceM, "%.2f", distance);
	sprintf(szDistanceCM, "%.2f", distance*100);

	ui->lbDistance->setText("Distance: " + QString(szDistanceM) + " m (" + QString(szDistanceCM) + " cm)");
	ui->lbDistance->setVisible(true);
	ui->lbDistanceIcon->setVisible(true);	
}

void MainWindow::setLoadedFile(QString newValue)
{
	loadedFile = newValue;
	QFileInfo qfi(newValue);
	ui->lbLoadedFile->setText(qfi.baseName());
}

void MainWindow::stopOpenThreads()
{
	stopThread(thread3D);
	stopThread(threadLateral);
	stopThread(threadPlan);
}

void MainWindow::stopMergeThread()
{
	stopThread(threadMerge);
}

void MainWindow::stopThread(QThread * qThread)
{
	if (qThread == nullptr)
		return;

	if (qThread->isRunning())
	{
		qThread->terminate();
		qThread->wait();
		qThread = nullptr;
	}
}	
