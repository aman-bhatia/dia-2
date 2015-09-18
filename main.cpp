#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "feature.h"

using namespace std;
using namespace cv;

int TIME = 1000;    // in milisecond
double FPS = 10;
string path = "../DIA_Ass2/";


void morph_image(Mat &img1, Mat &img2)
{
    if (img1.size() != img2.size()){
        cout << "Program requires equal sized images.\n";
        exit(0);
    }

    int num_frames = (int)(FPS * TIME / 100);
    VideoWriter out(path + "output.mov", CV_FOURCC('m','p', '4', 'v'), FPS, img1.size());

    if(!out.isOpened()) {
        cout <<"Error! Unable to open video file for output." << endl;
        exit(-1);
    }

    double t = 0;
    for (int k=1; k<=num_frames; k++){
        Mat temp;
        addWeighted(img1,1-t,img2,t,0,temp);
        out << temp;
        t = (double)k/num_frames;
    }

    cout << "done\n\n";
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    image_path.push_back(path + "data/1.jpg");
    image_path.push_back(path + "data/2.jpg");

    for (int i=0; i<image_path.size(); i++){
        Mat temp = imread(image_path[i]);

        if (temp.empty()){
            qDebug() << "Unable to open image" << image_path[i].data() << endl;
            exit(0);
        }

        image.push_back(temp);
    }

    namedWindow( window_name, CV_WINDOW_OPENGL | CV_WINDOW_AUTOSIZE);		// Create a window for display.
    setMouseCallback(window_name, draw, NULL);                              //set the callback function for any mouse event
    morph_image(image[0],image[1]);

    return a.exec();
}
