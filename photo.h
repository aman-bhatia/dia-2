#ifndef PHOTO_H
#define PHOTO_H

#define Coord pair<int,int>
#define RED Scalar(0,0,255)
#define BLUE Scalar(255,0,0)
#define GREEN Scalar(0,255,0)
#define WHITE Scalar(255,255,255)
#define BLACK Scalar(0,0,0)

#include <vector>
#include "opencv2/opencv.hpp"
#include <QDebug>

using namespace std;
using namespace cv;

class Photo
{
public:
    string name;
    Mat img;
    vector< Coord > fp;
    Photo(QString _name);
};

#endif // PHOTO_H
