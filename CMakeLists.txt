cmake_minimum_required(VERSION 3.16)
project(vspg)

set(CMAKE_CXX_STANDARD 17)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -DNDEBUG")

include_directories(resource)
include_directories(pugixml/src)
include_directories(src)
include_directories(cmdline)
include_directories(NanoLog)
include_directories(tinyxml2)
include_directories(CJsonObject)

add_library(libpugixml pugixml/src/pugixml.cpp)
link_libraries(libpugixml)

add_library(libNanoLog NanoLog/NanoLog.cpp)
link_libraries(libNanoLog)

add_library(libtinyxml2 tinyxml2/tinyxml2.cpp)
link_libraries(libtinyxml2)

add_library(libCJsonObject CJsonObject/cJSON.c CJsonObject/CJsonObject.cpp)
link_libraries(libCJsonObject)

set(src_vspg src/vspg.cpp src/error.cpp
        src/prj.cpp src/prjCube.cpp src/prjMCUX.cpp src/prjIAR.cpp src/prjMDK.cpp src/prjCCS.cpp src/prjAurix.cpp
        src/pg.cpp src/pgVS.cpp src/pgCmake.cpp src/pgVSCode.cpp)

add_executable(vspg src/main.cpp ${src_vspg})
add_executable(test test/test.cpp ${src_vspg})

if (WIN32)
    set(EWS_APP_NAME vspg)
    set(EXE_TARGET_NAME vspg)
    install(TARGETS vspg DESTINATION bin)
    install(FILES resource/c_cpp_properties_win32.json DESTINATION resource)
    install(FILES resource/EmbProjectTemplate.sln DESTINATION resource)
    install(FILES resource/EmbProjectTemplate.vcxproj DESTINATION resource)
    install(FILES resource/EmbProjectTemplate.vcxproj.filters DESTINATION resource)
    install(FILES resource/EmbProjectTemplate.vcxproj.user DESTINATION resource)

    include(InstallRequiredSystemLibraries)
    #https://cmake.org/cmake/help/v3.6/module/CPackNSIS.html
    #https://cmake.org/cmake/help/v3.17/cpack_gen/nsis.html#cpack_gen:CPack%20NSIS%20Generator
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
    set(CPACK_GENERATOR NSIS)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    CreateShortCut \\\"$DESKTOP\\\\${EWS_APP_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${EXE_TARGET_NAME}.exe\\\"
    ")
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    Delete \\\"$DESKTOP\\\\${EWS_APP_NAME}.lnk\\\"
        ")
    set(CPACK_PACKAGE_NAME "vspg")
    set(CPACK_PACKAGE_VENDOR "beforelight")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "vspg - EmbVSProjectGenerator")
    set(CPACK_PACKAGE_VERSION "1.2.0")
    set(CPACK_PACKAGE_VERSION_MAJOR "1")
    set(CPACK_PACKAGE_VERSION_MINOR "2")
    set(CPACK_PACKAGE_VERSION_PATCH "0")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "EmbVSProjectGenerator")
    SET(CPACK_NSIS_MODIFY_PATH ON)
    INCLUDE(CPack)
endif ()

