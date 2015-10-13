#ifndef DRAWPATH_H
#define DRAWPATH_H

#include "mainwindow.h"

extern vector<Point> path;
extern vector<Point> vertices;
extern int w, h;

float area(Point a, Point b, Point c);
bool isInside(Point a, Point b, Point c, Point p);
void main_drawpath_function();

#endif
