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
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QListWidget *listWidget;
    QProgressBar *progressBar;
    QLabel *lbCurrentMerge;
    QPushButton *btnMerge;
    QPushButton *btnBrowse;
    QLabel *lbCaptureName;
    QPushButton *pushButton_2;
    QVTKWidget *qvtkWidgetLateral;
    QVTKWidget *qvtkWidgetPlan;
    QVTKWidget *qvtkWidget3D;
    QLabel *gifInit3D;
    QLabel *gifInitLateral;
    QLabel *gifInitPlan;
    QPushButton *btnOpen;
    QLabel *gifLoading3D;
    QLabel *gifLoadingLateral;
    QLabel *gifLoadingPlan;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1150, 590);
        MainWindow->setMaximumSize(QSize(1150, 590));
        QIcon icon;
        icon.addFile(QStringLiteral("app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(10, 80, 201, 151));
        listWidget->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 6px;\n"
"border-color: grey;\n"
"padding: 4px;"));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(10, 310, 201, 21));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        progressBar->setTextVisible(true);
        lbCurrentMerge = new QLabel(centralWidget);
        lbCurrentMerge->setObjectName(QStringLiteral("lbCurrentMerge"));
        lbCurrentMerge->setGeometry(QRect(10, 290, 201, 16));
        lbCurrentMerge->setAlignment(Qt::AlignCenter);
        btnMerge = new QPushButton(centralWidget);
        btnMerge->setObjectName(QStringLiteral("btnMerge"));
        btnMerge->setEnabled(false);
        btnMerge->setGeometry(QRect(120, 240, 91, 31));
        btnMerge->setStyleSheet(QLatin1String("QPushButton#btnMerge {\n"
"	font: 11pt \"Nirmala UI\";\n"
"	background-color:#d1e0e0;\n"
"	border-style: outset;\n"
"	border-width: 2px;\n"
"	border-radius: 10px;\n"
"	border-color: grey;\n"
"	padding: 1px;\n"
"}\n"
"\n"
"QPushButton#btnMerge:hover {\n"
"	background-color:#BCC9C9;\n"
"}\n"
"\n"
"QPushButton#btnMerge:pressed {\n"
"	background-color:#a7b3b3;\n"
"}\n"
"\n"
"\n"
""));
        btnBrowse = new QPushButton(centralWidget);
        btnBrowse->setObjectName(QStringLiteral("btnBrowse"));
        btnBrowse->setGeometry(QRect(10, 10, 201, 31));
        btnBrowse->setStyleSheet(QLatin1String("QPushButton#btnBrowse {\n"
"	font: 11pt \"Nirmala UI\";\n"
"	background-color:#d1e0e0;\n"
"	border-style: outset;\n"
"	border-width: 2px;\n"
"	border-radius: 10px;\n"
"	border-color: grey;\n"
"	padding: 1px;\n"
"}\n"
"\n"
"QPushButton#btnBrowse:hover {\n"
"	background-color:#BCC9C9;\n"
"}\n"
"\n"
"QPushButton#btnBrowse:pressed {\n"
"	background-color:#a7b3b3;\n"
"}\n"
"\n"
"\n"
""));
        lbCaptureName = new QLabel(centralWidget);
        lbCaptureName->setObjectName(QStringLiteral("lbCaptureName"));
        lbCaptureName->setGeometry(QRect(10, 60, 201, 16));
        lbCaptureName->setAlignment(Qt::AlignCenter);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 360, 201, 221));
        qvtkWidgetLateral = new QVTKWidget(centralWidget);
        qvtkWidgetLateral->setObjectName(QStringLiteral("qvtkWidgetLateral"));
        qvtkWidgetLateral->setGeometry(QRect(860, 10, 281, 281));
        qvtkWidgetLateral->setStyleSheet(QStringLiteral(""));
        qvtkWidgetPlan = new QVTKWidget(centralWidget);
        qvtkWidgetPlan->setObjectName(QStringLiteral("qvtkWidgetPlan"));
        qvtkWidgetPlan->setGeometry(QRect(860, 300, 281, 281));
        qvtkWidgetPlan->setStyleSheet(QStringLiteral(""));
        qvtkWidget3D = new QVTKWidget(centralWidget);
        qvtkWidget3D->setObjectName(QStringLiteral("qvtkWidget3D"));
        qvtkWidget3D->setGeometry(QRect(220, 10, 631, 571));
        qvtkWidget3D->setStyleSheet(QStringLiteral(""));
        gifInit3D = new QLabel(centralWidget);
        gifInit3D->setObjectName(QStringLiteral("gifInit3D"));
        gifInit3D->setGeometry(QRect(220, 10, 631, 571));
        gifInit3D->setLayoutDirection(Qt::LeftToRight);
        gifInit3D->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        gifInitLateral = new QLabel(centralWidget);
        gifInitLateral->setObjectName(QStringLiteral("gifInitLateral"));
        gifInitLateral->setGeometry(QRect(860, 10, 281, 281));
        gifInitLateral->setLayoutDirection(Qt::LeftToRight);
        gifInitLateral->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        gifInitPlan = new QLabel(centralWidget);
        gifInitPlan->setObjectName(QStringLiteral("gifInitPlan"));
        gifInitPlan->setGeometry(QRect(860, 300, 281, 281));
        gifInitPlan->setLayoutDirection(Qt::LeftToRight);
        gifInitPlan->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        btnOpen = new QPushButton(centralWidget);
        btnOpen->setObjectName(QStringLiteral("btnOpen"));
        btnOpen->setEnabled(false);
        btnOpen->setGeometry(QRect(10, 240, 91, 31));
        btnOpen->setMinimumSize(QSize(0, 0));
        btnOpen->setStyleSheet(QLatin1String("QPushButton#btnOpen {\n"
"	font: 11pt \"Nirmala UI\";\n"
"	background-color:#d1e0e0;\n"
"	border-style: outset;\n"
"	border-width: 2px;\n"
"	border-radius: 10px;\n"
"	border-color: grey;\n"
"	padding: 1px;\n"
"}\n"
"\n"
"QPushButton#btnOpen:hover {\n"
"	background-color:#BCC9C9;\n"
"}\n"
"\n"
"QPushButton#btnOpen:pressed {\n"
"	background-color:#a7b3b3;\n"
"}\n"
"\n"
"\n"
""));
        gifLoading3D = new QLabel(centralWidget);
        gifLoading3D->setObjectName(QStringLiteral("gifLoading3D"));
        gifLoading3D->setGeometry(QRect(220, 10, 631, 571));
        gifLoading3D->setLayoutDirection(Qt::LeftToRight);
        gifLoading3D->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        gifLoadingLateral = new QLabel(centralWidget);
        gifLoadingLateral->setObjectName(QStringLiteral("gifLoadingLateral"));
        gifLoadingLateral->setGeometry(QRect(860, 10, 281, 281));
        gifLoadingLateral->setLayoutDirection(Qt::LeftToRight);
        gifLoadingLateral->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        gifLoadingPlan = new QLabel(centralWidget);
        gifLoadingPlan->setObjectName(QStringLiteral("gifLoadingPlan"));
        gifLoadingPlan->setGeometry(QRect(860, 300, 281, 281));
        gifLoadingPlan->setLayoutDirection(Qt::LeftToRight);
        gifLoadingPlan->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Kinect 3D Modelling", 0));
        lbCurrentMerge->setText(QString());
        btnMerge->setText(QApplication::translate("MainWindow", "Merge", 0));
        btnBrowse->setText(QApplication::translate("MainWindow", "Browse", 0));
        lbCaptureName->setText(QString());
        pushButton_2->setText(QApplication::translate("MainWindow", "Measure Area", 0));
        gifInit3D->setText(QApplication::translate("MainWindow", "Loading", 0));
        gifInitLateral->setText(QApplication::translate("MainWindow", "Loading", 0));
        gifInitPlan->setText(QApplication::translate("MainWindow", "Loading", 0));
        btnOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        gifLoading3D->setText(QApplication::translate("MainWindow", "Loading", 0));
        gifLoadingLateral->setText(QApplication::translate("MainWindow", "Loading", 0));
        gifLoadingPlan->setText(QApplication::translate("MainWindow", "Loading", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
