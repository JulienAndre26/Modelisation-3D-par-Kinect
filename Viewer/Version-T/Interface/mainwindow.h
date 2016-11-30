#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <iostream>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void showPLY();
	void showPlane(bool bPlanView);

	char *filePath;

private slots:
    void on_btnBrowse_clicked();

    void on_btnMerge_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);


private:
    Ui::MainWindow *ui;
    QStringList detectPlyFiles(QDir dirToImport);
    bool readImportFile(QString import);
	
	bool withColor = false;
    QStringList list;
	QHash<QString, QString> listContent;
};

#endif // MAINWINDOW_H
