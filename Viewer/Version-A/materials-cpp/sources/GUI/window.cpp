//#include "window.h"
//#include "ui_some_test.h"
//
//pcl::visualization::PCLVisualizer pviz("test_viz", false);
//pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz(new pcl::PointCloud<pcl::PointXYZ>);
//
//Pclwindow::Pclwindow(QWidget *parent) :
//	QMainWindow(parent),
//	ui(new Ui::Pclwindow)
//{
//	ui->setupUi(this);
//	this->setWindowTitle("PCL viewer");
//
//	// Setup the cloud pointer
//	cloud.reset(new PointCloudT);
//	// The number of points in the cloud
//	cloud->points.resize(200);
//
//	// The default color
//	red = 128;
//	green = 128;
//	blue = 128;
//
//	// Fill the cloud with some points
//	for (size_t i = 0; i < cloud->points.size(); ++i)
//	{
//		cloud->points[i].x = 1024 * rand() / (RAND_MAX + 1.0f);
//		cloud->points[i].y = 1024 * rand() / (RAND_MAX + 1.0f);
//		cloud->points[i].z = 1024 * rand() / (RAND_MAX + 1.0f);
//
//		cloud->points[i].r = red;
//		cloud->points[i].g = green;
//		cloud->points[i].b = blue;
//	}
//
//	// Set up the QVTK window
//	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
//	ui->qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
//	viewer->setupInteractor(ui->qvtkWidget->GetInteractor(), ui->qvtkWidget->GetRenderWindow());
//	ui->qvtkWidget->update();
//
//
//	viewer->addPointCloud(cloud, "cloud");
//	pSliderValueChanged(2);
//	viewer->resetCamera();
//	ui->qvtkWidget->update();
//}
//
//Pclwindow::~Pclwindow()
//{
//	delete ui;
//}
//
////void Pclwindow::on_action_triggered()
////{
////	this->close();
////}
////
////void Pclwindow::on_action_2_triggered()
////{
////	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.ply)"));
////	if (fileName != "")
////	{
////		pviz.removeAllPointClouds();
////		ui->widget->setDisabled(true);
////		ui->menuBar->setDisabled(true);
////		
////		boost::thread t(boost::bind(pcl::io::loadPLYFile<pcl::PointXYZ>, fileName.toStdString(), boost::ref(*cloud_xyz)));
////		t.join();
////				
////		ui->progressBar->setValue(100);
////		ui->widget->setEnabled(true);
////		ui->menuBar->setEnabled(true);
////		
////		if (cloud_xyz->size() > 0)
////		{
////			//pviz.addPointCloud<pcl::PointXYZ>(cloud_xyz);
////			ui->progressBar->setValue(0);
////		}
////	}
////}