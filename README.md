# 머리카락 감지 프로젝트

이마 색상을 기준으로 머리카락의 보존 비율을 체크하는 프로젝트입니다. 

## 1. 🗂️ 프로젝트 Git

Push 또는 Commit 하실 때 아래 구조와 사용설명에 따라주시면 감사드리겠습니다.🙇‍♀️

### 1.1 브랜치를 추가합니다.

*기능 추가* 시 아래와 같이 입력합니다.
```
features/원하는 이름
```
*버그로 인한* 브랜치 추가 시 아래와 같이 입력합니다.
```
bug/버그가 발생한 기능브랜치 이름 사용
```
**기타 사항**
dev: 개발 후 머징될 브랜치입니다.
release: 릴리즈 버전 배포 시 사용될 브랜치입니다.
beta: 베타 테스트 시 사용될 브랜치입니다.

**dev**는 머징만을 위해 사용합니다.
**beta**는 머징 후 테스트 할 때 태그로 버전을 붙여 빌드합니다.
**release**는 실제 라이브러리 배포시 태그로 버전을 붙여 빌드 후 각 플랫폼에 배포합니다.

### 1.2 Commit를 합니다.
*기능추가*
```
[features_add] 원하는 문구 입력
```
*기능수정*
```
[features_modi] 원하는 문구 입력
```
*버그수정*
```
[bug_modi] 원하는 문구입력
```

### 1.3 더욱 최적화된 사용법을 추가 시 연락주십시오. 언제든지 오픈되어 있습니다.
현재 사용방법보다 더욱 최적화된 사용방법이 있다면 연락주십시오.🙇‍♀️  우리는 언제든지 오픈마인드 입니다.
우리의 발전에 기여해주십시오. 그럼 더욱 많은 기능이 추가될 것입니다.

## 2. 💻 프로젝트 언어

- 언어는 C++

- 빌드는 Cmake로 진행합니다.

### 2.1 사용 IDE

VSCode에 C/C++ 확장팩을 설치하여 사용합니다.

### 2.2 라이브러리

1. OpenCV C++ 4.x

2. Cmake

  

## 3. 💾 디버깅 방법 (VSC, Mac 기준)

현재 OpenCV는 이 프로젝트에 포함되어 있습니다.

따라서 OpenCV를 Brew로 설치하지 않아도 되며 설치했더라도 이미 프로젝트 자체에서만 사용되도록 설정되어 있습니다.

  

- brew install cmake를 통해 mac에 cmake를 설치하여 주십시오.

- brew install openexr를 통해 openxr를 설치하여 주십시오.

- 프로젝트를 VSCode로 열고 왼쪽 탭에 빌드탭으로 이동합니다.

- Debug OpenCV App with CMake (macOS)를 선택 후 디버깅합니다.

  

## 4. 🌄 OpenCV를 나중에 내 프로젝트에 넣어보고 싶어!

OpenCV를 나중에 본인 프로젝트에 넣는 방법에 대해 설명합니다.

  

1. 프로젝트 폴더 안에 Libraries 라는 폴더를 생성합니다. 구조는 아래와 같습니다.

```

Proj Root

|

|___🆕Libraries(폴더 생성)

|

|___src

|___main.cpp

```

2. OpenCV C++ Git을 생성한 Libraries폴더에 Clone합니다.

```

git clone https://github.com/opencv/opencv.git

```

3. OpenCV 라이브러리 폴더 내부에 openCVBuild 폴더를 생성합니다.

```

mkdir openCVBuild && cd openCVBuild

```

  

4. OpenCV를 CMake로 Build 합니다.

```

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ..

```

  

5. Build가 되었으니 이제 실제로 dylib형태로 추출할 것입니다.

-j뒤에 본인의 컴퓨터 코어 수에 맞춰서 적어도 되지만 **<u>붙이지 않아도 자동으로 CMake가 컴퓨터의 코어 수를 계산 후</u>** 자동적으로 시작합니다.

  

☕️이 과정은 매우 오래 걸릴 수 있습니다.. 추출하는 동안 거의 컴퓨터를 사용할 수 없을 정도로 느려지니 잠시 쉬는 것을 추천드립니다.

```

make -j

```

  

6. 추출이 완료되면 OpenCV 라이브러리 폴더 내에 `install`이라는 폴더를 생성하기 위해 아래 코드를 입력합니다.

```
make install
```

**이제 50% 끝났습니다.**

  

7. VSCode C++ 확장 IDE 컴파일러가 우리의 `🌄OpenCV`를 인식할 수 있도록 설정을 변경할 것입니다. OpenCV에 생성되었던 `install`폴더를 아래 구조와 동일하게 설정하여 주십시오.

