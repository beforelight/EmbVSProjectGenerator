cmake_minimum_required(VERSION 3.16)
project(vspg)

set(CMAKE_CXX_STANDARD 17)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

link_directories(pugixml/src)
add_library(pugixml pugixml/src/pugixml.cpp)
link_libraries(pugixml)

link_directories(NanoLog)
add_library(NanoLog NanoLog/NanoLog.cpp)
link_libraries(NanoLog)

link_directories(tinyxml2)
add_library(tinyxml2 tinyxml2/tinyxml2.cpp)
link_libraries(tinyxml2)

link_directories(CJsonObject)
add_library(CJsonObject CJsonObject/cJSON.c CJsonObject/CJsonObject.cpp)
link_libraries(CJsonObject)

include_directories(pugixml/src)
include_directories(src)
include_directories(cmdline)
include_directories(NanoLog)
include_directories(tinyxml2)
include_directories(CJsonObject)

add_executable(vspg src/main.cpp
        src/prj.cpp src/prjCube.cpp src/prjMCUX.cpp
        src/pg.cpp src/pgVS.cpp src/pgCmake.cpp src/pgVSCode.cpp
        src/error.cpp
        )
add_executable(test src/test.cpp src/error.cpp)


if (WIN32)
    install(TARGETS vspg DESTINATION bin)
    install(FILES resource/c_cpp_properties_win32.json DESTINATION resource)
    install(FILES resource/EmbProjectTemplate.sln DESTINATION resource)
    install(FILES resource/EmbProjectTemplate.vcxproj DESTINATION resource)
    install(FILES resource/EmbProjectTemplate.vcxproj.filters DESTINATION resource)
    install(FILES resource/EmbProjectTemplate.vcxproj.user DESTINATION resource)

    set(CPACK_GENERATOR NSIS)
    set(CPACK_PACKAGE_NAME "vspg")
    set(CPACK_PACKAGE_VENDOR "beforelight")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "vspg - EmbVSProjectGenerator")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_VERSION_MAJOR "1")
    set(CPACK_PACKAGE_VERSION_MINOR "0")
    set(CPACK_PACKAGE_VERSION_PATCH "0")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "EmbVSProjectGenerator")
    SET(CPACK_NSIS_MODIFY_PATH ON)
    INCLUDE(CPack)
endif ()

