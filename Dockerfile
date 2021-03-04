FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

# RUN apt-get update && \
#   apt-get install -y --no-install-recommends \
#     wget git cmake unzip \
#     build-essential \
#     libeigen3-dev libgtk-3-dev qt5-default freeglut3-dev libvtk6-qt-dev libtbb-dev \
#     libjpeg-dev libpng++-dev libtiff-dev libopenexr-dev libwebp-dev \
#     libhdf5-dev libopenblas-dev liblapacke-dev \
#   && rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
  apt-get install -y --no-install-recommends \
    wget git cmake unzip ca-certificates \
    build-essential gcc-8 g++-8 libboost-dev \
    libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libavresample-dev libswscale-dev \
    libjpeg-dev libpng-dev ffmpeg libv4l-dev libtbb-dev libopenblas-dev \
  && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 8 && \
  update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 8 && \
  update-alternatives --config gcc && \
  update-alternatives --config g++


# apt update
# apt install --no-install-recommends -y build-essential cmake nano \
# ca-certificates unzip wget \
# pkg-config \
# libgtk2.0-dev \
# libjpeg-dev libpng-dev \
# ffmpeg libavcodec-dev libavformat-dev libavresample-dev libswscale-dev \
# libv4l-dev \
# libtbb-dev

WORKDIR opencv
RUN git clone https://github.com/opencv/opencv_contrib.git && \
  cd opencv_contrib && \
  git checkout -b 3.4.13 refs/tags/3.4.13

RUN wget https://github.com/opencv/opencv/archive/3.4.13.zip && \
  unzip 3.4.13.zip && \
  cd opencv-3.4.13 && \
  mkdir build && \
  cd build && \
  cmake -D WITH_V4L=OFF -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules .. && \
  make install

WORKDIR /app