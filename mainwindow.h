#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QListView>
#include <QStandardItemModel>
#include <QInputDialog>

#include "application.h"
#include "photo.h"
#include "morpher.h"
#include "manualfeature.h"


extern Application* app;

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
  void on_pushButton_clicked();

	void on_pushButton_proceed_clicked();

	void on_checkBox_warp_clicked(bool checked);

	void on_pushButton_quit_clicked();

	void on_pushButton_path_clicked();

	void on_pushButton_man_feature_clicked();

	void on_pushButton_auto_feature_clicked();

	void on_pushButton_make_video_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
