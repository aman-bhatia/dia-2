#include "morpher.h"

Morpher::Morpher(){
  TIME = 2;    // in second
  FPS = 10;
  make_video = false;
  window_name = "Image Morphing";
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

Point3f getBarycentricCoord(Point2f p, vector<Point2f> &triangle)
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

void print_triangle(vector< Point2f > &triangle){
  qDebug() << triangle[0].x << " " << triangle[0].y;
  qDebug() << triangle[1].x << " " << triangle[1].y;
  qDebug() << triangle[2].x << " " << triangle[2].y;
}

void print_triangle(vector< Coord > &triangle){
  qDebug() << triangle[0].first << " " << triangle[0].second;
  qDebug() << triangle[1].first << " " << triangle[1].second;
  qDebug() << triangle[2].first << " " << triangle[2].second;
}


void Morpher::morph_photos(Photo &photo1, Photo &photo2, string name)
{
  VideoWriter out("../DIA_Ass2/parts/" + name +".mov", CV_FOURCC('m','p', '4', 'v'), FPS, photo1.img.size());
  if(!out.isOpened()) {
    qDebug() <<"Error! Unable to open video file for output.";
    exit(0);
  }

  int num_frames = (int)(FPS * TIME);
  double t = 0;

  Mat intermediate(app->height, app->width, DataType<Vec3b>::type);

  vector< Point2f > intermediate_fp(0);

  for (int k=1; k<=num_frames; k++){
    intermediate_fp.clear();
    Rect rect(0,0,app->width, app->height);
    Subdiv2D intermediate_subdiv(rect);
    for(unsigned int i=0; i<photo1.fp.size(); i++){
      Point2f temp;
      temp.x = (1-t)*photo1.fp[i].first + t*photo2.fp[i].first;
      temp.y = (1-t)*photo1.fp[i].second + t*photo2.fp[i].second;

      intermediate_fp.push_back(temp);
      intermediate_subdiv.insert(temp);
    }

    for (int i=1; i<app->height-1; i++){
      for (int j=1; j<app->width-1; j++){
        Coord coord1, coord2;
        vector< Coord > triangle_in_img1, triangle_in_img2;
        vector< Point2f > triangle = getTriangle(make_pair(j,i), intermediate_subdiv);

        if (triangle.size() == 3){
          Point3f bc = getBarycentricCoord(Point2f(j,i),triangle);
          triangle_in_img1 = mapTriangle(triangle,intermediate_fp,photo1.fp);
          triangle_in_img2 = mapTriangle(triangle,intermediate_fp,photo2.fp);
          coord1 = getCartesianCoord(bc,triangle_in_img1);
          coord2 = getCartesianCoord(bc,triangle_in_img2);
        } else {
          size_t pos = find(intermediate_fp.begin(), intermediate_fp.end(), Point2f(j,i)) - intermediate_fp.begin();
          coord1 = photo1.fp[pos];
          coord2 = photo2.fp[pos];
        }

        if (coord1.first > intermediate.cols ||\
            coord2.first > intermediate.cols ||\
            coord1.second > intermediate.rows ||\
            coord2.second > intermediate.rows ||\
            coord1.first < 0 ||\
            coord2.first < 0 ||\
            coord1.second < 0 ||\
            coord2.second < 0){
          qDebug() << "(j,i) : (" << j << ", "<< i << ")";
          qDebug() << "t : " << t;
          qDebug() << "triangle size : " << triangle.size();
          qDebug() << "Intermediate triangle";
          print_triangle(triangle);
          qDebug() << "\nImage 1 triangle ";
          print_triangle(triangle_in_img1);
          qDebug() << "\nImage 2 triangle ";
          print_triangle(triangle_in_img2);
        }
        if (coord1.first > intermediate.cols){
          qDebug() << 1;
          coord1.first = 0;
        }
        if (coord2.first > intermediate.cols){
          qDebug() << 2;
          coord2.first = 0;
        }
        if (coord1.second > intermediate.rows){
          qDebug() << 3;
          coord1.second = 0;
        }
        if (coord2.second > intermediate.rows){
          qDebug() << 4;
          coord2.second = 0;
        }
        if (coord1.first < 0){
          qDebug() << 5;
          coord1.first = 0;
        }
        if (coord2.first < 0){
          qDebug() << 6;
          coord2.first = 0;
        }
        if (coord1.second < 0){
          qDebug() << 7;
          coord1.second = 0;
        }
        if (coord2.second < 0){
          qDebug() << 8;
          coord2.second = 0;
        }
        intermediate.at<Vec3b>(i,j) = ( ( (1-t) * (photo1.img.at<Vec3b>(coord1.second , coord1.first) ) )\
                                        + ( t * (photo2.img.at<Vec3b>(coord2.second ,  coord2.first) ) ) );
      }
    }

    out << intermediate;
    t = (double)k/num_frames;
  }
}


void Morpher::merge_parts(string name)
{
  VideoWriter out("../DIA_Ass2/" + name +".mov", CV_FOURCC('m','p', '4', 'v'), FPS, app->photos[0].img.size());
  if(!out.isOpened()) {
    qDebug() << "Error! Unable to open video file for output.";
    exit(0);
  }

  for (int i=0; i<app->num_images; i++){
    string part_name = "../DIA_Ass2/parts/" + to_string(i) + "_" + to_string((i+1)%app->num_images) + ".mov";
    VideoCapture cap(part_name);
    if (!cap.isOpened()){
      qDebug() << "Error! Unable to open video file(" << QString::fromStdString(part_name) << ") for output.";
    }

    while(cap.isOpened()){
      Mat frame;
      cap.read(frame);
      if (frame.empty())
        break;

      out << frame;
    }
  }
}
