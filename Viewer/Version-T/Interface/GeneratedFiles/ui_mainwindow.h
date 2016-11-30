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
    QLabel *label;
    QListWidget *lwImportInfo;
    QWidget *tab_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(481, 311);
        MainWindow->setMinimumSize(QSize(481, 311));
        MainWindow->setMaximumSize(QSize(481, 311));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1, 0, 481, 311));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        btnBrowse = new QPushButton(tab);
        btnBrowse->setObjectName(QStringLiteral("btnBrowse"));
        btnBrowse->setGeometry(QRect(327, 10, 75, 23));
        btnBrowse->setStyleSheet(QStringLiteral(""));
        listWidget = new QListWidget(tab);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(22, 50, 201, 151));
        progressBar = new QProgressBar(tab);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(20, 210, 238, 23));
        progressBar->setValue(0);
        btnMerge = new QPushButton(tab);
        btnMerge->setObjectName(QStringLiteral("btnMerge"));
        btnMerge->setEnabled(true);
        btnMerge->setGeometry(QRect(326, 210, 75, 23));
        lbCurrentMerge = new QLabel(tab);
        lbCurrentMerge->setObjectName(QStringLiteral("lbCurrentMerge"));
        lbCurrentMerge->setGeometry(QRect(21, 250, 201, 16));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 10, 181, 16));
        lwImportInfo = new QListWidget(tab);
        lwImportInfo->setObjectName(QStringLiteral("lwImportInfo"));
        lwImportInfo->setGeometry(QRect(270, 50, 181, 151));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Mod\303\251lisation par Kinect", 0));
        btnBrowse->setText(QApplication::translate("MainWindow", "Browse", 0));
        btnMerge->setText(QApplication::translate("MainWindow", "Merge", 0));
        lbCurrentMerge->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Choisir le fichier .import :", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Traitement", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Visualisation", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
