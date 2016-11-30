/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *btnBrowse;
    QListWidget *listWidget;
    QProgressBar *progressBar;
    QPushButton *btnMerge;
    QLabel *lbCurrentMerge;
    QLabel *lbPath;
    QLabel *lbCount;
    QLabel *lbFormat;
    QLabel *lbColor;
    QLabel *lbVoxels;
    QLabel *lbWidth;
    QLabel *lbHeight;
    QLabel *lbDepth;
    QWidget *tab_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(481, 271);
        MainWindow->setMinimumSize(QSize(481, 271));
        MainWindow->setMaximumSize(QSize(481, 311));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 481, 271));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        btnBrowse = new QPushButton(tab);
        btnBrowse->setObjectName(QStringLiteral("btnBrowse"));
        btnBrowse->setGeometry(QRect(80, 10, 75, 23));
        btnBrowse->setStyleSheet(QStringLiteral(""));
        listWidget = new QListWidget(tab);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(22, 50, 201, 151));
        progressBar = new QProgressBar(tab);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(22, 210, 201, 23));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        progressBar->setTextVisible(true);
        btnMerge = new QPushButton(tab);
        btnMerge->setObjectName(QStringLiteral("btnMerge"));
        btnMerge->setEnabled(true);
        btnMerge->setGeometry(QRect(310, 10, 75, 23));
        lbCurrentMerge = new QLabel(tab);
        lbCurrentMerge->setObjectName(QStringLiteral("lbCurrentMerge"));
        lbCurrentMerge->setGeometry(QRect(260, 214, 201, 16));
        lbPath = new QLabel(tab);
        lbPath->setObjectName(QStringLiteral("lbPath"));
        lbPath->setGeometry(QRect(280, 44, 161, 16));
        lbCount = new QLabel(tab);
        lbCount->setObjectName(QStringLiteral("lbCount"));
        lbCount->setGeometry(QRect(280, 64, 161, 16));
        lbFormat = new QLabel(tab);
        lbFormat->setObjectName(QStringLiteral("lbFormat"));
        lbFormat->setGeometry(QRect(280, 84, 161, 16));
        lbColor = new QLabel(tab);
        lbColor->setObjectName(QStringLiteral("lbColor"));
        lbColor->setGeometry(QRect(280, 104, 161, 16));
        lbVoxels = new QLabel(tab);
        lbVoxels->setObjectName(QStringLiteral("lbVoxels"));
        lbVoxels->setGeometry(QRect(280, 124, 161, 16));
        lbWidth = new QLabel(tab);
        lbWidth->setObjectName(QStringLiteral("lbWidth"));
        lbWidth->setGeometry(QRect(280, 144, 161, 16));
        lbHeight = new QLabel(tab);
        lbHeight->setObjectName(QStringLiteral("lbHeight"));
        lbHeight->setGeometry(QRect(280, 164, 161, 16));
        lbDepth = new QLabel(tab);
        lbDepth->setObjectName(QStringLiteral("lbDepth"));
        lbDepth->setGeometry(QRect(280, 184, 161, 16));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Kinect 3D Modelling", 0));
        btnBrowse->setText(QApplication::translate("MainWindow", "Browse", 0));
        btnMerge->setText(QApplication::translate("MainWindow", "Merge", 0));
        lbCurrentMerge->setText(QString());
        lbPath->setText(QApplication::translate("MainWindow", "Capture name : ", 0));
        lbCount->setText(QApplication::translate("MainWindow", "Files count :", 0));
        lbFormat->setText(QApplication::translate("MainWindow", "Format :", 0));
        lbColor->setText(QApplication::translate("MainWindow", "Color :", 0));
        lbVoxels->setText(QApplication::translate("MainWindow", "Voxels/meter :", 0));
        lbWidth->setText(QApplication::translate("MainWindow", "Width :", 0));
        lbHeight->setText(QApplication::translate("MainWindow", "Height :", 0));
        lbDepth->setText(QApplication::translate("MainWindow", "Depth :", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Process", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Visualization", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
