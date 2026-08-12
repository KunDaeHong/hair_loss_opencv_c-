cmake_minimum_required(VERSION 3.5)
project( BayabasAi )
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME Bayabas_bald_detector_module)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE Debug) #debug 때만 사용
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") #debug 때만 사용
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(PLATFORM_ANDROID)
    #TODO: LLVM NDK로 다시 빌드 필요
    set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/Libraries/opencv_static_android/sdk/native/jni") #Arm x64
    #set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/Libraries/opencv_static_android/sdk/native/jni/abi-armeabi-v7a") #Arm x86
    message(STATUS "Platform... Android")
elseif(PLATFORM_MAC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/Libraries/install/lib/cmake/opencv4") #Debug
    else()
        set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/Libraries/opencv_static/lib/cmake/opencv4") #Release
    endif()
    #set(OpenEXR_DIR "${CMAKE_SOURCE_DIR}/Libraries/openexr_static/lib/cmake/OpenEXR") #Mac, Windows 전용

    message(STATUS "Platform... Darwin")
else()
    set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/Libraries/opencv_static/lib/cmake/opencv4")
    #set(OpenEXR_DIR "${CMAKE_SOURCE_DIR}/Libraries/openexr_static/lib/cmake/OpenEXR") #Mac, Windows 전용
    
    message(STATUS "Platform... Windows")
endif()

if(PLATFORM_ANDROID)
    find_package(OpenCV REQUIRED)
    find_library(log-lib log)
    find_library(android-lib android)
    find_library(z-lib z)
    find_library(m-lib m)
else()
    find_package(OpenCV REQUIRED)
    #find_package(OpenEXR REQUIRED) #Mac, Windows 전용
endif()

include_directories( 
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/include
)

#Project Headers
file(GLOB SOURCES "src/*.cpp" "src/Utils/*.cpp")
file(GLOB MAIN_HEADERS "${CMAKE_SOURCE_DIR}/src/include/hair_loss_cnn.hpp")
file(GLOB FILTER_HEADERS "${CMAKE_SOURCE_DIR}/src/include/bayabas_hair_filter.hpp")
file(GLOB MLP_HEADERS "${CMAKE_SOURCE_DIR}/src/Utils/train/include/bayabas_mlp.hpp")
file(GLOB PERCEPTRON_HEADERS "${CMAKE_SOURCE_DIR}/src/Utils/train/core/include/Perceptron.hpp")

set(PUBLIC_HEADERS
    ${MLP_HEADERS}
    ${MAIN_HEADERS}
    ${FILTER_HEADERS}
    ${PERCEPTRON_HEADERS}
)

if(PLATFORM_MAC)
    set(FRAMEWORK_NAME "${PROJECT_NAME}")
    set(APPLE_BUNDLE_NAME hairlossCnnFramework)
    set(APPLE_BUNDLE_ID com.neurocircuit.hairLossCnn)
    set(LIBVERSION 0.0.1)
    set(IPHONEOS_DEPLOYMENT_TARGET 14)
    configure_file(
        ${CMAKE_SOURCE_DIR}/platforms/ios/Info.Dynamic.plist.in
        ${CMAKE_SOURCE_DIR}/platforms/ios/build/Info.plist
    )
endif()
##END

message(STATUS "\n\nSOURCES: ${SOURCES}")
message(STATUS "\nCV LIB: ${OpenCV_LIBS}")

message(STATUS "\nGLOB HEADERS: ${PUBLIC_HEADERS}\n")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #Binary
    add_executable( BayabasAi ${SOURCES} )
else()

    #MAC 또는 iOS는 xcode 자체 framework로 포함
    if(PLATFORM_MAC)
        add_library(BayabasAi SHARED 
            ${SOURCES}
            ${PUBLIC_HEADERS}
        )
        target_link_options(BayabasAi PRIVATE "-Wl,-all_load" )
    else()
        #Dynamic Library(Release용)
        add_library(BayabasAi SHARED ${SOURCES})
    endif()
endif()

set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

if(PLATFORM_ANDROID)
    target_link_libraries( BayabasAi 
        ${OpenCV_LIBS}
        ${log-lib}
        ${android-lib}
        ${z-lib}
        ${m-lib}
    )
elseif(PLATFORM_MAC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries( BayabasAi 
            ${OpenCV_LIBS} 
            "-framework Foundation"
            "-framework CoreFoundation"
            "-framework CoreGraphics"
            "-framework ImageIO"
            "-framework QuartzCore"
            #"-framework UIKit"          # iOS만 해당될 수도 있음
            "-framework Cocoa"           # Mac에서 디버깅 시 필요 (라이브러리 개발 시에 사용)
            "-framework AVFoundation"    # 필요 시
            #OpenEXR::OpenEXR #Mac, Windows 전용
        )
        target_compile_definitions(BayabasAi PRIVATE $<$<CONFIG:Debug>:DEBUG>)
    else()
        target_link_libraries( BayabasAi 
            ${OpenCV_LIBS} 
            "-framework Foundation"
            "-framework CoreFoundation"
            "-framework CoreGraphics"
            "-framework ImageIO"
            "-framework QuartzCore"
            "-framework UIKit"          # iOS만 해당될 수도 있음
            #"-framework Cocoa"         # Mac에서 디버깅 시 필요 (라이브러리 개발 시에 사용)
            "-framework AVFoundation"   # 필요 시
            #OpenEXR::OpenEXR #Mac, Windows 전용
        )
    endif()
else()
    target_link_libraries( BayabasAi 
            ${OpenCV_LIBS}
    )
endif()

if(PLATFORM_ANDROID)
    set_target_properties(BayabasAi
        PROPERTIES
        CXX_STANDARD 17
        OUTPUT_NAME "${PROJECT_NAME}"
        PUBLIC_HEADER "${PUBLIC_HEADERS}"
        SUFFIX ".so"
    )
elseif(PLATFORM_MAC)
    set_target_properties(BayabasAi
        PROPERTIES
        CXX_STANDARD 17
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.neurocircuit.hairLossCnn
        MACOSX_FRAMEWORK_INFO_PLIST "${CMAKE_SOURCE_DIR}/platforms/ios/build/Info.plist"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        OUTPUT_NAME "${PROJECT_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output_mac"
        PUBLIC_HEADER "${PUBLIC_HEADERS}"
    )
elseif(PLATFORM_WINDOWS)
    set_target_properties(BayabasAi 
        PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/lib"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/lib"
        PUBLIC_HEADER "${PUBLIC_HEADERS}"
        SUFFIX ".dll"
    )
endif()

if(PLATFORM_MAC)
    install(TARGETS BayabasAi
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        BUNDLE DESTINATION bin
        FRAMEWORK DESTINATION Frameworks
        PUBLIC_HEADER DESTINATION include
    )

    install(
        DIRECTORY "${CMAKE_SOURCE_DIR}/Libraries/opencv_static/include/opencv4/opencv2"
        DESTINATION "Frameworks/${PROJECT_NAME}.framework/Headers"
    )
else()
    install(TARGETS BayabasAi
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
    )

    install(
        DIRECTORY "${CMAKE_SOURCE_DIR}/Libraries/opencv_static/include/opencv4/opencv2"
        DESTINATION "include"
    )
endif()







