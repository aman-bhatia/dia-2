#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawpath.h"
#include "featureDetection.h"

Application* app;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->progressBar->hide();
	ui->checkBox_warp->hide();
	ui->pushButton_man_feature->hide();
	ui->pushButton_auto_feature->hide();
	ui->pushButton_make_video->hide();
	ui->pushButton_path->hide();
}

MainWindow::~MainWindow()
{
	delete ui;
}

// choose images push button
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

		ui->pushButton_proceed->setEnabled(true);
		QMessageBox::information(this, tr("Usage Information"),
														 tr("Reorder Images to order you want to morph them..."));
	} else {
		QMessageBox::information(this, tr("Usage Information"),tr("Please choose atleast two images"));
	}
}

// proceed push button
void MainWindow::on_pushButton_proceed_clicked()
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
	ui->pushButton_proceed->hide();
	ui->label->hide();

	ui->checkBox_warp->show();
	ui->pushButton_man_feature->show();
	ui->pushButton_auto_feature->show();
	ui->pushButton_path->show();
	ui->pushButton_make_video->show();
}


void MainWindow::on_pushButton_man_feature_clicked()
{
	QApplication::setOverrideCursor(QCursor(QPixmap("../DIA_Ass2/images/pencil.png")));
	ManualFeature manualFeature;
	manualFeature.setFeatures();
	ui->pushButton_man_feature->setEnabled(false);
	ui->pushButton_make_video->setEnabled(true);
	QApplication::restoreOverrideCursor();
}

void postProcessVideo(){
	Morpher mymorpher;
	VideoCapture in("../DIA_Ass2/output.mov");
	if (!in.isOpened()){
		cout << "Unable to open input video\n\n";
		exit(0);
	}

	Mat img;
	in >> img;
	int r = img.rows;
	int c = img.cols;

	VideoWriter out("../DIA_Ass2/video.mov", CV_FOURCC('m','p', '4', 'v'), mymorpher.FPS, Size(c-2*pad,r-2*pad)) ;
	if(!out.isOpened()) {
		cout <<"Error! Unable to open video file for output.";
		exit(0);
	}

	Rect crop_region(pad,pad,img.cols - 2*pad,img.rows - 2*pad);
	while(!img.empty()){
		Mat dst = img(crop_region);
		out << dst;
		in >> img;
	}
}

