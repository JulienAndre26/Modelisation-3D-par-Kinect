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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *vlMid;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnLeftPlan;
    QLabel *lbLoadedFile;
    QPushButton *btnRightPlan;
    QPushButton *btnExitFS;
    QPushButton *btnHelpM;
    QVTKWidget *qvtkWidget3D;
    QLabel *gif3D;
    QVBoxLayout *vlRight;
    QHBoxLayout *horizontalLayout;
    QLabel *lbInvisible;
    QPushButton *btnFS;
    QPushButton *btnResetCamera;
    QPushButton *btnHelpR;
    QVTKWidget *qvtkWidgetLateral;
    QLabel *gifLateral;
    QVTKWidget *qvtkWidgetPlan;
    QLabel *gifPlan;
    QVBoxLayout *vlLeft;
    QWidget *wdgLeftTop;
    QListWidget *listWidget;
    QPushButton *btnAdd;
    QPushButton *btnMerge;
    QPushButton *btnShow;
    QPushButton *btnDelete;
    QLabel *lbCaptureName;
    QPushButton *btnBrowse;
    QLabel *invisible2;
    QWidget *wdgLeftBottom;
    QLabel *lbPin1;
    QLabel *lbP2y;
    QLabel *lbP1x;
    QLabel *lbCurrentMerge;
    QLabel *lbDistance;
    QLabel *lbDistanceIcon;
    QLabel *lbP1z;
    QLabel *lbP2z;
    QLabel *lbP2x;
    QLabel *lbP1y;
    QProgressBar *progressBar;
    QLabel *lbPin2;
    QWidget *wdgBoxSize;
    QLabel *lbCompass;
    QLabel *lbWidth;
    QLabel *lbWidthValue;
    QLabel *lbHeight;
    QLabel *lbHeightValue;
    QLabel *lbDepthValue;
    QLabel *lbDepth;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(952, 647);
        MainWindow->setMaximumSize(QSize(10000, 10000));
        QIcon icon;
        icon.addFile(QStringLiteral("../../../../../.designer/backup/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        vlMid = new QVBoxLayout();
        vlMid->setSpacing(6);
        vlMid->setObjectName(QStringLiteral("vlMid"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btnLeftPlan = new QPushButton(centralWidget);
        btnLeftPlan->setObjectName(QStringLiteral("btnLeftPlan"));
        btnLeftPlan->setEnabled(false);
        btnLeftPlan->setMinimumSize(QSize(21, 21));
        btnLeftPlan->setMaximumSize(QSize(21, 21));
        btnLeftPlan->setStyleSheet(QLatin1String("QPushButton#btnLeftPlan {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnLeftPlan:hover {\n"
"	padding: 1px;\n"
"}"));

        horizontalLayout_2->addWidget(btnLeftPlan);

        lbLoadedFile = new QLabel(centralWidget);
        lbLoadedFile->setObjectName(QStringLiteral("lbLoadedFile"));
        lbLoadedFile->setMinimumSize(QSize(0, 21));
        lbLoadedFile->setMaximumSize(QSize(16777215, 21));
        lbLoadedFile->setStyleSheet(QLatin1String("font: 11pt \"Nirmala UI\";\n"
""));
        lbLoadedFile->setTextFormat(Qt::AutoText);
        lbLoadedFile->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lbLoadedFile);

        btnRightPlan = new QPushButton(centralWidget);
        btnRightPlan->setObjectName(QStringLiteral("btnRightPlan"));
        btnRightPlan->setEnabled(false);
        btnRightPlan->setMinimumSize(QSize(21, 21));
        btnRightPlan->setMaximumSize(QSize(21, 21));
        btnRightPlan->setStyleSheet(QLatin1String("QPushButton#btnRightPlan {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnRightPlan:hover {\n"
"	padding: 1px;\n"
"}"));

        horizontalLayout_2->addWidget(btnRightPlan);

        btnExitFS = new QPushButton(centralWidget);
        btnExitFS->setObjectName(QStringLiteral("btnExitFS"));
        btnExitFS->setMinimumSize(QSize(21, 21));
        btnExitFS->setMaximumSize(QSize(21, 21));
        btnExitFS->setStyleSheet(QLatin1String("QPushButton#btnExitFS {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnExitFS:hover {\n"
"	padding: 1px;\n"
"}"));

        horizontalLayout_2->addWidget(btnExitFS);

        btnHelpM = new QPushButton(centralWidget);
        btnHelpM->setObjectName(QStringLiteral("btnHelpM"));
        btnHelpM->setMinimumSize(QSize(21, 21));
        btnHelpM->setMaximumSize(QSize(21, 21));
        btnHelpM->setStyleSheet(QLatin1String("QPushButton#btnHelpM {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnHelpM:hover {\n"
"	padding: 1px;\n"
"}"));

        horizontalLayout_2->addWidget(btnHelpM);


        vlMid->addLayout(horizontalLayout_2);

        qvtkWidget3D = new QVTKWidget(centralWidget);
        qvtkWidget3D->setObjectName(QStringLiteral("qvtkWidget3D"));
        qvtkWidget3D->setMinimumSize(QSize(300, 300));
        qvtkWidget3D->setCursor(QCursor(Qt::CrossCursor));
        qvtkWidget3D->setStyleSheet(QStringLiteral(""));

        vlMid->addWidget(qvtkWidget3D);

        gif3D = new QLabel(centralWidget);
        gif3D->setObjectName(QStringLiteral("gif3D"));
        gif3D->setMinimumSize(QSize(186, 0));
        gif3D->setLayoutDirection(Qt::LeftToRight);
        gif3D->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));
        gif3D->setIndent(-1);

        vlMid->addWidget(gif3D);


        gridLayout->addLayout(vlMid, 0, 1, 1, 1);

        vlRight = new QVBoxLayout();
        vlRight->setSpacing(6);
        vlRight->setObjectName(QStringLiteral("vlRight"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lbInvisible = new QLabel(centralWidget);
        lbInvisible->setObjectName(QStringLiteral("lbInvisible"));
        lbInvisible->setMinimumSize(QSize(0, 21));
        lbInvisible->setMaximumSize(QSize(16777215, 21));

        horizontalLayout->addWidget(lbInvisible);

        btnFS = new QPushButton(centralWidget);
        btnFS->setObjectName(QStringLiteral("btnFS"));
        btnFS->setMinimumSize(QSize(21, 21));
        btnFS->setMaximumSize(QSize(21, 21));
        btnFS->setStyleSheet(QLatin1String("QPushButton#btnFS {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnFS:hover {\n"
"	padding: 1px;\n"
"}"));

        horizontalLayout->addWidget(btnFS);

        btnResetCamera = new QPushButton(centralWidget);
        btnResetCamera->setObjectName(QStringLiteral("btnResetCamera"));
        btnResetCamera->setEnabled(false);
        btnResetCamera->setMinimumSize(QSize(21, 21));
        btnResetCamera->setMaximumSize(QSize(21, 21));
        btnResetCamera->setStyleSheet(QLatin1String("QPushButton#btnResetCamera {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnResetCamera:hover {\n"
"	padding: 1px;\n"
"}"));

        horizontalLayout->addWidget(btnResetCamera);

        btnHelpR = new QPushButton(centralWidget);
        btnHelpR->setObjectName(QStringLiteral("btnHelpR"));
        btnHelpR->setMinimumSize(QSize(21, 21));
        btnHelpR->setMaximumSize(QSize(21, 21));
        btnHelpR->setStyleSheet(QLatin1String("QPushButton#btnHelpR {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnHelpR:hover {\n"
"	padding: 1px;\n"
"}"));

        horizontalLayout->addWidget(btnHelpR);


        vlRight->addLayout(horizontalLayout);

        qvtkWidgetLateral = new QVTKWidget(centralWidget);
        qvtkWidgetLateral->setObjectName(QStringLiteral("qvtkWidgetLateral"));
        qvtkWidgetLateral->setMinimumSize(QSize(100, 100));
        qvtkWidgetLateral->setMaximumSize(QSize(300, 16777215));
        qvtkWidgetLateral->setCursor(QCursor(Qt::CrossCursor));
        qvtkWidgetLateral->setStyleSheet(QStringLiteral(""));

        vlRight->addWidget(qvtkWidgetLateral);

        gifLateral = new QLabel(centralWidget);
        gifLateral->setObjectName(QStringLiteral("gifLateral"));
        gifLateral->setMinimumSize(QSize(186, 100));
        gifLateral->setMaximumSize(QSize(300, 16777215));
        gifLateral->setLayoutDirection(Qt::LeftToRight);
        gifLateral->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));

        vlRight->addWidget(gifLateral);

        qvtkWidgetPlan = new QVTKWidget(centralWidget);
        qvtkWidgetPlan->setObjectName(QStringLiteral("qvtkWidgetPlan"));
        qvtkWidgetPlan->setMinimumSize(QSize(100, 100));
        qvtkWidgetPlan->setMaximumSize(QSize(300, 16777215));
        qvtkWidgetPlan->setCursor(QCursor(Qt::CrossCursor));
        qvtkWidgetPlan->setStyleSheet(QStringLiteral(""));

        vlRight->addWidget(qvtkWidgetPlan);

        gifPlan = new QLabel(centralWidget);
        gifPlan->setObjectName(QStringLiteral("gifPlan"));
        gifPlan->setMinimumSize(QSize(186, 100));
        gifPlan->setMaximumSize(QSize(300, 16777215));
        gifPlan->setLayoutDirection(Qt::LeftToRight);
        gifPlan->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: grey;\n"
"font: bold 14px;\n"
"min-width: 10em;\n"
"padding: 6px;"));

        vlRight->addWidget(gifPlan);


        gridLayout->addLayout(vlRight, 0, 2, 1, 1);

        vlLeft = new QVBoxLayout();
        vlLeft->setSpacing(6);
        vlLeft->setObjectName(QStringLiteral("vlLeft"));
        wdgLeftTop = new QWidget(centralWidget);
        wdgLeftTop->setObjectName(QStringLiteral("wdgLeftTop"));
        wdgLeftTop->setMinimumSize(QSize(250, 300));
        wdgLeftTop->setMaximumSize(QSize(250, 300));
        listWidget = new QListWidget(wdgLeftTop);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(20, 100, 201, 151));
        listWidget->setStyleSheet(QLatin1String("border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 6px;\n"
"border-color: grey;\n"
"padding: 4px;\n"
""));
        btnAdd = new QPushButton(wdgLeftTop);
        btnAdd->setObjectName(QStringLiteral("btnAdd"));
        btnAdd->setGeometry(QRect(180, 80, 21, 21));
        btnAdd->setStyleSheet(QLatin1String("QPushButton#btnAdd {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnAdd:hover {\n"
"	padding: 1px;\n"
"}\n"
"\n"
""));
        btnMerge = new QPushButton(wdgLeftTop);
        btnMerge->setObjectName(QStringLiteral("btnMerge"));
        btnMerge->setEnabled(false);
        btnMerge->setGeometry(QRect(130, 260, 91, 31));
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
        btnShow = new QPushButton(wdgLeftTop);
        btnShow->setObjectName(QStringLiteral("btnShow"));
        btnShow->setEnabled(false);
        btnShow->setGeometry(QRect(20, 260, 91, 31));
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
        btnDelete = new QPushButton(wdgLeftTop);
        btnDelete->setObjectName(QStringLiteral("btnDelete"));
        btnDelete->setGeometry(QRect(200, 80, 21, 21));
        btnDelete->setStyleSheet(QLatin1String("QPushButton#btnDelete {\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton#btnDelete:hover {\n"
"	padding: 1px;\n"
"}\n"
"\n"
""));
        lbCaptureName = new QLabel(wdgLeftTop);
        lbCaptureName->setObjectName(QStringLiteral("lbCaptureName"));
        lbCaptureName->setGeometry(QRect(20, 80, 161, 16));
        lbCaptureName->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbCaptureName->setTextFormat(Qt::RichText);
        lbCaptureName->setAlignment(Qt::AlignCenter);
        btnBrowse = new QPushButton(wdgLeftTop);
        btnBrowse->setObjectName(QStringLiteral("btnBrowse"));
        btnBrowse->setGeometry(QRect(20, 30, 201, 31));
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

        vlLeft->addWidget(wdgLeftTop);

        invisible2 = new QLabel(centralWidget);
        invisible2->setObjectName(QStringLiteral("invisible2"));
        invisible2->setMaximumSize(QSize(250, 16777215));

        vlLeft->addWidget(invisible2);

        wdgLeftBottom = new QWidget(centralWidget);
        wdgLeftBottom->setObjectName(QStringLiteral("wdgLeftBottom"));
        wdgLeftBottom->setMinimumSize(QSize(250, 300));
        wdgLeftBottom->setMaximumSize(QSize(250, 300));
        lbPin1 = new QLabel(wdgLeftBottom);
        lbPin1->setObjectName(QStringLiteral("lbPin1"));
        lbPin1->setGeometry(QRect(50, 85, 41, 31));
        lbPin1->setAlignment(Qt::AlignCenter);
        lbP2y = new QLabel(wdgLeftBottom);
        lbP2y->setObjectName(QStringLiteral("lbP2y"));
        lbP2y->setGeometry(QRect(140, 135, 81, 16));
        lbP2y->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbP2y->setTextFormat(Qt::RichText);
        lbP1x = new QLabel(wdgLeftBottom);
        lbP1x->setObjectName(QStringLiteral("lbP1x"));
        lbP1x->setGeometry(QRect(40, 115, 81, 16));
        lbP1x->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbP1x->setTextFormat(Qt::RichText);
        lbCurrentMerge = new QLabel(wdgLeftBottom);
        lbCurrentMerge->setObjectName(QStringLiteral("lbCurrentMerge"));
        lbCurrentMerge->setGeometry(QRect(20, 30, 201, 16));
        lbCurrentMerge->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbCurrentMerge->setAlignment(Qt::AlignCenter);
        lbDistance = new QLabel(wdgLeftBottom);
        lbDistance->setObjectName(QStringLiteral("lbDistance"));
        lbDistance->setGeometry(QRect(60, 190, 191, 20));
        lbDistance->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbDistance->setTextFormat(Qt::RichText);
        lbDistanceIcon = new QLabel(wdgLeftBottom);
        lbDistanceIcon->setObjectName(QStringLiteral("lbDistanceIcon"));
        lbDistanceIcon->setGeometry(QRect(20, 180, 41, 31));
        lbDistanceIcon->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        lbP1z = new QLabel(wdgLeftBottom);
        lbP1z->setObjectName(QStringLiteral("lbP1z"));
        lbP1z->setGeometry(QRect(40, 155, 81, 16));
        lbP1z->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbP1z->setTextFormat(Qt::RichText);
        lbP2z = new QLabel(wdgLeftBottom);
        lbP2z->setObjectName(QStringLiteral("lbP2z"));
        lbP2z->setGeometry(QRect(140, 155, 81, 16));
        lbP2z->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbP2z->setTextFormat(Qt::RichText);
        lbP2x = new QLabel(wdgLeftBottom);
        lbP2x->setObjectName(QStringLiteral("lbP2x"));
        lbP2x->setGeometry(QRect(140, 115, 81, 16));
        lbP2x->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbP2x->setTextFormat(Qt::RichText);
        lbP1y = new QLabel(wdgLeftBottom);
        lbP1y->setObjectName(QStringLiteral("lbP1y"));
        lbP1y->setGeometry(QRect(40, 135, 81, 16));
        lbP1y->setStyleSheet(QStringLiteral("font: 9pt \"Nirmala UI\";"));
        lbP1y->setTextFormat(Qt::RichText);
        progressBar = new QProgressBar(wdgLeftBottom);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(20, 50, 201, 21));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        progressBar->setTextVisible(true);
        lbPin2 = new QLabel(wdgLeftBottom);
        lbPin2->setObjectName(QStringLiteral("lbPin2"));
        lbPin2->setGeometry(QRect(160, 85, 41, 31));
        lbPin2->setAlignment(Qt::AlignCenter);
        wdgBoxSize = new QWidget(wdgLeftBottom);
        wdgBoxSize->setObjectName(QStringLiteral("wdgBoxSize"));
        wdgBoxSize->setGeometry(QRect(10, 223, 231, 70));
        lbCompass = new QLabel(wdgBoxSize);
        lbCompass->setObjectName(QStringLiteral("lbCompass"));
        lbCompass->setGeometry(QRect(10, 10, 50, 50));
        lbWidth = new QLabel(wdgBoxSize);
        lbWidth->setObjectName(QStringLiteral("lbWidth"));
        lbWidth->setGeometry(QRect(100, 10, 47, 13));
        QFont font;
        font.setFamily(QStringLiteral("Nirmala UI"));
        font.setPointSize(9);
        lbWidth->setFont(font);
        lbWidthValue = new QLabel(wdgBoxSize);
        lbWidthValue->setObjectName(QStringLiteral("lbWidthValue"));
        lbWidthValue->setGeometry(QRect(150, 10, 47, 13));
        lbWidthValue->setFont(font);
        lbHeight = new QLabel(wdgBoxSize);
        lbHeight->setObjectName(QStringLiteral("lbHeight"));
        lbHeight->setGeometry(QRect(100, 30, 47, 13));
        lbHeight->setFont(font);
        lbHeightValue = new QLabel(wdgBoxSize);
        lbHeightValue->setObjectName(QStringLiteral("lbHeightValue"));
        lbHeightValue->setGeometry(QRect(150, 30, 47, 13));
        lbHeightValue->setFont(font);
        lbDepthValue = new QLabel(wdgBoxSize);
        lbDepthValue->setObjectName(QStringLiteral("lbDepthValue"));
        lbDepthValue->setGeometry(QRect(150, 50, 47, 13));
        lbDepthValue->setFont(font);
        lbDepth = new QLabel(wdgBoxSize);
        lbDepth->setObjectName(QStringLiteral("lbDepth"));
        lbDepth->setGeometry(QRect(100, 50, 47, 13));
        lbDepth->setFont(font);

        vlLeft->addWidget(wdgLeftBottom);


        gridLayout->addLayout(vlLeft, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Kinect 3D Modelling", 0));
#ifndef QT_NO_TOOLTIP
        btnLeftPlan->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Load Previous File</span></p><p>It will load and display the previous file in the list.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnLeftPlan->setText(QString());
        lbLoadedFile->setText(QApplication::translate("MainWindow", "Please browse a capture and open a file", 0));
#ifndef QT_NO_TOOLTIP
        btnRightPlan->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Load Next File</span></p><p>It will load and display the next file in the list.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnRightPlan->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnExitFS->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Minimize 3D View</span></p><p>It will enlarge the central view.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnExitFS->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnHelpM->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Handling 3D Model</span></p><p>Mouse Scroll : Zoom or unzoom the model</p><p>Clic + Move Mouse : Rotates the model</p><p>Shift + Move Mouse : Moves the model</p><p>Ctrl + Move Mouse : Rotates the model</p><p>Shift + Clic : Puts a pin for measurement (place two pins to get the distance between pins)</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnHelpM->setText(QString());
        gif3D->setText(QApplication::translate("MainWindow", "Loading", 0));
        lbInvisible->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnFS->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Maximize 3D View</span></p><p>It will reduce the central view.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnFS->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnResetCamera->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Reset 2D Views</span></p><p>It will reset the camera of the 2D views.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnResetCamera->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnHelpR->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Handling 3D model</span></p><p>Mouse Scroll : Zoom or unzoom the model</p><p>Clic + Move Mouse : Rotates the model</p><p>Shift + Move Mouse : Moves the model</p><p>Ctrl + Move Mouse : Rotates the model</p><p>Shift + Clic : Puts a pin for measurement (place two pins to get the distance between pins)</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnHelpR->setText(QString());
        gifLateral->setText(QApplication::translate("MainWindow", "Loading", 0));
        gifPlan->setText(QApplication::translate("MainWindow", "Loading", 0));
#ifndef QT_NO_TOOLTIP
        btnAdd->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Add a 3D plan to this capture</span></p><p>It will copy the selected PLY file into the capture's folder.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnAdd->setText(QString());
        btnMerge->setText(QApplication::translate("MainWindow", "Merge", 0));
        btnShow->setText(QApplication::translate("MainWindow", "Show", 0));
#ifndef QT_NO_TOOLTIP
        btnDelete->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Removes a 3D plan from this capture</span></p><p>It will remove the selected PLY file from the capture's folder.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        btnDelete->setText(QString());
        lbCaptureName->setText(QString());
        btnBrowse->setText(QApplication::translate("MainWindow", "Browse", 0));
        invisible2->setText(QString());
        lbPin1->setText(QString());
        lbP2y->setText(QApplication::translate("MainWindow", "Y2", 0));
        lbP1x->setText(QApplication::translate("MainWindow", "X1", 0));
        lbCurrentMerge->setText(QString());
        lbDistance->setText(QApplication::translate("MainWindow", "Distance: xx.xx m (xx.xx cm)", 0));
        lbDistanceIcon->setText(QString());
        lbP1z->setText(QApplication::translate("MainWindow", "Z1", 0));
        lbP2z->setText(QApplication::translate("MainWindow", "Z3", 0));
        lbP2x->setText(QApplication::translate("MainWindow", "X2", 0));
        lbP1y->setText(QApplication::translate("MainWindow", "Y1", 0));
        lbPin2->setText(QString());
        lbCompass->setText(QString());
        lbWidth->setText(QApplication::translate("MainWindow", "Width", 0));
        lbWidthValue->setText(QApplication::translate("MainWindow", "0.000", 0));
        lbHeight->setText(QApplication::translate("MainWindow", "Height", 0));
        lbHeightValue->setText(QApplication::translate("MainWindow", "0.000", 0));
        lbDepthValue->setText(QApplication::translate("MainWindow", "0.000", 0));
        lbDepth->setText(QApplication::translate("MainWindow", "Depth", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
