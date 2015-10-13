#include "morpher.h"

bool warp = false;

Morpher::Morpher(){
	TIME = 2;    // in second
	FPS = 20;
}

vector< Point2f > getTriangle(Coord p, Subdiv2D &subdiv){

	vector< Point2f > toret(0);

	int e0=0, vertex=0;
	subdiv.locate(Point2f(p.first,p.second), e0, vertex);

	if(e0 > 0){
		int e = e0;
		do {
			Point2f org, dst;
			if( subdiv.edgeOrg(e, &org) > 0 && subdiv.edgeDst(e, &dst) > 0 )
				toret.push_back(org);

			e = subdiv.getEdge(e, Subdiv2D::NEXT_AROUND_LEFT);
		}
		while( e != e0 );
	}
	return toret;
}

float cross(Point2f a, Point2f b){
	return ( a.x*b.y - a.y*b.x);
}

Point3f Morpher::getBarycentricCoord(Point2f p, vector<Point2f> &triangle)
{
	float areaABC = cross(triangle[1] - triangle[0] , triangle[2] - triangle[0]);
	float areaPBC = cross(triangle[1] - p , triangle[2] - p);
	float areaPCA = cross(triangle[2] - p , triangle[0] - p);

	Point3f bary;
	bary.x = abs(areaPBC / areaABC) ; // alpha
	bary.y = abs(areaPCA / areaABC) ; // beta
	bary.z = 1.0 - bary.x - bary.y ; // gamma

	return bary ;
}

Coord getCartesianCoord(Point3f &bc, vector< Coord > &triangle)
{
	int x = (int)(bc.x*triangle[0].first + bc.y*triangle[1].first + bc.z*triangle[2].first);
	int y = (int)(bc.x*triangle[0].second + bc.y*triangle[1].second + bc.z*triangle[2].second);
	return make_pair(x,y);
}

vector< Coord > mapTriangle(vector<Point2f> &triangle,vector< Point2f > &intermediate_fp, vector< Coord> &orig_fp){
	vector< Coord > toret(0);
	auto p1 = find(intermediate_fp.begin(), intermediate_fp.end(), triangle[0]);
	auto p2 = find(intermediate_fp.begin(), intermediate_fp.end(), triangle[1]);
	auto p3 = find(intermediate_fp.begin(), intermediate_fp.end(), triangle[2]);

	size_t i = p1 - intermediate_fp.begin();
	toret.push_back(orig_fp[i]);
	i = p2 - intermediate_fp.begin();
	toret.push_back(orig_fp[i]);
	i = p3 - intermediate_fp.begin();
	toret.push_back(orig_fp[i]);

	return toret;
}

vector< Coord > Morpher::getFeaturePoints(vector<Photo> photos, vector<float> weights){
	vector< Coord > intermediate_fp(0);

	for(unsigned int i=0; i<photos[0].fp.size(); i++){
		Point2f temp(0,0);
		for (unsigned int j=0;j<photos.size();j++){
			temp.x += weights[j] * photos[j].fp[i].first;
			temp.y += weights[j] * photos[j].fp[i].second;
		}
		intermediate_fp.push_back( make_pair( (int) temp.x, (int) temp.y ) );
	}
	return intermediate_fp;
}

Mat Morpher::morph_photos(vector<Photo> &photos, vector<float> weights)
{
	Mat intermediate(app->height, app->width, DataType<Vec3b>::type);

	vector< Point2f > intermediate_fp(0);

	Rect rect(0,0,app->width, app->height);
	Subdiv2D intermediate_subdiv(rect);
	for(unsigned int i=0; i<photos[0].fp.size(); i++){
		Point2f temp(0,0);
		for (unsigned int j=0;j<photos.size();j++){
			temp.x += weights[j] * photos[j].fp[i].first;
			temp.y += weights[j] * photos[j].fp[i].second;
		}
		intermediate_fp.push_back(temp);
		intermediate_subdiv.insert(temp);
	}

	for (int i=1; i<app->height-1; i++){
		for (int j=1; j<app->width-1; j++){
			vector<Coord> coords(photos.size());

			vector< vector< Coord > > triangle_in_imgs(photos.size());
			vector< Point2f > triangle = getTriangle(make_pair(j,i), intermediate_subdiv);

			if (triangle.size() == 3){
				Point3f bc = getBarycentricCoord(Point2f(j,i),triangle);
				for (unsigned int k=0;k<photos.size();k++){
					triangle_in_imgs[k] = mapTriangle(triangle,intermediate_fp,photos[k].fp);
				}

				for (unsigned int k=0;k<photos.size();k++){
					coords[k] = getCartesianCoord(bc,triangle_in_imgs[k]);
				}
			} else {
				size_t pos = find(intermediate_fp.begin(), intermediate_fp.end(), Point2f(j,i)) - intermediate_fp.begin();
				for (unsigned int k=0; k<photos.size();k++){
					coords[k] = photos[k].fp[pos];
				}
			}

			for (unsigned int k=0; k<photos.size();k++){
				if (coords[k].first > intermediate.cols || coords[k].first < 0)
					coords[k].first = 0;
				if (coords[k].second > intermediate.rows || coords[k].first < 0)
					coords[k].second = 0;
			}

			Vec3b temp;
			temp[0] = 0;
			temp[1] = 0;
			temp[2] = 0;
			for (unsigned int k=0;k<photos.size();k++){
				temp += weights[k] * photos[k].img.at<Vec3b>(coords[k].second , coords[k].first);
			}
			intermediate.at<Vec3b>(i,j) = temp;
		}
	}
	return intermediate;
}



Mat Morpher::warp_photos(Photo orig_img,vector<Photo> &photos, vector<float> weights)
{
	Mat intermediate(app->height, app->width, DataType<Vec3b>::type);

	vector< Point2f > intermediate_fp(0);

	Rect rect(0,0,app->width, app->height);
	Subdiv2D intermediate_subdiv(rect);
	for(unsigned int i=0; i<photos[0].fp.size(); i++){
		Point2f temp(0,0);
		for (unsigned int j=0;j<photos.size();j++){
			temp.x += weights[j] * photos[j].fp[i].first;
			temp.y += weights[j] * photos[j].fp[i].second;
		}
		intermediate_fp.push_back(temp);
		intermediate_subdiv.insert(temp);
	}

	for (int i=1; i<app->height-1; i++){
		for (int j=1; j<app->width-1; j++){
			Coord coord1;

			vector< Coord > triangle_in_img1;
			vector< Point2f > triangle = getTriangle(make_pair(j,i), intermediate_subdiv);

			if (triangle.size() == 3){
				Point3f bc = getBarycentricCoord(Point2f(j,i),triangle);
				triangle_in_img1 = mapTriangle(triangle,intermediate_fp,orig_img.fp);

				coord1 = getCartesianCoord(bc,triangle_in_img1);
			} else {
				size_t pos = find(intermediate_fp.begin(), intermediate_fp.end(), Point2f(j,i)) - intermediate_fp.begin();
				coord1 = orig_img.fp[pos];
			}

			if (coord1.first > intermediate.cols || coord1.first < 0)
				coord1.first = 0;
			if (coord1.second > intermediate.rows || coord1.first < 0)
				coord1.second = 0;

			intermediate.at<Vec3b>(i,j) = orig_img.img.at<Vec3b>(coord1.second,coord1.first);
		}
	}
	return intermediate;
}
