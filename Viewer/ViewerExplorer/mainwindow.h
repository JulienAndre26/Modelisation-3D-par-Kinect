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
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
	void on_listWidget_itemClicked(QListWidgetItem *item);
	void on_btnAdd_clicked();
	void on_btnDelete_clicked();
	
private:
    Ui::MainWindow *ui;
	
	QMovie * movieInit;
	QMovie * movieLoad;
	QMovie * movieMerge;

	bool hasColor = false;
	QDir captureDirectory;
	QString selectedFile;
	QString loadedFile;
	QStringList list;
	QHash<QString, QString> listContent;

    QStringList detectPlyFiles(QDir dirToImport);
    bool readImportFile(QString import);
	void dropEvent(QDropEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void setWidgetBorderRadius(QWidget* widget, int radius);
	void importFileOpened(QString fileName);
	void updateFileList();
	void setLoadedFile(QString newValue);
	QLabel * getLabel(int nView);
	QVTKWidget * getWidget(int nView);
	QMovie * getMovie(int nStatus);

	QThread * thread3D = nullptr;
	QThread * threadLateral = nullptr;
	QThread * threadPlan = nullptr;
	QThread * threadMerge = nullptr;

	void stopOpenThreads();
	void stopMergeThread();
	void stopThread(QThread * qThread);

	void reduceFile(std::string file);
};