void MainWindow::on_pushButton_make_video_clicked()
{
	ui->pushButton_make_video->setEnabled(false);
	ui->checkBox_warp->setEnabled(false);

	Morpher mymorpher;
	if (path.size() == 0){
		Point center(w/2,h/2);
		int rad = w/3;

		if (vertices.size() == 0){
			for (int i=0; i<app->num_images; i++){
				double theta = i*(2*3.1416/app->num_images);
				int deltax = (int) rad*sin(theta);
				int deltay = (int) rad*cos(theta);
				Point p = center - Point(deltax, deltay);
				vertices.push_back(p);
			}
		}

		for (unsigned int i=0; i<vertices.size();i++){
			double t=0;
			int num_images = 20;
			for (int k=0;k<num_images;k++){
				path.push_back((1-t)*vertices.at(i) + t*vertices.at((i+1)%vertices.size()));
				t += 1.0/num_images;
			}
		}
	}

	ui->progressBar->setRange(0,path.size());
	ui->progressBar->setValue(0);
	ui->progressBar->show();
	QApplication::processEvents();

	VideoWriter out("../DIA_Ass2/output.mov", CV_FOURCC('m','p', '4', 'v'), mymorpher.FPS, app->photos[0].img.size());
	if(!out.isOpened()) {
		qDebug() <<"Error! Unable to open video file for output.";
		exit(0);
	}

	if (vertices.size()==2){
		int processed_images = 1;
		for (unsigned int i=0; i<vertices.size();i++){
			double t=0;
			int num_images = 20;

			vector<Photo> photos(1,app->photos[i]);
			photos.push_back(app->photos[(i+1)%app->num_images]);

			for (int k=0;k<num_images;k++){
				ui->progress_label->setText(QString::fromStdString("Processing Image " + to_string(processed_images) + " of " + to_string(2*num_images)));
				ui->progressBar->setValue(processed_images);
				QApplication::processEvents();
				processed_images++;
				vector<float> weights(0);
				weights.push_back(1-t);
				weights.push_back(t);
				Mat interm;

				if (warp){
					interm = mymorpher.warp_photos(app->photos[app->order[0]],photos,weights);
				} else {
					interm = mymorpher.morph_photos(photos,weights);
				}

				out << interm;
				t += 1.0/num_images;
			}
		}
	} else {
		Point center(w/2,h/2);
		vector<float> weights(app->num_images,1.0/app->num_images);
		Mat center_img = mymorpher.morph_photos(app->photos,weights);
		vector< Coord > center_fp = mymorpher.getFeaturePoints(app->photos,weights);
		Photo center_photo("center",center_img,center_fp);

		for (unsigned int i=0;i<path.size();i++){
			ui->progress_label->setText(QString::fromStdString("Processing Image " + to_string(i+1) + " of " + to_string(path.size())));
			ui->progressBar->setValue(i+1);
			QApplication::processEvents();

			Point p = path[i];
			for (unsigned int j=0; j<vertices.size();j++){
				if (isInside(center,vertices.at(j),vertices.at((j+1) % vertices.size()),p)){
					vector<Point2f> triangle(0);
					triangle.push_back(center);
					triangle.push_back(vertices.at(j));
					triangle.push_back(vertices.at((j+1)%vertices.size()));
					Point3f bc = mymorpher.getBarycentricCoord(p,triangle);

					vector< Photo > ps(1,center_photo);
					ps.push_back(app->photos[app->order[j]]);
					ps.push_back(app->photos[app->order[(j+1)%vertices.size()]]);

					vector<float> weights(0);
					weights.push_back(bc.x);
					weights.push_back(bc.y);
					weights.push_back(bc.z);

					Mat interm;
					if (warp){
						interm = mymorpher.warp_photos(app->photos[app->order[0]],ps,weights);
					} else {
						interm = mymorpher.morph_photos(ps,weights);
					}
					out << interm;
				}
			}
		}
	}
	ui->progress_label->setText("Morphing Completed Successfully...");
	ui->progressBar->hide();
	out.release();
	postProcessVideo();
}

void MainWindow::on_checkBox_warp_clicked(bool checked)
{
	warp = checked;
}


void MainWindow::on_pushButton_quit_clicked()
{
	QApplication::quit();
}

void MainWindow::on_pushButton_path_clicked()
{
	QApplication::setOverrideCursor(QCursor(QPixmap("../DIA_Ass2/images/pencil.png")));
	main_drawpath_function();
	QApplication::restoreOverrideCursor();
}

void MainWindow::on_pushButton_auto_feature_clicked()
{
	//detect_features();

	for (int i=0;i<app->num_images;i++){
		vector< Coord > auto_fp = auto_feature_detect(app->photos[i].name);
		vector< Coord > automated_fp = map_fp(auto_fp);
		app->photos[i].fp.insert(app->photos[i].fp.begin(),automated_fp.begin(),automated_fp.end());
	}

	for (int i=1;i<app->num_images;i++){
		if (app->photos[i].fp.size() != app->photos[0].fp.size()){
			cout << "Unequal no. of feature points detected...\n";
			exit(0);
		}
	}

	ui->pushButton_auto_feature->setEnabled(false);
	ui->pushButton_make_video->setEnabled(true);

	for (int i=0;i<app->num_images;i++){
		Mat img = app->photos[i].img.clone();
		for (unsigned int j=0;j<app->photos[i].fp.size();j++){
			Coord temp = app->photos[i].fp[j];
			if (temp.first >=0 && temp.first < img.cols && temp.second>=0 && temp.second < img.rows)
				circle(img,Point(temp.first,temp.second),3,RED,-1);
			else
				cout << "hello, total fp : " << app->photos[i].fp.size() <<" and " << j << "th point is : " << temp.first << "," << temp.second << endl;
		}
		imshow("Detected Features",img);
		waitKey();
	}
	destroyAllWindows();
}

