#ifndef MANUALFEATURE_H
#define MANUALFEATURE_H

#include "application.h"
#include "mainwindow.h"

class ManualFeature
{
public:
	std::string window_name;

	ManualFeature();
	~ManualFeature();
	void setFeatures();
};

#endif // MANUALFEATURE_H
