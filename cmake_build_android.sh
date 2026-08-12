#Android Cmake from Mac --> https://developer.android.com/studio/projects/configure-cmake?hl=ko

#MARK OpenCV Build (안드로이드는 OpenEXR를 지원하지 않음)
SDK_ROOT="$HOME/Library/Android/sdk"
NDK_ROOT="$HOME/Library/Android/sdk/ndk/27.0.12077973"
API_LEVEL=28
TOOLCHAIN="$HOME/Library/Android/sdk/ndk/27.0.12077973/build/cmake/android.toolchain.cmake"
ARCH="arm64-v8a"  #arm x64
#ARCH="armeabi-v7a" #arm x86 

#OPENCV Build
OPENCV_ANDROID_BUILD_DIR="$(dirname $0)/Libraries/opencv/build_opencv_android"
OPENCV_LIBRARY_OUTPUT="$(dirname $0)/Libraries/opencv_static_android"

#Project Build
PROJ_ANDROID_BUILD_DIR="$(dirname $0)/build_android"
PROJ_LIBRARY_OUTPUT="$(dirname $0)/output_android"

echo "Android SDK Path ~==> $SDK_ROOT"
echo "Android NDK Path ~==> $NDK_ROOT"
echo "Android Toolchain Path (Cmake) ~==> $TOOLCHAIN"
echo "Android Target API Level ~==> $API_LEVEL"
echo "Android Target Architecture ~==> $ARCH"
echo "OpenCV Build Folder Path ~==> $OPENCV_ANDROID_BUILD_DIR"
echo "Project Build Folder Path ~==> $PROJ_ANDROID_BUILD_DIR"

rm -rf $OPENCV_ANDROID_BUILD_DIR
rm -rf $OPENCV_LIBRARY_OUTPUT
mkdir -p $OPENCV_ANDROID_BUILD_DIR
cd $OPENCV_ANDROID_BUILD_DIR

#-DBUILD_SHARED_LIBS=OFF를 통해 동적이 아닌 정적으로 빌드
cmake ../ \
 -DCMAKE_INSTALL_PREFIX="../../../$OPENCV_LIBRARY_OUTPUT" \
 -DCMAKE_BUILD_TYPE=Release \
 -DANDROID_SDK="$SDK_ROOT" \
 -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
 -DANDROID_NDK="$NDK_ROOT" \
 -DANDROID_SDK_TARGET="$API_LEVEL" \
 -DANDROID_PLATFORM="android-${API_LEVEL}" \
 -DANDROID_ABI="$ARCH" \
 -DBUILD_EXAMPLES=OFF \
 -DWITH_OPENEXR=OFF \
 -DBUILD_SHARED_LIBS=OFF \
 -DBUILD_FAT_JAVA_LIB=OFF \
 -DBUILD_ANDROID_EXAMPLES=OFF \
 -DBUILD_JAVA=OFF \
 -DBUILD_ANDROID_PROJECTS=OFF

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
rm -rf $PROJ_ANDROID_BUILD_DIR
rm -rf $PROJ_LIBRARY_OUTPUT
mkdir -p $PROJ_ANDROID_BUILD_DIR
cd $PROJ_ANDROID_BUILD_DIR

echo "\n\nFinished Make android build folder."

echo "Current"
pwd
echo "\n\n"

cmake ../ \
 -DCMAKE_INSTALL_PREFIX="../$PROJ_LIBRARY_OUTPUT" \
 -DCMAKE_BUILD_TYPE=Release \
 -DANDROID_SDK="$SDK_ROOT" \
 -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
 -DANDROID_NDK="$NDK_ROOT" \
 -DANDROID_SDK_TARGET="$API_LEVEL" \
 -DANDROID_PLATFORM="android-${API_LEVEL}" \
 -DANDROID_ABI="$ARCH" \
 -DBUILD_EXAMPLES=OFF \
 -DPLATFORM_ANDROID=ON 

 echo "\n\nFinished to made cmake definition."
 echo "Starting to cmake build section.\n\n"

 make -j8
 make install

 echo "Finished export bayabas_android_library to [[ $PROJ_LIBRARY_OUTPUT ]]"