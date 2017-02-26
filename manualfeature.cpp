#include "manualfeature.h"

ManualFeature::ManualFeature(){}

ManualFeature::~ManualFeature(){}

string feature_setting_window_name = "Set Features";
Mat current_feature_setting_image;
int current_feature_setting_image_index = -1;

void draw(int event, int x, int y, int /*flags*/, void* /*param*/){
  if (event == EVENT_LBUTTONDOWN){
    circle(current_feature_setting_image, Point(x,y), 7, RED,-1);
    app->photos[current_feature_setting_image_index].fp.push_back(make_pair(x,y));
    imshow(feature_setting_window_name,current_feature_setting_image);
  }
}

void ManualFeature::setFeatures(){
  namedWindow( feature_setting_window_name, WINDOW_OPENGL | WINDOW_AUTOSIZE);
  setMouseCallback( feature_setting_window_name, draw, NULL);

  for (int i=0; i<app->num_images; i++){
    current_feature_setting_image_index = app->order[i];
    current_feature_setting_image = app->photos[current_feature_setting_image_index].img.clone();
    vector< Coord > current_fp = app->photos[current_feature_setting_image_index].fp;
    for (unsigned int j=0; j<current_fp.size(); j++){
      circle(current_feature_setting_image, Point(current_fp[j].first, current_fp[j].second), 7, RED,-1);
    }
    imshow(feature_setting_window_name,current_feature_setting_image);
    waitKey();
    if (i == app->num_images-1){
      destroyAllWindows();
      current_feature_setting_image_index = -1;
    }
  }
}
