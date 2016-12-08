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
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1200, 600);
        MainWindow->setMinimumSize(QSize(1200, 600));
        MainWindow->setMaximumSize(QSize(1200, 600));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(30, 150, 201, 151));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(30, 470, 201, 121));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        progressBar->setTextVisible(true);
        lbCurrentMerge = new QLabel(centralWidget);
        lbCurrentMerge->setObjectName(QStringLiteral("lbCurrentMerge"));
        lbCurrentMerge->setGeometry(QRect(30, 430, 201, 16));
        lbCurrentMerge->setAlignment(Qt::AlignCenter);
        btnMerge = new QPushButton(centralWidget);
        btnMerge->setObjectName(QStringLiteral("btnMerge"));
        btnMerge->setEnabled(true);
        btnMerge->setGeometry(QRect(30, 320, 201, 91));
        btnBrowse = new QPushButton(centralWidget);
        btnBrowse->setObjectName(QStringLiteral("btnBrowse"));
        btnBrowse->setGeometry(QRect(30, 10, 201, 91));
        btnBrowse->setStyleSheet(QStringLiteral(""));
        lbCaptureName = new QLabel(centralWidget);
        lbCaptureName->setObjectName(QStringLiteral("lbCaptureName"));
        lbCaptureName->setGeometry(QRect(32, 120, 201, 16));
        lbCaptureName->setAlignment(Qt::AlignCenter);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(260, 10, 491, 451));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(260, 470, 491, 121));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(770, 10, 421, 281));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(770, 310, 421, 281));
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
        pushButton->setText(QApplication::translate("MainWindow", "3D model", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "m\303\251triques (position du point s\303\251lectionn\303\251, distance pour 2 points)", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "premi\303\250re vue 2D", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "une autre vue 2D", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
