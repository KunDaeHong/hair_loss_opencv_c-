#OPENCV Build
OPENCV_MAC_BUILD_DIR="$(dirname $0)/Libraries/opencv/build_opencv"
OPENCV_LIBRARY_OUTPUT="$(dirname $0)/Libraries/opencv_static"

#Project Build
PROJ_MAC_BUILD_DIR="$(dirname $0)/build_mac"
PROJ_LIBRARY_OUTPUT="$(dirname $0)/output_mac"

echo "OpenCV Build Folder Path ~==> $OPENCV_MAC_BUILD_DIR"
echo "Project Build Folder Path ~==> $PROJ_MAC_BUILD_DIR"

rm -rf $OPENCV_MAC_BUILD_DIR
rm -rf $OPENCV_LIBRARY_OUTPUT
mkdir -p $OPENCV_MAC_BUILD_DIR
cd $OPENCV_MAC_BUILD_DIR
export IPHONEOS_DEPLOYMENT_TARGET=12.0

#-DBUILD_SHARED_LIBS=OFF를 통해 동적이 아닌 정적으로 빌드
#ios에선 촬영 녹화기능을 메인 카메라앱의 api를 활용해야 해서 빌드 제외
#openexr은 hdr이미지 처리 이나 hdr이 학습 또는 분석 시 필요하지 않음.
cmake ../ \
 -DCMAKE_INSTALL_PREFIX="../../../$OPENCV_LIBRARY_OUTPUT" \
 -DCMAKE_TOOLCHAIN_FILE="../../../Libraries/opencv/platforms/ios/cmake/Toolchains/Toolchain-iPhoneOS_Xcode.cmake" \
 -DIPHONEOS_DEPLOYMENT_TARGET=12.0 \
 -DCMAKE_SYSTEM_NAME=iOS \
 -DIOS_ARCH="arm64" \
 -DCMAKE_BUILD_TYPE=Release \
 -DBUILD_EXAMPLES=OFF \
 -DBUILD_SHARED_LIBS=OFF \
 -DBUILD_opencv_apps=OFF \
 -DBUILD_opencv_python=OFF \
 -DBUILD_opencv_java=OFF \
 -DBUILD_opencv_videoio=OFF \
 -DWITH_NEON=ON \
 -DENABLE_NEON=ON 

echo "\n\nFinished to made OpenCV cmake definition."
echo "Starting to OpenCV cmake build section.\n\n"

make -j8
make install

echo "\n\nFinished export OpenCV_library to [[ $OPENCV_LIBRARY_OUTPUT ]]\n\n"

echo "Current"
pwd
echo "\n\n"

cd ../../../

#MARK Project Build
rm -rf $PROJ_MAC_BUILD_DIR
rm -rf $PROJ_LIBRARY_OUTPUT
mkdir -p $PROJ_MAC_BUILD_DIR
cd $PROJ_MAC_BUILD_DIR

echo "\n\nFinished Make android build folder."

echo "Current"
pwd
echo "\n\n"

cmake ../ \
 -DCMAKE_INSTALL_PREFIX="../$PROJ_LIBRARY_OUTPUT" \
 -DCMAKE_SYSTEM_NAME=iOS \
 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
 -DCMAKE_OSX_ARCHITECTURES=arm64 \
 -DCMAKE_BUILD_TYPE=Release \
 -DPLATFORM_MAC=ON

 echo "\n\nFinished to made cmake definition."
 echo "Starting to cmake build section.\n\n"

 make -j8
 make install

 echo "Finished export bayabas_mac_library to [[ $PROJ_LIBRARY_OUTPUT ]]"