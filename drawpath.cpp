#include "drawpath.h"

bool left_button_down = false;
string winname = "Draw Path";
vector<Point> path(0);
vector<Point> vertices(0);
int w = 700, h = 700;
Mat path_img = Mat(h,w,DataType<Vec3b>::type);


//return area of triangle
float area(Point a, Point b, Point c)
{
	return abs((a.x*(b.y-c.y) + b.x*(c.y-a.y)+ c.x*(a.y-b.y))/2.0);
}

// check if point lies inside triangle
bool isInside(Point a, Point b, Point c, Point p)
{
	float A = area (a,b,c);
	float A1 = area (p,b,c);
	float A2 = area (a,p,c);
	float A3 = area (a,b,p);

	return (A == A1 + A2 + A3);
}

void path_draw(int event, int x, int y, int flags, void* param){
	if (event == CV_EVENT_LBUTTONDOWN){
		left_button_down = true;
	} else if (event == CV_EVENT_MOUSEMOVE && left_button_down){
		circle(path_img, Point(x,y), 4, RED, -1);
		path.push_back(Point(x,y));
		imshow(winname,path_img);
	} else if (event == CV_EVENT_LBUTTONUP){
		left_button_down = false;
	}
}

void main_drawpath_function()
{
	vertices.clear();
	path_img = GRAY;
	Point center(w/2,h/2);
	circle(path_img,center,10,BLOOD_RED,-1);
	int rad = w/3;
	int num_images = app->num_images;

	for (int i=0; i<num_images; i++){
		double theta = i*(2*3.1416/num_images);
		int deltax = (int) rad*sin(theta);
		int deltay = (int) rad*cos(theta);
		Point p = center - Point(deltax, deltay);
		vertices.push_back(p);
	}

	for (int i=0; i<num_images; i++){
		if (i==0){
			line(path_img,vertices.at(num_images-1),vertices.at(i),BLOOD_RED,6);
		} else {
			line(path_img,vertices.at(i-1),vertices.at(i),BLOOD_RED,6);
		}
	}

	// set icon
	int icon_dim = 100;
	for(int i=0;i<num_images;i++){
		Mat icon = app->photos[app->order[i]].img;
		cv::resize(icon,icon,Size(icon_dim, icon_dim));

		cv::Rect roi( vertices.at(i) - Point(icon_dim/2,icon_dim/2), Size(icon_dim, icon_dim) );
		icon.copyTo(path_img(roi));
		circle(path_img,vertices.at(i),icon_dim/2+10,BLOOD_RED,20);
	}

	namedWindow(winname);
	setMouseCallback( winname, path_draw, NULL);
	imshow(winname, path_img);
	waitKey();
	destroyAllWindows();
}

