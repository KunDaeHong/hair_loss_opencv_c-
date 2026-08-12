#OPENCV Build
OPENCV_DLL_BUILD_DIR="$(dirname $0)/Libraries/opencv/build_opencv"
OPENCV_LIBRARY_OUTPUT="$(dirname $0)/Libraries/opencv_static"

#Project Build
PROJ_DLL_BUILD_DIR="$(dirname $0)/build_dll"
PROJ_LIBRARY_OUTPUT="$(dirname $0)/output_dll"

#TOOLCHAIN
TOOLCHAIN="$HOME/Desktop/project/cv/bayabasHairLossCV/platforms/windows/mingw64_86.cmake"

echo "OpenCV Build Folder Path ~==> $OPENCV_DLL_BUILD_DIR"
echo "Project Build Folder Path ~==> $PROJ_DLL_BUILD_DIR"

rm -rf $OPENCV_DLL_BUILD_DIR
rm -rf $OPENCV_LIBRARY_OUTPUT
mkdir -p $OPENCV_DLL_BUILD_DIR
cd $OPENCV_DLL_BUILD_DIR

#-DBUILD_SHARED_LIBS=OFF를 통해 동적이 아닌 정적으로 빌드
#openexr은 hdr이미지 처리 이나 hdr이 학습 또는 분석 시 필요하지 않음.
cmake ../ \
 -DCMAKE_INSTALL_PREFIX="../../../$OPENCV_LIBRARY_OUTPUT" \
 -DCMAKE_BUILD_TYPE=Release \
 -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
 -DBUILD_EXAMPLES=OFF \
 -DBUILD_SHARED_LIBS=OFF \
 -DWITH_OPENEXR=OFF \
 -DBUILD_opencv_apps=OFF \
 -DBUILD_opencv_python=OFF \
 -DBUILD_opencv_java=OFF \
 -DBUILD_opencv_videoio=OFF \
 -DWITH_NEON=OFF \
 -DENABLE_NEON=OFF 

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
rm -rf $PROJ_DLL_BUILD_DIR
rm -rf $PROJ_LIBRARY_OUTPUT
mkdir -p $PROJ_DLL_BUILD_DIR
cd $PROJ_DLL_BUILD_DIR

echo "\n\nFinished Make android build folder."

echo "Current"
pwd
echo "\n\n"

cmake ../ \
 -DCMAKE_INSTALL_PREFIX="../$PROJ_LIBRARY_OUTPUT" \
 -DCMAKE_BUILD_TYPE=Release \
 -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
 -DPLATFORM_WINDOWS=ON

 echo "\n\nFinished to made cmake definition."
 echo "Starting to cmake build section.\n\n"

 make -j8
 make install

 echo "Finished export bayabas_mac_library to [[ $PROJ_LIBRARY_OUTPUT ]]"