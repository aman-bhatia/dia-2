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


// Automatic feature detection
int eyerow = 0;
int mouthrow = 0;

int masksizer = 30;
int masksizec = 30;
int maxaverage = 0;
pair< Mat , Mat > initialise(Mat img)
{
	Mat abs_grad_x, abs_grad_y;
	Mat src_gray = img;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );

	/// Total Gradient (approximate)
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

	int gridsize = abs_grad_y.rows/16;
	vector<int> results;
	for(int i = 4 ; i < 9 ; i++ ){
		line(abs_grad_y,Point(0,i*gridsize),Point(abs_grad_y.cols,i*gridsize),Scalar(255),1);
		int sum = 0;
		for(int p = 0 ; p < gridsize ; ++p){
			for(int j = 0 ; j < abs_grad_x.cols ; ++j){
					sum += abs_grad_y.at<uchar>(i*gridsize +p,j);
			}
		}
		results.push_back(sum);
	}

	int max_location = 0, sec_max_loc = 0 , max_value = 0 , sec_max_value = 0;
	for(unsigned int i = 0 ; i < results.size() ; ++i)
	{
		if(results.at(i) > max_value)
		{
			sec_max_loc = max_location;
			sec_max_value = max_value;
			max_location = i;
			max_value = results.at(i);
		}
	}
//	cout << "max_value " << max_value << endl;
//	cout << "max_location " << max_location << endl;
//	cout << "second max_value " << sec_max_value << endl;
//	cout << "second max_location " << sec_max_loc << endl;

	circle(abs_grad_y,Point(0,2*gridsize+max_location*gridsize),3,255,-1);
	eyerow = 2*gridsize+max_location*gridsize;

	// mouth detection
	vector<int> results1;
	for(int i = 10 ; i < 16 ; i++ ){
		line(abs_grad_y,Point(0,i*gridsize),Point(abs_grad_y.cols,i*gridsize),Scalar(255),1);
		int sum = 0;
		for(int p = 0 ; p < gridsize ; ++p){
			for(int j = 0 ; j < abs_grad_x.cols ; ++j){
				sum += abs_grad_y.at<uchar>(i*gridsize +p,j);
			}
		}
		results1.push_back(sum);
	}
	int max_location1 = 0, sec_max_loc1 = 0 , max_value1 = 0 , sec_max_value1 = 0;
	for(unsigned int i = 0 ; i < results1.size() ; ++i)
	{
		if(results1.at(i) > max_value1)
		{
			sec_max_loc1 = max_location1;
			sec_max_value1 = max_value1;
			max_location1 = i;
			max_value1 = results1.at(i);
		}
	}
//	cout << "max_value1 " << max_value1 << endl;
//	cout << "max_location1 " << max_location1 << endl;
//	cout << "second max_value1 " << sec_max_value1 << endl;
//	cout << "second max_location1 " << sec_max_loc1 << endl;

	for(int i = 0 ; i < abs_grad_y.rows ; ++i)
	{
		for(int j = 0 ; j < abs_grad_y.cols ; ++j)
		{
			int q = 0;
			for(int a = -masksizer/2 ; a < masksizer/2 ; ++a)
				{
					for(int b = -masksizec/2 ; b < masksizec/2 ; ++b)
					{
						if((i+a >= 0) and (i + a < src_gray.rows) and (j+b >= 0) and (j+b < src_gray.cols))
							q += abs_grad_y.at<uchar>(i+a,j+b);
					}
				}
			int qav = q/81;
			if(qav > maxaverage)
				maxaverage = qav;
		}
	}
	circle(abs_grad_y,Point(0,abs_grad_y.rows-1-max_location1*20),2,255,-1);
	mouthrow = abs_grad_y.rows-1-max_location1*20;

//	imwrite("result.jpg",grad);
//	imwrite("resultx.jpg",abs_grad_x);
//	imwrite("resulty.jpg",abs_grad_y);
	return (make_pair(abs_grad_x, abs_grad_y));
}

