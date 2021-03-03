https://seesaawiki.jp/asama-yaya/d/OpenCVaruco%A5%DE%A1%BC%A5%AB

https://seesaawiki.jp/asama-yaya/d/opencv%a4%c8aruco%a4%ce%a5%a4%a5%f3%a5%b9%a5%c8%a1%bc%a5%eb

https://docs.opencv.org/3.2.0/d5/d07/tutorial_charuco_diamond_detection.html


https://kinacon.hatenablog.com/entry/2018/10/11/131047

opencv_contrib$ git checkout -b 3.4.13 refs/tags/3.4.13

https://github.com/opencv/opencv/archive/3.4.13.zip

sudo docker build -t camera-pose-estimator -f Dockerfile .
sudo docker run --privileged -it -v $PWD:/app -v /tmp/.X11-unix/:/tmp/.X11-unix --device=/dev/video0:/dev/video0:mwr -e DISPLAY=$DISPLAY camera-pose-estimator 


# opencv build
cmake -D WITH_V4L=OFF -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
make install


sys/videoio.h not found -> -D WITH_V4L=OFF

CMake Error at cmake/OpenCVModule.cmake:339 (message):
  Duplicated modules NAMES has been found
Call Stack (most recent call first):
  cmake/OpenCVModule.cmake:357 (_assert_uniqueness)
  modules/CMakeLists.txt:7 (ocv_glob_modules)




  libv4l2: error setting pixformat: Input/output error
HIGHGUI ERROR: libv4l unable to ioctl S_FMT
libv4l2: error setting pixformat: Input/output error
libv4l1: error setting pixformat: Input/output error
HIGHGUI ERROR: libv4l unable to ioctl VIDIOCSPICT


https://stackoverflow.com/questions/24705101/opencv-videocapture-not-opening
cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/usr/local -DWITH_GTK=ON -DWITH_FFMPEG=1 
sudo apt-get install libav-tools libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libx264-dev


###############
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(3.4.13) /app/opencv/opencv-3.4.13/modules/highgui/src/window_gtk.cpp:624: error: (-2:Unspecified error) Can't initialize GTK backend in function 'cvInitSystem'

- host pc
xhost local: