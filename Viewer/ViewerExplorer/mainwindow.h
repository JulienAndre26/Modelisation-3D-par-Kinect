#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// ------ This need to be placed before each VTK include
#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkMutexLock.h>
#include <vtkCamera.h>
// ---------------

#include <QMainWindow>
#include <QFileDialog>
#include <QThread>
#include <QHash>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTextStream>
#include <pcl/io/ply_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <thread>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMovie>
#include <QtGui>

#include <iostream>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>

#define VIEW_3D			1
#define VIEW_LATERAL	2
#define VIEW_PLAN		3

#define MOVIE_INIT		1
#define MOVIE_LOAD		2
#define MOVIE_MERGE		3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void loadWidgets(QString path);

	void setViewDisplay(int nView, bool bShowWidget, int nStatus);
	void setAllViewDisplay(bool bShowWidget, int nStatus);
	
	void processView(int nView);
	void processMerge();

	void onMerge();
	void onMergeEnd();


	void showPLY();
	void showPlane(bool bPlanView);

	void showPlaneNoColor(bool bPlanView);
	void showPlaneColor(bool bPlanView);
	
	char *filePath;
	vtkMutexLock *renderLock;

private slots:
    void on_btnBrowse_clicked();
    void on_btnMerge_clicked();
	void on_btnOpen_clicked();
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
	void on_listWidget_itemClicked(QListWidgetItem *item);

	
private:
    Ui::MainWindow *ui;
    QStringList detectPlyFiles(QDir dirToImport);
    bool readImportFile(QString import);
	void dropEvent(QDropEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void setWidgetBorderRadius(QWidget* widget, int radius);
	
	QMovie * movieInit;
	QMovie * movieLoad;
	QMovie * movieMerge;

	bool withColor = false;
	QString selectedFile;
    QStringList list;
	QHash<QString, QString> listContent;
};

class ThreadOpen : public QThread
{
	Q_OBJECT

public:
	ThreadOpen(MainWindow * mw, int n) { this->mw = mw;  this->nView = n; }

public slots :
	void onEnd()
	{
		mw->setViewDisplay(nView, true, NULL);
		cout << "Thread " << (nView+1) << " finished" << endl;
	}

private:
	MainWindow * mw;
	int nView;

	void run()
	{
		cout << "Thread " << (nView+1) << " launched..." << endl;
		mw->processView(nView);
	}
};

class ThreadMerge : public QThread
{
	Q_OBJECT

public:
	ThreadMerge(MainWindow * mw) { this->mw = mw; }

public slots :
	void onEnd()
	{
		cout << "Merge Thread finished" << endl;
		this->mw->onMergeEnd();
	}

private:
	MainWindow * mw;

	void run()
	{
		cout << "Merge Thread launched..." << endl;
		mw->processMerge();
	}
};

#endif // MAINWINDOW_H
