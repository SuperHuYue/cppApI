##############################################################################################################
#此文件保存了一些cmake的高阶使用方式，以及在网络上查找到的好用的方法
#
#
#
#
#
#
##############################################################################################################







#加载外部dll
add_library(
  TDPlatformC SHARED IMPORTED)
set_target_properties(
  TDPlatformC
  PROPERTIES IMPORTED_IMPLIB               "${sTdpcDir}/lib/TDPlatformC.lib"
             IMPORTED_LOCATION             "${sTdpcDir}/bin/TDPlatformC.dll"
             INTERFACE_INCLUDE_DIRECTORIES "${sTdpcDir}/include")

#使用conan下载配置文件

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Download conan.cmake script")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.14/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
       SHOW_PROGRESS STATUS zStatus)
  list(GET zStatus 0 sStatusCode)
  if(NOT sStatusCode EQUAL 0)
    file(REMOVE "${CMAKE_BINARY_DIR}/conan.cmake")
    list(GET zStatus 1 sStatusText)
    message(FATAL_ERROR "Download conan.cmake script failed: ${sStatusText}")
  endif()
  message(STATUS "Download conan.cmake script - done")
endif()
include("${CMAKE_BINARY_DIR}/conan.cmake")
#下述在consloe使用conan search libcurl -r conan-center进行查询
conan_cmake_run(
  #    REQUIRES  libcurl/7.61.1@bincrafters/stable
    REQUIRES  asio/1.12.0@bincrafters/stable
    REQUIRES  jsoncpp/1.8.4@theirix/stable
    REQUIRES  glog/0.4.0@bincrafters/stable
    REQUIRES  opencv/3.4.5@conan/stable
    BASIC_SETUP CMAKE_TARGETS)
#下载完毕后的使用
target_link_libraries(
  websocket
  CONAN_PKG::asio
  CONAN_PKG::glog
  CONAN_PKG::jsoncpp
 ) 


#设置编译器宏选项
target_compile_definitions(
  YLGaitLib
  PRIVATE "YLGAITLIB_EXPORTS")


#设置标准编译器命令
set_target_properties(
  YLGaitTest
  PROPERTIES CXX_STANDARD          17
             CXX_STANDARD_REQUIRED ON)

            
#######################################################################
#具体项目的示例
cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)

project(YLGaitSearchSer VERSION 0.1.0 LANGUAGES C CXX)

if(MSVC)
    add_compile_options("/utf-8")
endif()

#增加conan脚本
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Download conan.cmake script")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.14/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
       SHOW_PROGRESS STATUS zStatus)
  list(GET zStatus 0 sStatusCode)
  if(NOT sStatusCode EQUAL 0)
    file(REMOVE "${CMAKE_BINARY_DIR}/conan.cmake")
    list(GET zStatus 1 sStatusText)
    message(FATAL_ERROR "Download conan.cmake script failed: ${sStatusText}")
  endif()
  message(STATUS "Download conan.cmake script - done")
endif()
include("${CMAKE_BINARY_DIR}/conan.cmake")

conan_cmake_run(
    REQUIRES  asio/1.12.0@bincrafters/stable
    REQUIRES  jsoncpp/1.8.4@theirix/stable
    REQUIRES  glog/0.4.0@bincrafters/stable
    OPTIONS   jsoncpp:shared=True
    BASIC_SETUP CMAKE_TARGETS)

# 配置头文件，把CMake设置传递给源文件
configure_file(
	"${PROJECT_SOURCE_DIR}/Config.h.in"
	"${PROJECT_BINARY_DIR}/Config.h")

#为了让编译程序找到Config.h,增加二进制目录到搜索包含文件中
#include_directories 用于增加 include目录
include_directories("${PROJECT_BINARY_DIR}")

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:.:./lib")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

#-------------------------------------commonlib---------------------------------
if(WIN32)
  set(ZooKeeperC_ROOT "${CMAKE_CURRENT_LIST_DIR}/deps/zookeeper/win")
endif()
find_package(ZooKeeperC)

add_library(ylcommonlib
    STATIC
    ${PROJECT_BINARY_DIR}/Config.h
    src/common.h
    src/common/ylcommonlib.h
    src/common/ylcommonlib.cpp
    src/common/configfile.h
    src/common/configfile.cpp
    src/zk/ylzkc.hxx
    src/zk/ylzkc.cxx
)

set_target_properties(ylcommonlib
  PROPERTIES CXX_STANDARD          17
             CXX_STANDARD_REQUIRED ON
)

if(UNIX)
target_compile_options(ylcommonlib
  PRIVATE -fPIC
)
endif()

target_compile_definitions(ylcommonlib
  PUBLIC  "YLCOMMON_SHARED"
  PRIVATE "YLCOMMON_EXPORTS"
)

target_include_directories(ylcommonlib
  PRIVATE src
  PRIVATE src/zk
  PRIVATE src/common
)

target_link_libraries(ylcommonlib
  ZooKeeperC::zookeeper
)
#-------------------------------------commonlib---------------------------------

#-------------------------------------algorserlib---------------------------------
add_library(ylalgorserlib
    ${PROJECT_BINARY_DIR}/Config.h
    src/algor/ylalgorserlib.h
    src/algor/ylalgorserlib.cpp
)

target_compile_definitions(ylalgorserlib
  PUBLIC  "YLALGOR_SHARED"
  PRIVATE "YLALGOR_EXPORTS"
)

#gaitsearch
if(WIN32)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(gaitsearch)

add_library(ylgaitsearchlib
    SHARED
    ${PROJECT_BINARY_DIR}/Config.h
    src/algor/ylgaitsearchlib.h
    src/algor/ylgaitsearchlib.cpp
    conf/gaitsearch.conf
)

target_link_libraries(ylgaitsearchlib
    ylcommonlib
    ylalgorserlib
    CONAN_PKG::glog
    CONAN_PKG::jsoncpp
    ${GAITLIB_LIBRARY}
    )

target_compile_definitions(ylgaitsearchlib
    PRIVATE "YLGAIT_EXPORTS"
)

target_include_directories(ylgaitsearchlib
    PRIVATE src
    PRIVATE src/zk
    PRIVATE src/common
    PRIVATE ${GAITLIB_INCLUDE_DIR}
)

set_target_properties(ylgaitsearchlib
  PROPERTIES CXX_STANDARD          17
             CXX_STANDARD_REQUIRED ON
)
endif()
#gaitsearch

#-------------------------------------algorserlib---------------------------------


#-------------------------------------websocketser---------------------------------
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(websocketpp)

add_executable(ylbasicwebser
    main.cpp 
    ${PROJECT_BINARY_DIR}/Config.h
    src/websocket/websocket_server.h 
    src/websocket/websocket_server.cpp
    conf/websocket.conf
 )

target_link_libraries(ylbasicwebser
    ylcommonlib
    ylalgorserlib
    websocketpp::websocketpp
    CONAN_PKG::asio
    CONAN_PKG::glog
    CONAN_PKG::jsoncpp
)

if(UNIX)
target_link_libraries(ylbasicwebser
    dl
)
endif()

target_include_directories(ylbasicwebser
    PRIVATE src
    PRIVATE src/websocket
)

set_target_properties(ylbasicwebser
	PROPERTIES
	CXX_STANDARD 17)
#-------------------------------------websocketser---------------------------------
#################################################################################################################################################
