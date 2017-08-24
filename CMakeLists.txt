
cmake_minimum_required(VERSION 2.6)
if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose build type: None Debug Release RelWithDebInfo MinSizeRel")
project(eventstream)
enable_language(CXX)
enable_language(C)
enable_testing()
set(VERSION 0.1)
set(LIBVER 0.0.0)

SET(CMAKE_BUILD_TYPE debug)
set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/share/gnuradio/cmake/Modules)

OPTION(ENABLE_STATIC_LIBS "Enable building of static libraries" OFF)
message(STATUS "Building Static Libraries: ${ENABLE_STATIC_LIBS}")

########################################################################
# Dependencies setup
########################################################################
find_package(PythonInterp)
set(GR_REQUIRED_COMPONENTS RUNTIME BLOCKS PMT)
find_package(Gnuradio)
if(NOT PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter required by the build system.")
endif(NOT PYTHONINTERP_FOUND)
if(NOT GNURADIO_RUNTIME_FOUND)
    message(FATAL_ERROR "GR core not found")
endif(NOT GNURADIO_RUNTIME_FOUND)

# locate gnu radio
include(FindPkgConfig)

find_package(CppUnit)
if(NOT CPPUNIT_FOUND)
    message(FATAL_ERROR "CPPUNIT NOT FOUND!")
endif(NOT CPPUNIT_FOUND)

find_package(Log4Cpp)
if(NOT LOG4CPP_FOUND)
    message(FATAL_ERROR "LOG4CPP NOT FOUND!")
endif(NOT LOG4CPP_FOUND)

########################################################################
# Deal with Controlport.
########################################################################
# Enable Control Port code by default.
option(ENABLE_GR_CTRLPORT "Enable the Control Port stats." ON)

# This doesn't mean the backend for Control Port (ICE or THRIFT) is
# installed or working. It just means that you want to compile in the
# Control Port related methods of this OOT module. If you are running
# your graph and wish to view Control Port data/stats from this module,
# then your GNU Radio build must deal with ensuring your Control Port
# backend is installed/working.
if (ENABLE_GR_CTRLPORT)
    add_definitions(-DGR_CTRLPORT)
endif()
message(STATUS "GR_CTRLPORT ${ENABLE_GR_CTRLPORT}.")

# locate python
include(FindPythonLibs)

########################################################################
# Find boost
########################################################################
IF(UNIX AND EXISTS "/usr/lib64")
    LIST(APPEND BOOST_LIBRARYDIR "/usr/lib64") #fedora 64-bit fix
ENDIF(UNIX AND EXISTS "/usr/lib64")
SET(Boost_ADDITIONAL_VERSIONS
    "1.35.0" "1.35" "1.36.0" "1.36" "1.37.0" "1.37" "1.38.0" "1.38" "1.39.0" "1.39"
    "1.40.0" "1.40" "1.41.0" "1.41" "1.42.0" "1.42" "1.43.0" "1.43" "1.44.0" "1.44"
    "1.45.0" "1.45" "1.46.0" "1.46" "1.47.0" "1.47" "1.48.0" "1.48" "1.49.0" "1.49"
    "1.50.0" "1.50" "1.51.0" "1.51" "1.52.0" "1.52" "1.53.0" "1.53" "1.54.0" "1.54"
    "1.55.0" "1.55" "1.56.0" "1.56" "1.57.0" "1.57" "1.58.0" "1.58" "1.59.0" "1.59"
    "1.60.0" "1.60" "1.61.0" "1.61" "1.62.0" "1.62" "1.63.0" "1.63" "1.64.0" "1.64"
    "1.65.0" "1.65" "1.66.0" "1.66" "1.67.0" "1.67" "1.68.0" "1.68" "1.69.0" "1.69"
)

if (NOT DEFINED BOOST_ROOT)
	set(BOOST_ROOT ${CMAKE_INSTALL_PREFIX})
endif (NOT DEFINED BOOST_ROOT)

if(MSVC)
    if (NOT DEFINED BOOST_ALL_DYN_LINK)
        set(BOOST_ALL_DYN_LINK TRUE)
    endif()
    set(BOOST_ALL_DYN_LINK "${BOOST_ALL_DYN_LINK}" CACHE BOOL "boost enable dynamic linking")
    if(BOOST_ALL_DYN_LINK)
        add_definitions(-DBOOST_ALL_DYN_LINK) #setup boost auto-linking in msvc
    else(BOOST_ALL_DYN_LINK)
        unset(BOOST_REQUIRED_COMPONENTS) #empty components list for static link
    endif(BOOST_ALL_DYN_LINK)
endif(MSVC)

set(BOOST_REQUIRED_COMPONENTS
    system
    date_time
    program_options
    thread
    regex
	filesystem
	atomic
	chrono
)
MESSAGE("")
set(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost "1.53" COMPONENTS ${BOOST_REQUIRED_COMPONENTS})

IF(NOT Boost_FOUND)
    MESSAGE(FATAL_ERROR "Boost required")
ENDIF()

########################################################################
# Setup the package config file
########################################################################
#set variables found in the pc.in file
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix "\${prefix}")
set(libdir "\${exec_prefix}/lib${LIB_SUFFIX}")
set(includedir "\${prefix}/include")
set(grcdir "\${prefix}/share/gnuradio/grc/blocks/")
set(GR_PKG_DATA_DIR "\${prefix}/share/gnuradio/")
set(GRC_BLOCKS_DIR      ${GR_PKG_DATA_DIR}/grc/blocks)
set(GR_LIBRARY_DIR lib${LIB_SUFFIX})

########################################################################
# On Apple only, set install name and use rpath correctly, if not already set
########################################################################
if(APPLE)
    if(NOT CMAKE_INSTALL_NAME_DIR)
        set(CMAKE_INSTALL_NAME_DIR
            ${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR} CACHE
            PATH "Library Install Name Destination Directory" FORCE)
    endif(NOT CMAKE_INSTALL_NAME_DIR)
    if(NOT CMAKE_INSTALL_RPATH)
        set(CMAKE_INSTALL_RPATH
            ${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR} CACHE
            PATH "Library Install RPath" FORCE)
    endif(NOT CMAKE_INSTALL_RPATH)
    if(NOT CMAKE_BUILD_WITH_INSTALL_RPATH)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH ON CACHE
            BOOL "Do Build Using Library Install RPath" FORCE)
    endif(NOT CMAKE_BUILD_WITH_INSTALL_RPATH)
endif(APPLE)


configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/eventstream.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/eventstream.pc
@ONLY)


install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/eventstream.pc
    DESTINATION lib${LIB_SUFFIX}/pkgconfig
    COMPONENT "eventstream_devel"
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/GnuradioEventstreamConfig.cmake
    DESTINATION lib${LIB_SUFFIX}/cmake/gnuradioeventstream/
    COMPONENT "eventstream_devel"
)

########################################################################
# Install all headers in the include directories
########################################################################
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/es
    DESTINATION include COMPONENT "eventstream_devel"
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hh"
)

SET(EVENTSTREAM_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake)

########################################################################
# Setup the library
########################################################################
add_subdirectory(lib)
add_subdirectory(swig)
add_subdirectory(grc)
add_subdirectory(python)

set(GR_PKG_LIBEXEC_DIR ${GR_PYTHON_DIR}/es/)
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_INSTALL_PREFIX}/bin/)
set(GR_RUNTIME_DIR ${CMAKE_INSTALL_PREFIX}/bin/)
set(GR_PKG_CONF_DIR lib${LIB_SUFFIX}/pkgconfig)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
