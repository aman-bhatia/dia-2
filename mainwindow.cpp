#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waitingspinnerwidget.h"


Application* app;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->progressBar->hide();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  QStringList ls = QFileDialog::getOpenFileNames(this,"Choose two or more images","",tr("Image Files (*.png *.jpg *.bmp)"));

  if (ls.size() >= 2){
    app = new Application(ls);

    //Creating a standard item model
    QStandardItemModel* model = new QStandardItemModel(this);

    for (int i=0;i<app->num_images;i++){
      QString s = QString::fromStdString(app->photos[i].name);
      QStandardItem* item = new QStandardItem(QIcon(s),s);
      item->setFlags(item->flags() ^ (Qt::ItemIsDropEnabled));
      model->appendRow(item);
    }

    //Setting the icon size
    ui->listView->setIconSize(QSize(60,60));

    //Setting the model
    ui->listView->setModel(model);

    //Setting listview geometry
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDragDropMode(QAbstractItemView::InternalMove);

    ui->pushButton_2->setEnabled(true);
    ui->label_2->setText("Reorder Images to order you want to morph images");
  } else {
    QMessageBox::information(this, tr("Usage Information"),tr("Please choose atleast two images"));
  }
}

void MainWindow::on_pushButton_2_clicked()
{
  QAbstractItemModel* model = ui->listView->model();
  app->order.clear();
  for (int i=0;i<model->rowCount(); i++){
    for (int j=0; j<app->num_images; j++){
      if (model->index(i,0).data().toString() == QString::fromStdString(app->photos[j].name))
        app->order.push_back(j);
    }
  }

  ui->listView->hide();
  ui->pushButton->hide();
  ui->pushButton_2->hide();
  ui->label->hide();
  ui->label_2->hide();
  ui->pushButton_4->hide();
}



void MainWindow::on_pushButton_3_clicked()
{
  ManualFeature manualFeature;
  manualFeature.setFeatures();
  ui->pushButton_3->setEnabled(false);
  ui->pushButton_4->show();
}

void MainWindow::on_pushButton_4_clicked()
{
//  WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget(this);
//  spinner->setRoundness(70.0);
//  spinner->setMinimumTrailOpacity(15.0);
//  spinner->setTrailFadePercentage(70.0);
//  spinner->setNumberOfLines(12);
//  spinner->setLineLength(20);
//  spinner->setLineWidth(5);
//  spinner->setInnerRadius(10);
//  spinner->setRevolutionsPerSecond(1);
//  spinner->setColor(QColor(81, 4, 71));
//  spinner->start();

  ui->progressBar->setRange(0,app->num_images);
  ui->progressBar->setValue(0);
  ui->progressBar->show();
  QApplication::processEvents();

  Morpher mymorpher;
  for (int i=0; i<app->num_images; i++){
    ui->progress_label->setText(QString::fromStdString("Processing Image " + to_string(i+1) + " of " + to_string(app->num_images)));
    ui->progressBar->setValue(i+1);
    QApplication::processEvents();
    mymorpher.morph_photos(app->photos[app->order[i]], app->photos[app->order[(i+1)%app->num_images]], to_string(i)+"_"+to_string((i+1)%app->num_images));
  }
  ui->progress_label->setText("Morphing Completed Successfully...");
  ui->progressBar->hide();
//  spinner->stop();
  mymorpher.merge_parts("output");
}

