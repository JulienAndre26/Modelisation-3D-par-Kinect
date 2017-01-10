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
    QVTKWidget *qvtkWidgetLateral;
    QVTKWidget *qvtkWidgetPlan;
    QVTKWidget *qvtkWidget3D;
    QLabel *gif3D;
    QLabel *gifLateral;
    QLabel *gifPlan;
    QPushButton *btnShow;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    QLabel *lbP1x;
    QLabel *lbP1y;
    QLabel *lbP1z;
    QLabel *lbP2x;
    QLabel *lbP2z;
    QLabel *lbP2y;
    QLabel *lbDistance;
    QLabel *lbPin1;
    QLabel *lbPin2;
    QLabel *lbDistanceIcon;
    QLabel *lbLoadedFile;
    QPushButton *btnHelp;

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
        lbCaptureName->setGeometry(QRect(10, 60, 161, 16));
        lbCaptureName->setTextFormat(Qt::RichText);
        lbCaptureName->setAlignment(Qt::AlignCenter);
        qvtkWidgetLateral = new QVTKWidget(centralWidget);
        qvtkWidgetLateral->setObjectName(QStringLiteral("qvtkWidgetLateral"));
        qvtkWidgetLateral->setGeometry(QRect(860, 10, 281, 281));
        qvtkWidgetLateral->setCursor(QCursor(Qt::OpenHandCursor));
        qvtkWidgetLateral->setStyleSheet(QStringLiteral(""));
        qvtkWidgetPlan = new QVTKWidget(centralWidget);
        qvtkWidgetPlan->setObjectName(QStringLiteral("qvtkWidgetPlan"));
        qvtkWidgetPlan->setGeometry(QRect(860, 300, 281, 281));
        qvtkWidgetPlan->setCursor(QCursor(Qt::OpenHandCursor));
        qvtkWidgetPlan->setStyleSheet(QStringLiteral(""));
        qvtkWidget3D = new QVTKWidget(centralWidget);
        qvtkWidget3D->setObjectName(QStringLiteral("qvtkWidget3D"));
        qvtkWidget3D->setGeometry(QRect(220, 30, 631, 551));
        qvtkWidget3D->setCursor(QCursor(Qt::OpenHandCursor));
        qvtkWidget3D->setStyleSheet(QStringLiteral(""));
        gif3D = new QLabel(centralWidget);
        gif3D->setObjectName(QStringLiteral("gif3D"));
        gif3D->setGeometry(QRect(220, 30, 631, 551));
        gif3D->setLayoutDirection(Qt::LeftToRight);
        gif3D->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        gif3D->setIndent(-1);
        gifLateral = new QLabel(centralWidget);
        gifLateral->setObjectName(QStringLiteral("gifLateral"));
        gifLateral->setGeometry(QRect(860, 10, 281, 281));
        gifLateral->setLayoutDirection(Qt::LeftToRight);
        gifLateral->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        gifPlan = new QLabel(centralWidget);
        gifPlan->setObjectName(QStringLiteral("gifPlan"));
        gifPlan->setGeometry(QRect(860, 300, 281, 281));
        gifPlan->setLayoutDirection(Qt::LeftToRight);
        gifPlan->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        btnShow = new QPushButton(centralWidget);
        btnShow->setObjectName(QStringLiteral("btnShow"));
        btnShow->setEnabled(false);
        btnShow->setGeometry(QRect(10, 240, 91, 31));
        btnShow->setMinimumSize(QSize(0, 0));
        btnShow->setStyleSheet(QLatin1String("QPushButton#btnShow {\n"
"	font: 11pt \"Nirmala UI\";\n"
"	background-color:#d1e0e0;\n"
"	border-style: outset;\n"
"	border-width: 2px;\n"
"	border-radius: 10px;\n"
"	border-color: grey;\n"
"	padding: 1px;\n"
"}\n"
"\n"
"QPushButton#btnShow:hover {\n"
"	background-color:#BCC9C9;\n"
"}\n"
"\n"
"QPushButton#btnShow:pressed {\n"
"	background-color:#a7b3b3;\n"
"}\n"
"\n"
"\n"
""));
        btnAdd = new QPushButton(centralWidget);
        btnAdd->setObjectName(QStringLiteral("btnAdd"));
        btnAdd->setGeometry(QRect(170, 60, 21, 21));
        btnAdd->setStyleSheet(QLatin1String("QPushButton#btnAdd {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnAdd:hover {\n"
"	padding: 1px;\n"
"}\n"
"\n"
""));
        btnDelete = new QPushButton(centralWidget);
        btnDelete->setObjectName(QStringLiteral("btnDelete"));
        btnDelete->setGeometry(QRect(190, 60, 21, 21));
        btnDelete->setStyleSheet(QLatin1String("QPushButton#btnDelete {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnDelete:hover {\n"
"	padding: 1px;\n"
"}\n"
"\n"
""));
        lbP1x = new QLabel(centralWidget);
        lbP1x->setObjectName(QStringLiteral("lbP1x"));
        lbP1x->setGeometry(QRect(20, 490, 81, 16));
        lbP1x->setTextFormat(Qt::RichText);
        lbP1y = new QLabel(centralWidget);
        lbP1y->setObjectName(QStringLiteral("lbP1y"));
        lbP1y->setGeometry(QRect(20, 510, 81, 16));
        lbP1y->setTextFormat(Qt::RichText);
        lbP1z = new QLabel(centralWidget);
        lbP1z->setObjectName(QStringLiteral("lbP1z"));
        lbP1z->setGeometry(QRect(20, 530, 81, 16));
        lbP1z->setTextFormat(Qt::RichText);
        lbP2x = new QLabel(centralWidget);
        lbP2x->setObjectName(QStringLiteral("lbP2x"));
        lbP2x->setGeometry(QRect(120, 490, 81, 16));
        lbP2x->setTextFormat(Qt::RichText);
        lbP2z = new QLabel(centralWidget);
        lbP2z->setObjectName(QStringLiteral("lbP2z"));
        lbP2z->setGeometry(QRect(120, 530, 81, 16));
        lbP2z->setTextFormat(Qt::RichText);
        lbP2y = new QLabel(centralWidget);
        lbP2y->setObjectName(QStringLiteral("lbP2y"));
        lbP2y->setGeometry(QRect(120, 510, 81, 16));
        lbP2y->setTextFormat(Qt::RichText);
        lbDistance = new QLabel(centralWidget);
        lbDistance->setObjectName(QStringLiteral("lbDistance"));
        lbDistance->setGeometry(QRect(50, 560, 141, 20));
        lbDistance->setTextFormat(Qt::RichText);
        lbPin1 = new QLabel(centralWidget);
        lbPin1->setObjectName(QStringLiteral("lbPin1"));
        lbPin1->setGeometry(QRect(40, 450, 41, 31));
        lbPin1->setAlignment(Qt::AlignCenter);
        lbPin2 = new QLabel(centralWidget);
        lbPin2->setObjectName(QStringLiteral("lbPin2"));
        lbPin2->setGeometry(QRect(140, 450, 41, 31));
        lbPin2->setAlignment(Qt::AlignCenter);
        lbDistanceIcon = new QLabel(centralWidget);
        lbDistanceIcon->setObjectName(QStringLiteral("lbDistanceIcon"));
        lbDistanceIcon->setGeometry(QRect(10, 550, 41, 31));
        lbDistanceIcon->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        lbLoadedFile = new QLabel(centralWidget);
        lbLoadedFile->setObjectName(QStringLiteral("lbLoadedFile"));
        lbLoadedFile->setGeometry(QRect(220, 10, 601, 20));
        lbLoadedFile->setTextFormat(Qt::AutoText);
        lbLoadedFile->setAlignment(Qt::AlignCenter);
        btnHelp = new QPushButton(centralWidget);
        btnHelp->setObjectName(QStringLiteral("btnHelp"));
        btnHelp->setGeometry(QRect(830, 10, 20, 20));
        btnHelp->setStyleSheet(QLatin1String("QPushButton#btnHelp {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnHelp:hover {\n"
"	padding: 1px;\n"
"}\n"
"\n"
""));
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
        gif3D->setText(QApplication::translate("MainWindow", "Loading", 0));
        gifLateral->setText(QApplication::translate("MainWindow", "Loading", 0));
        gifPlan->setText(QApplication::translate("MainWindow", "Loading", 0));
        btnShow->setText(QApplication::translate("MainWindow", "Show", 0));