```

Proj Root

|

|___Libraries

| |

| |__🆕install (이렇게 옮겨주십시오.)

| |

| |__opencv

|

|___src

|___main.cpp

```

  

8. VSCode C++ 확장 IDE 컴파일러가 우리의 `🌄OpenCV`를 인식할 수 있도록 설정을 변경할 것입니다.

Project Root 경로 내에 `.vscode`를 생성하여 주십시오.

```

mkdir .vscode

```

  

9. 위 폴더 내에서 `c_cpp_properties.json` 파일을 생성하여 주십시오. 그리고 아래 코드를 해당 파일에 작성합니다. 그럼 C++ 확장팩 컴파일러가 이를 수용하여 자동으로 추출된 OpenCV를 인식할 수 있을 것입니다.

```

{

"configurations": [

{

"name": "Mac",

"includePath": [

"${workspaceFolder}/Libraries/install/include/opencv4",

"${workspaceFolder}/src/include"

],

"defines": [],

"macFrameworkPath": [

"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks"

],

"cStandard": "c17",

"cppStandard": "c++17",

"intelliSenseMode": "macos-clang-arm64",

"compilerPath": "/usr/bin/clang"

}

],

"version": 4

}

```

  

10. 위 폴더 내에서 `tasks.json`파일을 생성하여 주십시오. 그리고 아래 코드를 해당 파일에 작성합니다.

```

{

"version": "2.0.0",

"runner": "terminal",

"type": "shell",

"echoCommand": true,

"presentation": {

"reveal": "always"

},

"tasks": [

{

"type": "shell",

"label": "C/C++: clang++ Build with OpenCV, Cmake (macOS)",

"command": "cmake",

"args": ["-S", ".", "-B", "build", "&&", "cmake", "--build", "build"],

"group": "build",

"problemMatcher": []

}

]

}

```

  

11. 위 폴더 내에서 `launch.json`파일을 생성하여 주십시오. 그리고 아래 코드를 해당 파일에 작성합니다. 그럼 이제 `👩‍💻VSCode`가 이를 수용하여 빌드 선택지에 `Debug OpenCV App with CMake (macOS)`를 추가할 것입니다.

```

{

"version": "0.2.0",

"configurations": [

{

"name": "Debug OpenCV App with CMake (macOS)",

"type": "cppdbg",

"request": "launch",

"program": "${workspaceFolder}/build/(원하는 프로젝트 이름)",

"stopAtEntry": false,

"cwd": "${workspaceFolder}",

"environment": [],

"MIMode": "lldb",

"preLaunchTask": "C/C++: clang++ Build with OpenCV, Cmake (macOS)"

}

]

}

```

  

12. 우리는 이제 CMake를 통해 코드를 디버깅 해야합니다. 그럼 CMake도 이를 수용하여 VSCode에서 디버깅을 할 수 있도록 도와줄 것입니다. Project Root 폴더 내에서 `CMakeLists.txt`를 생성하여 주십시오. 그리고 아래 코드를 작성합니다.

```

cmake_minimum_required(VERSION 2.8)

project( 원하는 프로젝트 이름 )

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/Libraries/install/lib/cmake/opencv4")

find_package(OpenCV REQUIRED)

include_directories(

${OpenCV_INCLUDE_DIRS}

${CMAKE_SOURCE_DIR}/src/include

)

file(GLOB SOURCES "src/*.cpp")

message(STATUS "SOURCES: ${SOURCES}")

add_executable( BayabasAi ${SOURCES} )

target_link_libraries( BayabasAi ${OpenCV_LIBS} )

```

  

13. 🎉🎉이제 우리 `VSCode`는 디버깅 할 준비가 완료되었습니다. 테이블을 정리하여 주시고 main.cpp에 opencv라이브러리를 추가할 준비를 하여 주십시오.

  

14.  `main.cpp`파일에 아래와 같이 입력합니다.

```

// Packages

#include <iostream>

#include <opencv2/opencv.hpp>

  

// Namespace

using namespace cv

  

int main()

{

string path = "이미지 파일 주소(jpg, bmp 등)";

Mat imgRead = imread(path);

imshow("Hello World", imgRead);

waitKey(0);

return 0;

}

```

15. VSCode의 빌드 탭에서 우리가 추가한 **Debug OpenCV App with CMake (macOS)**를 선택하여 디버깅을 시작합니다.

  

## 5. 👩‍💻개발 인원
- Daehyeon Hong