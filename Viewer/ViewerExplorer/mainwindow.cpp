#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "custom_qthreads.h"
#include "metric_visualizer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	setWindowIcon(QIcon(":/icons/app"));

	setWindowFlags(windowFlags() | Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint /*| Qt::Window | Qt::FramelessWindowHint*/);

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
	ui->btnExitFS->setVisible(false);
	ui->btnHelpM->setVisible(false);

	// Alignements
	ui->gif3D->setAlignment(Qt::AlignCenter);
	ui->gifLateral->setAlignment(Qt::AlignCenter);
	ui->gifPlan->setAlignment(Qt::AlignCenter);

	// Icons
	QPixmap pixLeftArrow(":/icons/leftarrow");
	QIcon iconLeftArrow(pixLeftArrow);
	ui->btnLeftPlan->setIcon(iconLeftArrow);
	ui->btnLeftPlan->setIconSize(pixLeftArrow.rect().size());

	QPixmap pixRightArrow(":/icons/rightarrow");
	QIcon iconRightArrow(pixRightArrow);
	ui->btnRightPlan->setIcon(iconRightArrow);
	ui->btnRightPlan->setIconSize(pixRightArrow.rect().size());

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
	ui->btnHelpR->setIcon(iconHelp);
	ui->btnHelpR->setIconSize(pixHelp.rect().size());
	ui->btnHelpM->setIcon(iconHelp);
	ui->btnHelpM->setIconSize(pixHelp.rect().size());

	QPixmap pixFS(":/icons/fs");
	QIcon iconFS(pixFS);
	ui->btnFS->setIcon(iconFS);
	ui->btnFS->setIconSize(pixFS.rect().size());

	QPixmap pixEFS(":/icons/exit_fs");
	QIcon iconEFS(pixEFS);
	ui->btnExitFS->setIcon(iconEFS);
	ui->btnExitFS->setIconSize(pixEFS.rect().size());

	QPixmap pixResCam(":/icons/reset_cam");
	QIcon iconResCam(pixResCam);
	ui->btnResetCamera->setIcon(iconResCam);
	ui->btnResetCamera->setIconSize(pixResCam.rect().size());

	ui->btnBrowse->setIcon(QIcon(":/icons/browse"));
	ui->btnShow->setIcon(QIcon(":/icons/show"));
	ui->btnMerge->setIcon(QIcon(":/icons/merge"));

	// Gifs
	qmInit = new QMovie(":/gifs/init");
	/*movieLoad = new QMovie(":/gifs/load"); These gifs must not be a single instance
	movieMerge = new QMovie(":/gifs/merge");*/

	ui->gif3D->setMovie(qmInit);
	ui->gifLateral->setMovie(qmInit);
	ui->gifPlan->setMovie(qmInit);
	qmInit->start();

	// Mutex
	vmuRenderLock = vtkMutexLock::New();
	
	// Variables
	setLoadedFile("");
	ui->lbLoadedFile->setText("Please browse a capture and open a file");
	
	showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);

	int nWidthValue = rect().width() / 4;

	ui->qvtkWidgetLateral->setMaximumWidth(nWidthValue);
	ui->qvtkWidgetPlan->setMaximumWidth(nWidthValue);
	
	ui->gifLateral->setMaximumWidth(nWidthValue);
	ui->gifPlan->setMaximumWidth(nWidthValue);

	setWidgetBorderRadius(ui->qvtkWidget3D, 6);
	setWidgetBorderRadius(ui->qvtkWidgetLateral, 6);
	setWidgetBorderRadius(ui->qvtkWidgetPlan, 6);
}


void MainWindow::on_btnLeftPlan_clicked()
{
	int index = qslFilenameList.indexOf(ui->lbLoadedFile->text() + ".ply") -1;	
	index = (index < 0) ? qslFilenameList.size()-1 : index;
	QString path = qslFilenameList.at(index);
	onLoad(qmFileMap.value(path));
}

void MainWindow::on_btnRightPlan_clicked()
{
	int index = (qslFilenameList.indexOf(ui->lbLoadedFile->text() + ".ply")+1)% qslFilenameList.size();
	QString path = qslFilenameList.at(index);
	onLoad(qmFileMap.value(path));
}

