/********************************************************************************
** Form generated from reading UI file 'some_test.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOME_TEST_H
#define UI_SOME_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_some_testClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *some_testClass)
    {
        if (some_testClass->objectName().isEmpty())
            some_testClass->setObjectName(QStringLiteral("some_testClass"));
        some_testClass->resize(600, 400);
        menuBar = new QMenuBar(some_testClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        some_testClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(some_testClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        some_testClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(some_testClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        some_testClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(some_testClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        some_testClass->setStatusBar(statusBar);

        retranslateUi(some_testClass);

        QMetaObject::connectSlotsByName(some_testClass);
    } // setupUi

    void retranslateUi(QMainWindow *some_testClass)
    {
        some_testClass->setWindowTitle(QApplication::translate("some_testClass", "some_test", 0));
    } // retranslateUi

};

namespace Ui {
    class some_testClass: public Ui_some_testClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOME_TEST_H
