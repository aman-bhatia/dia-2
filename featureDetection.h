#ifndef FEATUREDETECTION_H
#define FEATUREDETECTION_H

#include "mainwindow.h"

void detect_features();
vector< Coord > auto_feature_detect(string name);
vector< Coord > map_fp(vector< Coord > &fp);

#endif
