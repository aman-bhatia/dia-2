#include "feature.h"

Mat img;
string window_name = "Hello there";
vector<string> image_path(0);
vector<Mat> image(0);
vector< vector< Coord> > feature_points(0);
int current_image = -1;


void draw(int event, int x, int y, int flags, void* param){
    if (event == CV_EVENT_LBUTTONDOWN){

        // draw circle on the image
        circle(img, Point(x,y), 7, RED,-1);

        // check if feature_points contain vector for current image
        if (current_image == feature_points.size()){
            vector< Coord > temp(0);
            temp.push_back(make_pair(0,0));
            temp.push_back(make_pair(img.cols-1,0));
            temp.push_back(make_pair(img.cols-1,img.rows-1));
            temp.push_back(make_pair(0,img.rows-1));
            feature_points.push_back(temp);
        }

        feature_points[current_image].push_back(make_pair(x,y));
        imshow(window_name,img);
    }
}

void setFeatures(){
    namedWindow( window_name, CV_WINDOW_OPENGL | CV_WINDOW_AUTOSIZE);		// Create a window for display.
    setMouseCallback(window_name, draw, NULL);                              //set the callback function for any mouse event

    for (int i=0;i<image.size(); i++){
        current_image = i;
        img = image[i].clone();
        imshow(window_name,img);
        if (waitKey() == 'd'){
            cout << "d pressed" << endl;
        }
        if (i == image.size()-1){
            destroyAllWindows();
            current_image = -1;
        }
    }
}


void draw_subdiv_point( Mat& img, Point2f fp)
{
    circle( img, fp, 5, RED , -1);
}

void draw_subdiv( Mat& img, Subdiv2D& subdiv )
{
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);

    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
        line(img, pt[0], pt[1], BLUE, 1, LINE_AA, 0);
        line(img, pt[1], pt[2], BLUE, 1, LINE_AA, 0);
        line(img, pt[2], pt[0], BLUE, 1, LINE_AA, 0);
    }
}

void triangulate(){
    window_name = "Delaunay Triangulation";
    namedWindow( window_name, CV_WINDOW_OPENGL | CV_WINDOW_AUTOSIZE);		// Create a window for display.
    for (int i=0;i<image.size(); i++){
        img = image[i].clone();
        Rect rect(0,0,img.cols, img.rows);
        Subdiv2D subdiv(rect);
//        imshow(window_name, img);

        for(int j=0; j<feature_points[i].size(); j++){
            Point2f fp( (float)feature_points[i][j].first,
                        (float)feature_points[i][j].second);

            draw_subdiv_point(img,fp);
            subdiv.insert(fp);

            if (j == feature_points[i].size()-1){
                draw_subdiv(img,subdiv);
                imshow(window_name,img);
                waitKey();
            }
        }

        if (i == image.size()-1){
            destroyAllWindows();
        }
    }
}
