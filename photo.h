#ifndef PHOTO_H
#define PHOTO_H

#define Coord pair<int,int>
#define RED Scalar(0,0,255)
#define BLOOD_RED Scalar(7,7,138)
#define BLUE Scalar(255,0,0)
#define GREEN Scalar(0,255,0)
#define WHITE Scalar(255,255,255)
#define BLACK Scalar(0,0,0)
#define GRAY Scalar(230,230,230)

#include <vector>
#include "opencv2/opencv.hpp"
#include <QDebug>

using namespace std;
using namespace cv;

extern int pad;

class Photo
{
public:
	string name;
	Mat img;
	vector< Coord > fp;
	Photo(string _name, Mat _img, vector< Coord > _fp);
	Photo(QString _name);
	~Photo();
};

#endif // PHOTO_H
