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

            