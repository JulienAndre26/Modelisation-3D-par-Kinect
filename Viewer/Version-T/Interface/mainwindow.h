#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QThread>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTextStream>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnBrowse_clicked();

    void on_btnMerge_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QStringList detectPlyFiles(QDir dirToImport);
    void readImportFile(QString import);

    QStringList list;
};

#endif // MAINWINDOW_H
