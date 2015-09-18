#include "feature.h"

Mat img;
string window_name = "Hello there";
vector<string> image_path(0);
vector<Mat> image(0);
vector< vector< Coord> > feature_points(0);
int current_image = -1;


void draw(int event, int x, int y, int flags, void* param){
    if (event == CV_EVENT_LBUTTONDOWN){
        circle(img, Point(x,y), 10, Scalar(255,0,0,0),-1);
        cout << "x : " << x << " y : " << y << endl;

        if (current_image == feature_points.size()){
            vector< Coord > temp(0);
            feature_points.push_back(temp);
        }

        feature_points[current_image].push_back(make_pair(x,y));
        imshow(window_name,img);
    }
}

void setFeatures(){
    for (int i=0;i<image.size(); i++){
        current_image = i;
        img = image[i].clone();
        imshow(window_name,img);
        if (waitKey() == 'd'){
            cout << "d pressed" << endl;

        }
        if (i == image.size()-1)
            destroyAllWindows();
    }
}
