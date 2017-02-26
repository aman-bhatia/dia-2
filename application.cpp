#include "application.h"

Application::Application(QStringList ls){
  num_images = ls.size();
  order.clear();
  photos.clear();

  photos.push_back(Photo(ls[0]));
  width = photos[0].img.cols;
  height = photos[0].img.rows;

  for (int i=1;i<num_images;i++){
    photos.push_back(Photo(ls[i]));

    if (photos.back().img.cols != width || photos.back().img.rows != height){
      QMessageBox::critical(NULL, QObject::tr("Error in sizes of images"),QObject::tr("Images are of different sizes. Please choose images of same sizes."));
      exit(0);
    }
  }
}

Application::~Application(){
	order.clear();
	photos.clear();
}
