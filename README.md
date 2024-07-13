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

cmake ../lib/opencv-4.10.0 -DBUILD_EXAMPLES=ON

cmake --build .

make -j4

sudo make install

cd .. && cmake .

make

```

### Configure YOLOv8 ONNX Model in OpenCV

```bash
cd build

git clone https://github.com/opencv/opencv_extra.git

cd opencv_extra/testdata/dnn

python3 download_models.py yolov8n

cd ..

export OPENCV_TEST_DATA_PATH=$(pwd)

cd ../..

./bin/example_dnn_yolo_detector --model=onnx/models/yolov8n.onnx --yolo=yolov8 --mean=0.0 --scale=0.003921568627 --paddingmode=2 --padvalue=144.0 --thr=0.5 --nms=0.4 --rgb=0

```

`./WorkBuddy`
