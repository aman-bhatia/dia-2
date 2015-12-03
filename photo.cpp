#include "photo.h"

int pad = 10;

Photo::Photo(string _name, Mat _img, vector< Coord > _fp){
	name = _name;
	img = _img;
	fp = _fp;
}

Photo::Photo(QString _name){
	name = _name.toStdString();
	Mat temp = imread(name);
	copyMakeBorder( temp, img, pad,pad,pad,pad, BORDER_CONSTANT, Scalar(255,255,255) );
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

Photo::~Photo(){
	fp.clear();
}
