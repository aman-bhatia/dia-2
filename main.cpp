#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int TIME = 1000; // in milisecond
double FPS = 10;

int main() {

	Mat img1 = imread("1.jpg");
	Mat img2 = imread("2.jpg");

	if (img1.size() != img2.size()){
		cout << "Program requires equal sized images.\n";
		exit(0);
	}
	int num_frames = (int)(FPS * TIME / 100);
	VideoWriter out("output.mov", CV_FOURCC('m','p', '4', 'v'), FPS, img1.size());
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
	return 0;
}