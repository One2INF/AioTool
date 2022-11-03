cmake_minimum_required(VERSION 3.5)

project(AioTool VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)


file(GLOB PROJECT_SOURCES "*.cpp" "*.h" "*.ui")
file(GLOB CUSTOM_WIDGET "CustomWidget/*")
file(GLOB SESSIONS "Sessions/*")
file(GLOB TOOLS "Tools/ymodem/*")
file(GLOB EMBEDDED_LIB "ThirdParty/embedded_lib/ymodem.*")

include_directories(
  .
  ThirdParty/embedded_lib
  CustomWidget
  Sessions
  Tools/ymodem
)

add_executable(AioTool
  ${PROJECT_SOURCES}
  ${CUSTOM_WIDGET}
  ${SESSIONS}
  ${TOOLS}
  ${EMBEDDED_LIB}
)

target_link_libraries(AioTool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort)

set_target_properties(AioTool PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS AioTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AioTool)
endif()
