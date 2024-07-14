# work-buddy-runtime

A custom Linux based OpenCV C++ runtime for powering the Work Buddy XR HUD.

## Local Setup

Target Operating System: Ubuntu 24 LTS
Target GPU: Nvidia RTX 2080 Super

## Install Nvidia CUDA Toolkit

https://gist.github.com/raulqf/f42c718a658cddc16f9df07ecc627be7
https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=24.04&target_type=deb_local
```bash
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-ubuntu2404.pin
sudo mv cuda-ubuntu2404.pin /etc/apt/preferences.d/cuda-repository-pin-600
wget https://developer.download.nvidia.com/compute/cuda/12.5.1/local_installers/cuda-repo-ubuntu2404-12-5-local_12.5.1-555.42.06-1_amd64.deb
sudo dpkg -i cuda-repo-ubuntu2404-12-5-local_12.5.1-555.42.06-1_amd64.deb
sudo cp /var/cuda-repo-ubuntu2404-12-5-local/cuda-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cuda-toolkit-12-5
sudo apt-get install -y nvidia-driver-555-open
sudo apt-get install -y cuda-drivers-555
```

## Install Nvidia cuDNN

https://developer.nvidia.com/cudnn-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=22.04&target_type=deb_local
```bash
wget https://developer.download.nvidia.com/compute/cudnn/9.2.1/local_installers/cudnn-local-repo-ubuntu2204-9.2.1_1.0-1_amd64.deb
sudo dpkg -i cudnn-local-repo-ubuntu2204-9.2.1_1.0-1_amd64.deb
sudo cp /var/cudnn-local-repo-ubuntu2204-9.2.1/cudnn-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cudnn
sudo apt-get -y install cudnn-cuda-12
```

### Build and Install OpenCV

https://docs.opencv.org/4.10.0/d7/d9f/tutorial_linux_install.html

```bash
sudo apt update && sudo apt install -y cmake make g++ libgtk2.0-dev pkg-config wget unzip
mkdir -p build
mkdir -p lib
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.10.0.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/refs/tags/4.10.0.zip
unzip opencv.zip -d lib
unzip opencv_contrib.zip -d lib
cd build
cmake ../lib/opencv-4.10.0 -D BUILD_EXAMPLES=ON \
-D WITH_CUDA=ON \
-D ENABLE_FAST_MATH=1 \
-D CUDA_FAST_MATH=1 \
-D WITH_TBB=ON \
-D WITH_CUDNN=ON \
-D OPENCV_DNN_CUDA=ON \
-D CUDA_ARCH_BIN=7.5 \
-D OPENCV_EXTRA_MODULES_PATH=../lib/opencv_contrib-4.10.0/modules
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
./bin/example_dnn_yolo_detector --model=onnx/models/yolov8n.onnx --yolo=yolov8 --mean=0.0 --scale=0.003921568627 --paddingmode=2 --padvalue=144.0 --thr=0.5 --nms=0.4 --rgb=0 --backend=5 --target=6

```

`./WorkBuddy`
