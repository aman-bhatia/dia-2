#ifndef APPLICATION_H
#define APPLICATION_H

#include <QStringList>
#include <vector>
#include <photo.h>
#include <QMessageBox>
#include <QObject>

class Application
{
public:
    int num_images;
    int width;
    int height;
    std::vector<int> order;
    std::vector<Photo> photos;
    Application(QStringList ls);
		~Application();
};

#endif // APPLICATION_H
