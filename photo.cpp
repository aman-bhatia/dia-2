#include "photo.h"

Photo::Photo(QString _name){
    name = _name.toStdString();
    Mat temp = imread(name);
    copyMakeBorder( temp, img, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0) );
    fp.clear();
    fp.push_back(make_pair(0,0));
    fp.push_back(make_pair(img.cols/2,0));
    fp.push_back(make_pair(img.cols-1,0));
    fp.push_back(make_pair(img.cols-1,img.rows/2));
    fp.push_back(make_pair(img.cols-1,img.rows-1));
    fp.push_back(make_pair(img.cols/2,img.rows-1));
    fp.push_back(make_pair(0,img.rows-1));
    fp.push_back(make_pair(0,img.rows/2));
}
