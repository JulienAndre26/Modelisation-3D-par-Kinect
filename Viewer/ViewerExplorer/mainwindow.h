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

#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/vtk_io.h> 
#include <pcl/io/vtk_lib_io.h> 

#include <PCLCore.h>
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

#define METER " m"

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
	
	void processLoadThread(MainWindow * mw);
	void processView(pcl::PolygonMesh::Ptr mesh, int nView);
	void processView(PointCloudColored::Ptr cloud, int nView);
	void processMerge();

	void onMerge();
	void onMergeEnd();

	void show3D(PolygonMesh::Ptr mesh);
	void show3D(PointCloudColored::Ptr cloud);
	void show2D(PolygonMesh::Ptr mesh, bool bPlanView);
	void show2D(PointCloudColored::Ptr mesh, bool bPlanView);

	void updateMetrics(int nPoint, float x, float y, float z);
	void updateMetrics(double distance);

	void displayBoxSize();

	void launchOpenThreads(PolygonMesh::Ptr mesh, MainWindow * mw);
	void launchOpenThreads(PointCloudColored::Ptr cloud, MainWindow * mw);

	char * szFilePath;
	vtkMutexLock * vmuRenderLock;
	QMutex qmuReduceLock;

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
	void on_btnResetCamera_clicked();
	void on_btnLeftPlan_clicked();
	void on_btnRightPlan_clicked();

	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
	void on_listWidget_itemClicked(QListWidgetItem *item);

	
private:
    Ui::MainWindow *ui;

	QMovie * qmInit;

	bool bIsMeshColorized = false;
	bool bIsFullscreenActive = false;
	bool bIsWidgetActive = false;
	bool bIsModelLoading = false;

	QDir qdCaptureDirectory;
	QString qsSelectedFile;
	QString qsLoadedFile;
	QStringList qslFilenameList;
	QMap<QString, QString> qmFileMap;

	QThread * qth3D = nullptr;
	QThread * qthLateral = nullptr;
	QThread * qthPlan = nullptr;
	QThread * qthMerge = nullptr;
	QThread * qthLoad = nullptr;

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
	void hideMetrics();

	QLabel * getLabel(int nView);
	QVTKWidget * getWidget(int nView);
	QMovie * getMovie(int nStatus);

	void stopOpenThreads();
	void stopMergeThread();
	void stopThread(QThread * qThread);

	void setFullscreenActive(bool bFullscreen);
	void resetWidgetCamera(QVTKWidget * qw, int posX, int posY, int posZ, int viewX, int viewY, int viewZ);
	void resetWidgetCamera(QVTKWidget * qw, int posX, int posY, int posZ);

	void showHelp();
	void assignIcon(QPushButton* button, std::string& resource);
	void assignIcon(QLabel * label, std::string& resource);
	void updateBoundingBoxLabels(double x, double y, double z);
};

enum CameraPositionPlan {
	xPlan = -10,
	yPlan = 10,
	zPlan = 0
};

enum CameraPositionLateral {
	xLateral = 0,
	yLateral = 0,
	zLateral = 10
};