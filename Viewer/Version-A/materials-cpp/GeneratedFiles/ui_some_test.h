/********************************************************************************
** Form generated from reading UI file 'some_test.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOME_TEST_H
#define UI_SOME_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Pclwindow
{
public:
    QAction *action;
    QAction *action_2;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVTKWidget *widget;
    QProgressBar *progressBar;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Pclwindow)
    {
        if (Pclwindow->objectName().isEmpty())
            Pclwindow->setObjectName(QStringLiteral("Pclwindow"));
        Pclwindow->resize(710, 414);
        action = new QAction(Pclwindow);
        action->setObjectName(QStringLiteral("action"));
        action_2 = new QAction(Pclwindow);
        action_2->setObjectName(QStringLiteral("action_2"));
        centralWidget = new QWidget(Pclwindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new QVTKWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(601, 271));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        gridLayout->addWidget(progressBar, 1, 0, 1, 1);

        Pclwindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Pclwindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 710, 21));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        Pclwindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Pclwindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Pclwindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Pclwindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Pclwindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(action_2);
        menu->addAction(action);

        retranslateUi(Pclwindow);

        QMetaObject::connectSlotsByName(Pclwindow);
    } // setupUi

    void retranslateUi(QMainWindow *Pclwindow)
    {
        Pclwindow->setWindowTitle(QApplication::translate("Pclwindow", "Pclwindow", 0));
        action->setText(QApplication::translate("Pclwindow", "Exit", 0));
        action_2->setText(QApplication::translate("Pclwindow", "Open file", 0));
        menu->setTitle(QApplication::translate("Pclwindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class Pclwindow: public Ui_Pclwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOME_TEST_H
