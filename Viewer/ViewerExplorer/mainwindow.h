#pragma once

// This need to be placed before each VTK include
#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkMutexLock.h>
#include <vtkCamera.h>

#include <iostream>

#include <QMainWindow>
#include <QFileDialog>
#include <QHash>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTextStream>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMovie>
#include <QtGui>
#include <QThread.h>

#include <PCLCore.h>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>
#include <pcl/io/ply_io.h>
#include <pcl/visualization/pcl_visualizer.h>

#include "IOXML.h"
#include "IOPLY.h"


#define VIEW_3D			1
#define VIEW_LATERAL	2
#define VIEW_PLAN		3

#define MOVIE_INIT		1
#define MOVIE_LOAD		2
#define MOVIE_MERGE		3

#define METRIC_P1		1
#define METRIC_P2		2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void onLoad(QString path);

	void setViewDisplay(int nView, bool bShowWidget, int nStatus);
	void setAllViewDisplay(bool bShowWidget, int nStatus);
	
	void processView(int nView);
	void processMerge();

	void onMerge();
	void onMergeEnd();

	void showPLY();
	void showPlane(bool bPlanView);

	void updateMetrics(int nPoint, float x, float y, float z);
	void updateMetrics(double distance);
		
	char *filePath;
	vtkMutexLock *renderLock;
	QMutex reduceLock;

	bool modelLoading = false;

private slots:
    void on_btnBrowse_clicked();
    void on_btnMerge_clicked();
	void on_btnShow_clicked();
	void on_btnHelpR_clicked();
	void on_btnHelpM_clicked();
	void on_btnAdd_clicked();
	void on_btnDelete_clicked();
	void on_btnFS_clicked();
	void on_btnExitFS_clicked();

	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
	void on_listWidget_itemClicked(QListWidgetItem *item);

	
private:
    Ui::MainWindow *ui;
	
	QMovie * movieInit;
	QMovie * movieLoad;
	QMovie * movieMerge;

	bool hasColor = false;
	bool isFullscreenActive = false;
	bool isWidgetActive = false;

	QDir captureDirectory;
	QString selectedFile;
	QString loadedFile;
	QStringList list;
	QHash<QString, QString> listContent;

	QThread * thread3D = nullptr;
	QThread * threadLateral = nullptr;
	QThread * threadPlan = nullptr;
	QThread * threadMerge = nullptr;

	void dropEvent(QDropEvent *e);
	void resizeEvent(QResizeEvent* event);
	void dragEnterEvent(QDragEnterEvent *e);

	QStringList detectPlyFiles(QDir dirToImport);
	bool readImportFile(QString import);
	void importFileOpened(QString fileName);
	void updateFileList();
	void setLoadedFile(QString newValue);
	void setWidgetBorderRadius(QWidget* widget, int radius);
	void reduceFile(std::string file);

	QLabel * getLabel(int nView);
	QVTKWidget * getWidget(int nView);
	QMovie * getMovie(int nStatus);

	void stopOpenThreads();
	void stopMergeThread();
	void stopThread(QThread * qThread);

	void setFullscreenActive(bool bFullscreen);

	void showHelp();
};