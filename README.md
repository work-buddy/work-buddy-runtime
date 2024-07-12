# work-buddy-runtime

A custom Linux based OpenCV C++ runtime for powering the Work Buddy XR HUD.

## Local Setup

Target Operating System: Ubuntu 24 LTS

### Build and Install OpenCV

https://docs.opencv.org/4.10.0/d7/d9f/tutorial_linux_install.html

```bash

sudo apt update && sudo apt install -y cmake make g++ libgtk2.0-dev pkg-config wget unzip

wget -O opencv.zip https://github.com/opencv/opencv/archive/4.10.0.zip

mkdir -p lib

unzip opencv.zip -d lib

mkdir -p build && cd build

cmake ../lib/opencv-4.10.0

cmake --build .

make -j4

sudo make install

cd .. && cmake .

make

```

`./WorkBuddy work-buddy-square.png`