vector< Coord > auto_feature_detect(string name)
{
	Mat src, src_gray , newer;
	src = imread(name);

	if( src.empty() ){
		cout << "Unable to open the image... Exitting." << endl;
		exit(0);
	}

	cvtColor( src, src_gray, CV_RGB2GRAY );
	pair < Mat , Mat > ans;
	ans = initialise(src_gray);
	Mat abs_grad_x = ans.first;
	Mat abs_grad_y = ans.second;
	newer = src_gray.clone();


	Mat grad = src_gray.clone();
	src_gray = newer.clone();
	float max_var = 0;
	float max_var2 = 0;
	float max_i = 0;
	float max_j = 0;
	float max_i2 = 0;
	float max_j2 = 0;
//	cout << "eye and mouth row " << eyerow << " " << mouthrow << endl;
	for(int i = eyerow ; i < (eyerow+mouthrow)/2 ; ++i)
	{
		for(int j = 3 ; j < src_gray.cols/2 ; ++j)
		{
				float sum_x = 0;
				float sum_x2 = 0;
				for(int a = -3 ; a < 4 ; ++a)
				{
					for(int b = -3 ; b < 4 ; ++b)
					{
						sum_x += src_gray.at<uchar>(i+a,j+b);
					}
				}
				float mean = sum_x / 49;
				for(int a = -3 ; a < 4 ; ++a)
				{
					for(int b = -3 ; b < 4 ; ++b)
					{
						sum_x2 += (src_gray.at<uchar>(i+a,j+b)-mean)*(src_gray.at<uchar>(i+a,j+b)-mean);
					}
				}
				float mean2 = sum_x2 / 49;
				float variance = mean2;
				int q = 0;
				for(int a = -masksizer/2 ; a < masksizer/2 ; ++a)
				{
					for(int b = -masksizec/2 ; b < masksizec/2 ; ++b)
					{
						if((i+a >= 0) and (i + a < src_gray.rows) and (j+b >= 0) and (j+b < src_gray.cols))
							q += abs_grad_y.at<uchar>(i+a,j+b);
					}
				}
				int localav = q/81;
				bool truth = (localav >(maxaverage/1.3));
				if((variance > max_var) and truth)
				{
					max_var = variance;
					max_i = i;
					max_j = j;
				}
				grad.at<uchar>(i,j) = variance ;
		}
	}
	for(int i = eyerow ; i < (eyerow+mouthrow)/2 ; ++i)
	{
		for(int j = src_gray.cols/2 ; j < src_gray.cols - 3 ; ++j)
		{
				float sum_x = 0;
				float sum_x2 = 0;
				for(int a = -3 ; a < 4 ; ++a)
				{
					for(int b = -3 ; b < 4 ; ++b)
					{
						sum_x += src_gray.at<uchar>(i+a,j+b);
					}
				}
				float mean = sum_x / 49;
				for(int a = -3 ; a < 4 ; ++a)
				{
					for(int b = -3 ; b < 4 ; ++b)
					{
						sum_x2 += (src_gray.at<uchar>(i+a,j+b)-mean)*(src_gray.at<uchar>(i+a,j+b)-mean);
					}
				}
				float mean2 = sum_x2 / 49;
				float variance = mean2;
				int q = 0;
				for(int a = -masksizer/2 ; a < masksizer/2 ; ++a)
				{
					for(int b = -masksizec/2 ; b < masksizec/2 ; ++b)
					{
						if((i+a >= 0) and (i + a < src_gray.rows) and (j+b >= 0) and (j+b < src_gray.cols))
							q += abs_grad_y.at<uchar>(i+a,j+b);
					}
				}
				int localav = q/81;
				bool truth = (localav >(maxaverage/1.5));
				if((variance > max_var2) and truth)
				{
					max_var2 = variance;
					max_i2 = i;
					max_j2 = j;
				}
				//grad.at<uchar>(i,j) = variance ;
		}
	}


	Mat other_features = src_gray.clone();
	threshold(abs_grad_y , other_features , 210 , 255 , 1);
	//imshow(window_name , other_features);
	//waitKey(0);
	int mouth_col = (max_j + max_j2) / 2;
	int mouth_row = (max_i + max_i2) / 2;
	int starter = 0 , end = 0;
	for(int i = mouth_row ; i < src.rows ; ++i)
	{
		if(starter == 0 and other_features.at<uchar>(i,mouth_col) == 0)
		{
//			cout << "hello" << endl;
			starter = i;
		}
		else if(other_features.at<uchar>(i,mouth_col) == 0)
			end = i;
	}
//	cout << starter << " " << end << endl;
	Mat extra_features = abs_grad_x.clone();
	threshold(extra_features , extra_features , 125 , 255 , THRESH_TOZERO);
	for(int i = 0 ; i < extra_features.rows ; ++i)
	{
		int prevloc = 0;
		for(int j = extra_features.cols/2 ; j < extra_features.cols ; ++j)
		{
			if(extra_features.at<uchar>(i,j) != 0)
			{
				extra_features.at<uchar>(i ,prevloc) = 0;
				prevloc = j;
			}
		}
		prevloc = 0;
		for(int j = extra_features.cols/2 ; j >= 0 ; --j)
		{
			if(extra_features.at<uchar>(i,j) != 0)
			{
				extra_features.at<uchar>(i ,prevloc) = 0;
				prevloc = j;
			}
		}
	}
	int number_divisions = 7;
	vector< pair<int , int > > features;
	vector< pair<int , int > > efeatures_below_left(number_divisions , make_pair(-1,-1));
	vector< pair<int , int > > efeatures_below_right(number_divisions , make_pair(-1,-1));
	vector< pair<int , int > > efeatures_above_left(number_divisions , make_pair(-1,-1));
	vector< pair<int , int > > efeatures_above_right(number_divisions , make_pair(-1,-1));
	circle(src,Point(max_j,max_i),2,Scalar(255,0,0));
	features.push_back(make_pair(max_j,max_i));
	circle(src,Point(max_j2,max_i2),2,Scalar(0,0,255));
	features.push_back(make_pair(max_j2,max_i2));
	circle(src,Point(mouth_col,starter),2,Scalar(0,255,0));
	features.push_back(make_pair(mouth_col,starter));
	circle(src,Point(mouth_col,end),2,Scalar(0,255,0));
	features.push_back(make_pair(mouth_col,end));
	circle(src,Point(max_j,(starter + 1.85*end) / 3),2,Scalar(0,255,0));
	features.push_back(make_pair(max_j,(starter + 1.85*end) / 3));
	circle(src,Point(max_j2,(starter + 1.85*end) / 3),2,Scalar(0,255,0));
	features.push_back(make_pair(max_j2,(starter + 1.85*end) / 3));
	//circle(src,Point(mouth_col,end + (-starter + end)/2),2,Scalar(0,255,0));
	//features.push_back(make_pair(mouth_col,end + (-starter + end)/2));
	circle(src,Point(mouth_col,(starter + end) / 2),2,Scalar(0,255,0));
	features.push_back(make_pair(mouth_col,(starter + end) / 2));
	circle(src,Point(mouth_col,((max_j2 - max_j)*2)/3 + max_i),2,Scalar(0,255,0));
	features.push_back(make_pair(mouth_col,((max_j2 - max_j)*2)/3 + max_i));
	int lower_limit = int((extra_features.rows - (max_i + max_i2)/2)/(number_divisions) + 1);
	int upper_limit = int(((max_i + max_i2)/2)/(number_divisions) + 1);

//	cout << "max_i " << max_i << endl;
//	cout << "max_i2 " << max_i2 << endl;
//	cout << "hi" << endl;
//	cout << "The upper_limit is " << upper_limit << endl;
//	cout << "The lower limit is " << lower_limit << endl;
	int temp_counter = 0;
	for(int i = (max_i + max_i2)/2 ; i < extra_features.rows ; i += lower_limit)
	{
		for(int j = extra_features.cols/2 ; j < extra_features.cols ; ++j)
		{
			if(extra_features.at<uchar>(i,j) != 0)
			{
				efeatures_below_right.at(temp_counter) = make_pair(j,i);
				circle(src,Point(j,i),2,Scalar(0,255,0));
			}
		}
		for(int j = extra_features.cols/2 ; j >= 0 ; --j)
		{
			if(extra_features.at<uchar>(i,j) != 0)
			{
				efeatures_below_left.at(temp_counter) = make_pair(j,i);
				circle(src,Point(j,i),2,Scalar(0,255,0));
			}
		}
		++temp_counter;
	}
	temp_counter = 0;
	for(int i = (max_i + max_i2)/2 ; i > 0 ; i -= upper_limit)
	{
		for(int j = extra_features.cols/2 ; j < extra_features.cols ; ++j)
		{
			if(extra_features.at<uchar>(i,j) != 0)
			{
				efeatures_above_right.at(temp_counter) = make_pair(j,i);
				circle(src,Point(j,i),2,Scalar(0,255,0));
			}
		}
		for(int j = extra_features.cols/2 ; j >= 0 ; --j)
		{
			if(extra_features.at<uchar>(i,j) != 0)
			{
				efeatures_above_left.at(temp_counter) = make_pair(j,i);
				circle(src,Point(j,i),2,Scalar(0,255,0));
			}
		}
		++temp_counter;
	}
//  circle(src,Point(max_j3+(max_j2 - max_j)/2,max_i3),2,Scalar(0,0,255));
//  circle(src,Point(max_j3-(max_j2 - max_j)/2,max_i3),2,Scalar(0,0,255));

	//imshow( window_name, src);
	//waitKey(0);
	//imshow(window_name , extra_features);
//	imwrite("det.jpg" , src);

	features.insert(features.end(),efeatures_above_left.begin(),efeatures_above_left.end());
	features.insert(features.end(),efeatures_above_right.begin(),efeatures_above_right.end());
	features.insert(features.end(),efeatures_below_left.begin(),efeatures_below_left.end());
	features.insert(features.end(),efeatures_below_right.begin(),efeatures_below_right.end());

	return features;
}



vector< Coord > map_fp(vector< Coord > &fp){
	vector< Coord > ret(0);
	for (unsigned int i=0;i<fp.size();i++){
		int a = fp[i].first;
		int b = fp[i].second;
		ret.push_back(make_pair(a+pad,b+pad));
	}
	return ret;
}