void MainWindow::on_btnHelpR_clicked()
{
	showHelp();
}

void MainWindow::on_btnHelpM_clicked()
{
	showHelp();
}

void MainWindow::on_btnResetCamera_clicked()
{
	if (bIsWidgetActive)
	{
		resetWidgetCamera(ui->qvtkWidgetPlan, xPlan, yPlan, zPlan, 1, 0, 0);
		resetWidgetCamera(ui->qvtkWidgetLateral, xLateral, yLateral, zLateral);
	}
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

		qdCaptureDirectory = importFileInfo.absoluteDir();
		ui->lbCaptureName->setText("<b>" + qdCaptureDirectory.dirName() + "</b>");

		updateFileList();
		
		setLoadedFile("");
		ui->lbLoadedFile->setText("Please open a file");
		ui->btnResetCamera->setEnabled(false);

		bIsWidgetActive = false;
		setFullscreenActive(this->bIsFullscreenActive);
	}
}

void MainWindow::updateFileList()
{
	qslFilenameList.clear();
	qmFileMap.clear();

	qslFilenameList = detectPlyFiles(qdCaptureDirectory);
	ui->listWidget->clear();
	ui->listWidget->addItems(qmFileMap.keys());

	QFont font("Nirmala UI");
	font.setStyleHint(QFont::Monospace);
	font.setPointSize(8);
	font.setFixedPitch(true);

	QIcon fileIcon(":icons/file");
	for (int i = 0; i < ui->listWidget->count(); ++i)
	{
		QListWidgetItem* item = ui->listWidget->item(i);
		item->setIcon(fileIcon);
		item->setFont(font);
	}

	ui->btnAdd->setVisible(true);
	ui->btnDelete->setVisible(true);

	ui->progressBar->setVisible(false);
	ui->lbCurrentMerge->setText("");

	ui->btnMerge->setEnabled(qslFilenameList.size() > 1);
	ui->btnShow->setEnabled(false);

	setAllViewDisplay(false, MOVIE_INIT);

	bIsModelLoading = false;
	qsSelectedFile = "";
}

