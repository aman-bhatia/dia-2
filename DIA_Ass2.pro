#-------------------------------------------------
#
# Project created by QtCreator 2015-10-03T21:27:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DIA_Ass2
TEMPLATE = app

CONFIG += console

CONFIG += c++11

INCLUDEPATH += /usr/local/include/opencv /usr/local/include/opencv2

LIBS += -L/usr/local/lib/ -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_objdetect -lopencv_shape -lopencv_video -lopencv_calib3d -lopencv_features2d -lopencv_ml -lopencv_flann -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core

SOURCES += main.cpp\
        mainwindow.cpp \
    photo.cpp \
    application.cpp \
    morpher.cpp \
    manualfeature.cpp \
    drawpath.cpp \
    featureDetection.cpp

HEADERS  += mainwindow.h \
    photo.h \
    application.h \
    morpher.h \
    manualfeature.h \
    drawpath.h \
    featureDetection.h

FORMS    += mainwindow.ui
