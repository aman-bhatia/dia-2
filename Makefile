all:
	@g++ -std=c++11 *.cpp -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -lopencv_videostab -lopencv_stereo -lopencv_video -lopencv_ximgproc -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core