QStringList MainWindow::detectPlyFiles(QDir dirToImport)
{
    dirToImport.setNameFilters(QStringList()<<"*.ply");
    QFileInfoList listInfo = dirToImport.entryInfoList();

    QStringList res;

    for (int i = 0; i < listInfo.size(); i++)
    {
		qmFileMap.insert(listInfo.at(i).fileName(), listInfo.at(i).absoluteFilePath());
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
	if (qslFilenameList.size() < 2)
	{
		QMessageBox::warning(this, "Empty list", "Please select .import file with at least 2 models");
	}
	else
	{
		ui->progressBar->setVisible(true);
		ui->progressBar->setValue(0);

		ui->progressBar->setMaximum(0);
		ui->progressBar->setMinimum(0);

		if (!bIsModelLoading)
		{
			setAllViewDisplay(false, MOVIE_MERGE);
		}
		

		qthMerge = new ThreadMerge(this);
		QObject::connect(qthMerge, SIGNAL(finished()), qthMerge, SLOT(onEnd()));
		qthMerge->start();
	}
}

void MainWindow::processMerge()
{
	QString sTotal = QString::number(qslFilenameList.size() - 1);
	for (int i = 0; i < qslFilenameList.size() - 1; i++)
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

	if (!bIsModelLoading)
	{
		setAllViewDisplay(false, MOVIE_INIT);
	}
	
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
	qsSelectedFile = qmFileMap.find(item->data(Qt::DisplayRole).toString()).value();
	cout << qsSelectedFile.toLocal8Bit().data() << endl;

	ui->btnShow->setEnabled(!(QString::compare(qsSelectedFile, qsLoadedFile, Qt::CaseInsensitive) == 0));
}

void MainWindow::on_btnShow_clicked()
{
	onLoad(qsSelectedFile);
}

bool MainWindow::readImportFile(QString import)
{
    QFile importFile(import);
    
	if (!importFile.exists()) {
		QMessageBox::critical(this, "Bad file import", "Please select a correct import file");
		return false;
	}

	IOXML * parser = IOXML::Instance();
	parser->init(import.toLocal8Bit().data());

	if (parser->get("color").compare("ERROR") == 0) {
		QMessageBox::critical(this, "Bad file import", "Please select a correct import file");
		return false;
	}

	bIsMeshColorized = (parser->get("color").compare("true") == 0);

	return true;
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = qmFileMap.find(item->data(Qt::DisplayRole).toString()).value();
	cout << path.toLocal8Bit().data() << endl;
	
	onLoad(path);
}

void MainWindow::onLoad(QString path)
{
	if (ui->lbLoadedFile->text().compare("Please open a file", Qt::CaseInsensitive) == 0) {
		ui->btnLeftPlan->setEnabled(qslFilenameList.size() > 1);
		ui->btnRightPlan->setEnabled(qslFilenameList.size() > 1);
	}

	if (QString::compare(path, qsLoadedFile, Qt::CaseInsensitive) == 0)
		return;

	setLoadedFile(path);
	ui->btnResetCamera->setEnabled(true);

	szFilePath = new char[path.size() + 1];
	szFilePath[path.size()] = '\0';
	strcpy(szFilePath, path.toLocal8Bit().data());

	setAllViewDisplay(false, MOVIE_LOAD);

	bIsModelLoading = true;

	stopOpenThreads();

	qthLoad = new ThreadLoad(this);
	QObject::connect(qthLoad, SIGNAL(finished()), qthLoad, SLOT(onEnd()));
	qthLoad->start();
}

void MainWindow::processLoadThread(MainWindow * mw)
{
	cout << "LOADING MESH..." << endl;
	pcl::PolygonMesh::Ptr mesh(new PolygonMesh());
	IOPLY::load(szFilePath, mesh);

	launchOpenThreads(mesh, mw);

	if (qth3D->isRunning())
	{
		qth3D->wait();
		qth3D = nullptr;
	}

	if (qthLateral->isRunning())
	{
		qthLateral->wait();
		qthLateral = nullptr;
	}

	if (qthPlan->isRunning())
	{
		qthPlan->wait();
		qthPlan = nullptr;
	}
}

void MainWindow::launchOpenThreads(pcl::PolygonMesh::Ptr mesh, MainWindow * mw) {
	qth3D = new ThreadOpen(mw, mesh, VIEW_3D);
	QObject::connect(qth3D, SIGNAL(finished()), qth3D, SLOT(onEnd()));
	qth3D->start();

	qthLateral = new ThreadOpen(mw, mesh, VIEW_LATERAL);
	QObject::connect(qthLateral, SIGNAL(finished()), qthLateral, SLOT(onEnd()));
	qthLateral->start();

	qthPlan = new ThreadOpen(mw, mesh, VIEW_PLAN);
	QObject::connect(qthPlan, SIGNAL(finished()), qthPlan, SLOT(onEnd()));
	qthPlan->start();
}

void MainWindow::show2D(pcl::PolygonMesh::Ptr mesh, bool bPlanView)
{
	MetricVisualizer * pv = new MetricVisualizer(mesh, this);

	cout << "showPlane - MetricVisulizer ready" << endl;

	vmuRenderLock->Lock();
	vtkSmartPointer<vtkRenderWindow> renderWindow = pv->getRenderWindow();
	// Set camera position if plan view
	if (bPlanView)
	{
		ui->qvtkWidgetPlan->SetRenderWindow(renderWindow);
		resetWidgetCamera(ui->qvtkWidgetPlan, xPlan, yPlan, zPlan, 1, 0, 0);
	}
	else {
		ui->qvtkWidgetLateral->SetRenderWindow(renderWindow);
		resetWidgetCamera(ui->qvtkWidgetLateral, xLateral, yLateral, zLateral);
	}
	vmuRenderLock->Unlock();
}

/*
* Show the 3D Model selected (filePath) - Handling Colors
*/

void MainWindow::show3D(pcl::PolygonMesh::Ptr mesh) {
	
	MetricVisualizer * pv = new MetricVisualizer(mesh, this);
	
	cout << "showPly - MetricVisulizer ready" << endl;

	vmuRenderLock->Lock();
	vtkSmartPointer<vtkRenderWindow> renderWindow = pv->getRenderWindow();
	ui->qvtkWidget3D->SetRenderWindow(renderWindow);
	vmuRenderLock->Unlock();
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

	QCoreApplication::processEvents();

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
	QVTKWidget * qw = getWidget(nView);
	QLabel * ql = getLabel(nView);
	QMovie * qm = getMovie(nStatus);
	
	// Stopping previous movie
	if (ql != nullptr)
		ql->movie()->stop();

	if (qm != nullptr) 
	{
		ql->setMovie(qm);
		qm->start();
	}

	bool bDisplay = nView == VIEW_3D || ((nView == VIEW_LATERAL || nView == VIEW_PLAN) && !this->bIsFullscreenActive);
	if (ql != nullptr && bDisplay)
	{
		ql->setVisible(!bShowWidget);
		ql->repaint();
	}

	if (qw != nullptr && bIsWidgetActive && bDisplay)
	{
		qw->setVisible(bShowWidget);
		if (bShowWidget)
			setWidgetBorderRadius(qw, 6);
	}
}

QMovie * MainWindow::getMovie(int nStatus)
{
	QMovie * qm;
	 
	// Set the new movie
	switch (nStatus) {
	case MOVIE_INIT:
		qm = qmInit;
		break;
	case MOVIE_LOAD:
		//qm = movieLoad;
		qm = new QMovie(":/gifs/load");
		break;
	case MOVIE_MERGE:
		qm = new QMovie(":/gifs/load");
		break;
	default:
		qm = nullptr;
	}

	return qm;
}

QVTKWidget * MainWindow::getWidget(int nView)
{
	QVTKWidget * qw;

	// Getting Qt elements to set
	switch (nView)
	{
	case VIEW_3D:
		qw = this->ui->qvtkWidget3D;
		break;

	case VIEW_LATERAL:
		qw = this->ui->qvtkWidgetLateral;
		break;

	case VIEW_PLAN:
		qw = this->ui->qvtkWidgetPlan;
		break;

	default:
		cout << "MainWindow::setViewDisplay : Invalid view number " + nView << endl;
		qw = nullptr;
	}

	return qw;
}

QLabel * MainWindow::getLabel(int nView)
{
	QLabel * ql;

	// Getting Qt elements to set
	switch (nView)
	{
	case VIEW_3D:
		ql = this->ui->gif3D;
		break;

	case VIEW_LATERAL:
		ql = this->ui->gifLateral;
		break;

	case VIEW_PLAN:
		ql = this->ui->gifPlan;
		break;

	default:
		cout << "MainWindow::setViewDisplay : Invalid view number " + nView << endl;
		ql = nullptr;
		//qw = nullptr;
	}

	return ql;
}

void MainWindow::setAllViewDisplay(bool bShowWidget, int nStatus)
{
	setViewDisplay(VIEW_3D, bShowWidget, nStatus);
	setViewDisplay(VIEW_LATERAL, bShowWidget, nStatus);
	setViewDisplay(VIEW_PLAN, bShowWidget, nStatus);
}

void MainWindow::processView(pcl::PolygonMesh::Ptr mesh, int nView)
{
	bIsWidgetActive = false;

	switch (nView)
	{
	case VIEW_3D:
		show3D(mesh);
		break;
	case VIEW_LATERAL:
		show2D(mesh, false);
		break;
	case VIEW_PLAN:
		show2D(mesh, true); 
		break;
	default:
		cout << "MainWindow::processView : Invalid view number " + nView << endl;
	}

	bIsWidgetActive = true;
}

void MainWindow::on_btnAdd_clicked()
{
	QString fileToAdd = QFileDialog::getOpenFileName(this, tr("Add 3D File"), QDir::homePath(), tr("3D files (*.ply)"));

	QFileInfo infoToAdd(fileToAdd);
	QFileInfo infoDir(qdCaptureDirectory.absolutePath());

	cout << "Selected : " << infoToAdd.absoluteFilePath().toLocal8Bit().data() << endl;
	
	QString finalPath = infoDir.absoluteFilePath() + "/" + infoToAdd.baseName() + ".ply";

	cout << "Final Path : " << finalPath.toLocal8Bit().data() << endl;

	QFile::copy(fileToAdd, finalPath);

	updateFileList();
}

void MainWindow::on_btnDelete_clicked()
{
	if (!qsSelectedFile.isEmpty())
	{
		QFileInfo infoToDel(qsSelectedFile);
		QMessageBox::StandardButton reply;
		reply = QMessageBox::warning(this, "Warning", "Are you sure you want to delete " + infoToDel.baseName() + " ?",
			QMessageBox::Yes | QMessageBox::No);
		
		if (reply == QMessageBox::Yes) {
			QFile::remove(qsSelectedFile);
			qsSelectedFile = "";
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
	qsLoadedFile = newValue;
	QFileInfo qfi(newValue);
	ui->lbLoadedFile->setText(qfi.baseName());
}

void MainWindow::stopOpenThreads()
{
	stopThread(qth3D);
	stopThread(qthLateral);
	stopThread(qthPlan);
	stopThread(qthLoad);
}

void MainWindow::stopMergeThread()
{
	stopThread(qthMerge);
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

void MainWindow::reduceFile(std::string file)
{
	ThreadReduce * threadReduce = new ThreadReduce(file, &qmuReduceLock);
	QObject::connect(threadReduce, SIGNAL(finished()), threadReduce, SLOT(onEnd()));
	threadReduce->start();
}

void MainWindow::on_btnFS_clicked() 
{
	setFullscreenActive(true);
}

void MainWindow::on_btnExitFS_clicked() 
{
	setFullscreenActive(false);
}

void MainWindow::setFullscreenActive(bool bFullscreen)
{
	cout << "setFullscreen(" << bFullscreen << ") isWidgetActive = " << bIsWidgetActive << endl;

	ui->btnFS->setVisible(!bFullscreen);
	ui->lbInvisible->setVisible(!bFullscreen);
	ui->btnHelpR->setVisible(!bFullscreen);
	ui->btnResetCamera->setVisible(!bFullscreen);

	bool bDisplayGifs = !bIsWidgetActive && !bFullscreen;
	ui->gifLateral->setVisible(bDisplayGifs);
	ui->gifPlan->setVisible(bDisplayGifs);

	bool bDisplayWidgets = bIsWidgetActive && !bFullscreen;
	ui->qvtkWidgetLateral->setVisible(bDisplayWidgets);
	ui->qvtkWidgetPlan->setVisible(bDisplayWidgets);
	
	ui->btnExitFS->setVisible(bFullscreen);
	ui->btnHelpM->setVisible(bFullscreen);

	if (bDisplayWidgets)
	{
		setWidgetBorderRadius(ui->qvtkWidgetLateral, 6);
		setWidgetBorderRadius(ui->qvtkWidgetPlan, 6);
	}
	setWidgetBorderRadius(ui->qvtkWidget3D, 6);

	this->bIsFullscreenActive = bFullscreen;
}

void MainWindow::showHelp()
{
	QMessageBox::about(this, "Handling 3D model", "<p><b>Handling 3D model</b></p><p><img src = ':/icons/m_scroll'> : Zoom or unzoom the model</p><p><img src = ':/icons/m_left'> + <img src = ':/icons/m_move'> : Rotates the model</p><p><img src = ':/icons/k_shift'> + <img src = ':/icons/m_left'> + <img src = ':/icons/m_move'> : Moves the model</p><p><img src = ':/icons/k_ctrl'> + <img src = ':/icons/m_left'> + <img src = ':/icons/m_move'> : Rotates the model</p><p><img src = ':/icons/k_shift'> + <img src = ':/icons/m_left'> : Puts a pin for measurement (place two pins to get the distance between pins)</p>");
}

void MainWindow::resetWidgetCamera(QVTKWidget * qw, int posX, int posY, int posZ, int viewX, int viewY, int viewZ)
{
	vtkSmartPointer<vtkCamera> tmpCam = qw->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
	tmpCam->SetPosition(posX, posY, posZ);
	tmpCam->SetViewUp(viewX, viewY, viewZ);
	qw->update();
}

void MainWindow::resetWidgetCamera(QVTKWidget * qw, int posX, int posY, int posZ)
{
	vtkSmartPointer<vtkCamera> tmpCam = qw->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
	tmpCam->SetPosition(posX, posY, posZ);
	qw->update();
}