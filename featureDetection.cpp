#include "featureDetection.h"

void detect_features()
{
	vector<Mat> imgs(app->num_images);

	// Reduce noise with a kernel 3x3
	for (int i=0;i<app->num_images;i++){
		Mat temp;
		blur( app->photos[i].img, temp, Size(3,3) );
		Canny( temp,imgs[i], 55, 55*3, 3 );
	}

	int division = 7;
	for (int m=0;m<division;m++){
		for (int n=0;n<division;n++){
			vector< vector<Point> > img_edges(app->num_images);
			for (int k=0; k<app->num_images;k++){
				vector<Point> edges(0);
				for (int i=1;i<imgs[k].rows/division -1;i++){
					for(int j=1;j<imgs[k].cols/division -1;j++){
						if (imgs[k].at<uchar>(m*imgs[k].rows/division + i, n*imgs[k].cols/division + j) == 255){
							edges.push_back(Point(n*imgs[k].cols/division + j, m*imgs[k].rows/division + i));
						}
					}
				}
				if (edges.size() == 0){
					break;
				} else {
					img_edges[k] = edges;
				}

				if (k == app->num_images-1){
					for (int z=0; z<app->num_images;z++){
						Point temp = img_edges[z][rand() % img_edges[z].size()];
						Coord fpoint(temp.x,temp.y);
						app->photos[z].fp.push_back(fpoint);
						circle(imgs[z],temp,3,WHITE,-1);
					}
				}
			}
		}
	}

//	for (int i=0;i<app->num_images;i++){
//		imshow("Detected Features",imgs[i]);
//		waitKey();
//	}
}
