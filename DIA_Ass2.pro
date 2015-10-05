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

LIBS += -L/usr/local/lib/ -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_adas -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_datasets -lopencv_face -lopencv_latentsvm -lopencv_objdetect -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_surface_matching -lopencv_text -lopencv_tracking -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_ml -lopencv_flann -lopencv_xobjdetect -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core -lopencv_hal

SOURCES += main.cpp\
        mainwindow.cpp \
    photo.cpp \
    application.cpp \
    morpher.cpp \
    manualfeature.cpp \
    waitingspinnerwidget.cpp

HEADERS  += mainwindow.h \
    photo.h \
    application.h \
    morpher.h \
    manualfeature.h \
    waitingspinnerwidget.h

FORMS    += mainwindow.ui
