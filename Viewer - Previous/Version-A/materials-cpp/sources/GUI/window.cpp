//
//#include "window.h"
//#include "ui_some_test.h"
//#include <QFileDialog>
//#include <QMessageBox>
//
//#include <pcl/point_types.h>
//#include <pcl/common/common.h>
//#include <pcl/visualization/cloud_viewer.h>
//#include <pcl/visualization/pcl_visualizer.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/io/ply_io.h>
//#include <vtkRenderWindow.h>
//#include <iostream>
//#include <boost/filesystem.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/bind.hpp>
////
////
////
//pcl::visualization::PCLVisualizer pviz("test_viz", false);
//pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz(new pcl::PointCloud<pcl::PointXYZ>);
//
//
//void Pclwindow::on_action_triggered()
//{
//	this->close();
//}
//
//void Pclwindow::on_action_2_triggered()
//{
//	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.ply)"));
//	if (fileName != "")
//	{
//		pviz.removeAllPointClouds();
//		//ui->widget->setDisabled(true);
//		ui->menuBar->setDisabled(true);
//		boost::thread t(boost::bind(pcl::io::loadPLYFile<pcl::PointXYZ>, fileName.toStdString(), boost::ref(*cloud_xyz)));
//		//for (int i = 0; i <= (int) sec; i++)
//		//{
//		//    ui->progressBar->setValue((int) ((i/sec)*100));
//		//    ui->label->setText(QString::fromStdString("Wait for "+boost::lexical_cast<std::string>(((int)sec)-i)+" sec"));
//		//boost::this_thread::sleep(boost::posix_time::seconds(1)); // wait 1 sec
//		//}
//		t.join();
//		ui->progressBar->setValue(100);
//		//ui->widget->setEnabled(true);
//		ui->menuBar->setEnabled(true);
//		if (cloud_xyz->size() > 0)
//		{
//			pviz.addPointCloud<pcl::PointXYZ>(cloud_xyz);
//			ui->progressBar->setValue(0);
//		}
//	};
//}
//
//////void Pclwindow::on_action_triggered()
//////{
//////	this->close();
//////}
//////
//////void Pclwindow::on_action_2_triggered()
//////{
//////	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.ply)"));
//////	if (fileName != "")
//////	{
//////		pviz.removeAllPointClouds();
//////		ui->widget->setDisabled(true);
//////		ui->menuBar->setDisabled(true);
//////		
//////		boost::thread t(boost::bind(pcl::io::loadPLYFile<pcl::PointXYZ>, fileName.toStdString(), boost::ref(*cloud_xyz)));
//////		t.join();
//////				
//////		ui->progressBar->setValue(100);
//////		ui->widget->setEnabled(true);
//////		ui->menuBar->setEnabled(true);
//////		
//////		if (cloud_xyz->size() > 0)
//////		{
//////			//pviz.addPointCloud<pcl::PointXYZ>(cloud_xyz);
//////			ui->progressBar->setValue(0);
//////		}
//////	}
//////}
