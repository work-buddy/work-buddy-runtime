# work-buddy-runtime

A custom Linux based OpenCV C++ runtime for powering the Work Buddy XR HUD.

## Local Setup

Target Operating System: Ubuntu 24 LTS

Target GPU: Nvidia RTX 2080 Super

### Install Nvidia CUDA Toolkit

https://gist.github.com/raulqf/f42c718a658cddc16f9df07ecc627be7
https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=24.04&target_type=deb_local

```bash
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-ubuntu2404.pin
sudo mv cuda-ubuntu2404.pin /etc/apt/preferences.d/cuda-repository-pin-600
wget https://developer.download.nvidia.com/compute/cuda/12.5.1/local_installers/cuda-repo-ubuntu2404-12-5-local_12.5.1-555.42.06-1_amd64.deb
sudo dpkg -i cuda-repo-ubuntu2404-12-5-local_12.5.1-555.42.06-1_amd64.deb
sudo cp /var/cuda-repo-ubuntu2404-12-5-local/cuda-*-keyring.gpg /usr/share/keyrings/
sudo apt update
sudo apt -y install cuda-toolkit-12-5
sudo apt install -y nvidia-driver-555-open
sudo apt install -y cuda-drivers-555
sudo apt install nvidia-cuda-toolkit
```

### Install Nvidia cuDNN

https://developer.nvidia.com/cudnn-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=22.04&target_type=deb_local

```bash
wget https://developer.download.nvidia.com/compute/cudnn/9.2.1/local_installers/cudnn-local-repo-ubuntu2204-9.2.1_1.0-1_amd64.deb
sudo dpkg -i cudnn-local-repo-ubuntu2204-9.2.1_1.0-1_amd64.deb
sudo cp /var/cudnn-local-repo-ubuntu2204-9.2.1/cudnn-*-keyring.gpg /usr/share/keyrings/
sudo apt update
sudo apt -y install cudnn
sudo apt -y install cudnn-cuda-12
```

### Build and Install OpenCV

https://docs.opencv.org/4.10.0/d7/d9f/tutorial_linux_install.html

```bash
sudo apt update && sudo apt dist-upgrade
sudo apt install -y cmake make g++ pkg-config wget unzip
sudo apt install -y libjpeg-dev libpng-dev libtiff-dev
sudo apt install -y libavcodec-dev libavformat-dev libswscale-dev
sudo apt install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt install -y libxvidcore-dev x264 libx264-dev libfaac-dev libmp3lame-dev libtheora-dev
sudo apt install -y libfaac-dev libmp3lame-dev libvorbis-dev
sudo apt install -y libopencore-amrnb-dev libopencore-amrwb-dev
sudo apt install -y libgtk-3-dev
sudo apt install -y libtbb-dev
sudo apt install -y libatlas-base-dev gfortran
sudo apt install -y libprotobuf-dev protobuf-compiler
sudo apt install -y libgoogle-glog-dev libgflags-dev
sudo apt install -y libgphoto2-dev libeigen3-dev libhdf5-dev doxygen
sudo apt install -y libyaml-cpp-dev
sudo apt install libjsoncpp-dev
sudo apt install libcurl4-openssl-dev
mkdir -p build
mkdir -p lib
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.10.0.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/refs/tags/4.10.0.zip
unzip opencv.zip -d lib
unzip opencv_contrib.zip -d lib
cd build
sudo cmake ../lib/opencv-4.10.0 -D CMAKE_BUILD_TYPE=RELEASE \
-D BUILD_EXAMPLES=ON \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D BUILD_opencv_cudacodec=OFF \
-D WITH_CUDA=ON \
-D ENABLE_FAST_MATH=1 \
-D CUDA_FAST_MATH=1 \
-D WITH_CUBLAS=1 \
-D WITH_TBB=ON \
-D WITH_CUDNN=ON \
-D OPENCV_DNN_CUDA=ON \
-D WITH_GSTREAMER=ON \
-D WITH_OPENGL=ON \
-D WITH_V4L=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D CUDA_ARCH_BIN=7.5 \
-D OPENCV_EXTRA_MODULES_PATH=../lib/opencv_contrib-4.10.0/modules \
-D CUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda \
-D CUDNN_INCLUDE_DIR=/usr/include \
-D CUDNN_LIBRARY=/usr/lib/x86_64-linux-gnu/libcudnn.so \
-D GLOG_INCLUDE_DIR=/usr/include
sudo make -j16
sudo make install
cd .. && cmake .
make
```

### Run the Example YOLOv8 ONNX Model in OpenCV to test your setup.

```bash
cd build
git clone https://github.com/opencv/opencv_extra.git
cd opencv_extra/testdata/dnn
python3 download_models.py yolov8x
cd ..
export OPENCV_TEST_DATA_PATH=$(pwd)
cd ../../..
./build/bin/example_dnn_yolo_detector --model=models/onnx/yolov8x.onnx --yolo=yolov8 --mean=0.0 --scale=0.003921568627 --paddingmode=2 --padvalue=144.0 --thr=0.4 --nms=0.4 --rgb=0 --backend=5 --target=6
```

This should run smooth on target hardware, else your not on GPU and CUDA failed, re-install drivers, check your OpenCV build logs.

### Export other YOLOv8 models to ONNX

Only the detection module is available on package managers. -cls -pose -seg, etc need to download and export ourselves.
Find all the models for download in YOLO's Github releases:
https://github.com/ultralytics/assets/releases  
https://docs.ultralytics.com/models/yolov8/  
https://docs.ultralytics.com/modes/export/

```bash
python3 utils/yolo-onnx-export.py
mv models/pre-trained/yolov8x-cls.onnx models/onnx/
```

(opencv example will only work with detection, -pose, -cls, etc need custom rendering)

### Troubleshooting

I had to turn secure boot off on my machine for Nvidia drivers, TODO: make secure.

### Run Executable

1. Create `.env` file in root folder. Use `env.example` as template.

2. `chmod +x workbuddy.sh`

3. `./workbuddy.sh`

### Python AI Modules

More info on our local Python AI model microservice architecture in the [Python README](./python/README.md)