#ifndef QT_NO_TOOLTIP
        btnAdd->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Add a 3D plan to this capture</span></p><p>It will copy the selected PLY file into the capture's folder.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnAdd->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnDelete->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Removes a 3D plan from this capture</span></p><p>It will remove the selected PLY file from the capture's folder.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnDelete->setText(QString());
        lbP1x->setText(QApplication::translate("MainWindow", "X1", 0));
        lbP1y->setText(QApplication::translate("MainWindow", "Y1", 0));
        lbP1z->setText(QApplication::translate("MainWindow", "Z1", 0));
        lbP2x->setText(QApplication::translate("MainWindow", "X2", 0));
        lbP2z->setText(QApplication::translate("MainWindow", "Z3", 0));
        lbP2y->setText(QApplication::translate("MainWindow", "Y2", 0));
        lbDistance->setText(QApplication::translate("MainWindow", "Distance: xx.xx m (xx.xx cm)", 0));
        lbPin1->setText(QString());
        lbPin2->setText(QString());
        lbDistanceIcon->setText(QString());
        lbLoadedFile->setText(QApplication::translate("MainWindow", "Please browse a capture and open a file", 0));
#ifndef QT_NO_TOOLTIP
        btnHelp->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Handling 3D model</span></p><p>Clic + Move Mouse : Rotates the model</p><p>Shift + Move Mouse : Moves the model in X, Y or Z axis</p><p>Shift + Clic : Puts a pin for measurement (place two pins to get the distance between pins)</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnHelp->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
