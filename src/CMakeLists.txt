file(GLOB lycam-c-SRCS
  *.c)

if (NOT WIN32)
  list(REMOVE_ITEM lycam-c-SRCS ${CMAKE_CURRENT_LIST_DIR}/arvgetifaddrs_win32.c)
endif()

option(BUILD_DLL ON)
if(BUILD_DLL)
  add_library(lycam-c SHARED ${lycam-c-SRCS})
else()
  add_executable(lycam-c ${lycam-c-SRCS})
endif()

target_include_directories(lycam-c PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_compile_definitions(lycam-c
  PRIVATE "-DRAVIS_COMPILATION"
  PRIVATE "-DARV_H_INSIDE"
  PRIVATE -DARAVIS_DATA_DIR="/usr/local/arvais_data"
  PRIVATE -DARAVIS_API_VERSION="0.6")

if(WIN32)
  set(GLIB_INCLUDE_DIRS "d:/linux-root/lib/glib/include/glib-2.0" CACHE PATH "glib headers")
  set(GIO_INCLUDE_WIN32_DIRS "d:/linux-root/lib/glib/include/gio-win32-2.0"
    CACHE PATH "gio windows inclue directory")
  set(GLIB_CONFIG_INCLUDE_DIR "D:/linux-root/lib/glib/lib/glib-2.0/include"
    CACHE PATH "gio config include directory")

  message("GLIB_INCLUDE_DIRS: ${GLIB_INCLUDE_DIRS}")
  message("GIO_INCLUDE_WIN32_DIRS: ${GIO_INCLUDE_WIN32_DIRS}")
  message("GLIB_CONFIG_INCLUDE_DIR: ${GLIB_CONFIG_INCLUDE_DIR}")

  target_include_directories(lycam-c
    PRIVATE ${GLIB_INCLUDE_DIRS}
    PRIVATE ${GIO_INCLUDE_WIN32_DIRS}
    PRIVATE ${GLIB_CONFIG_INCLUDE_DIR})

  set(GLIB_LIB_DIRS "D:/linux-root/lib/glib/lib" CACHE PATH "glib lib directory")
  message("GLIB_LIB_DIRS: ${GLIB_LIB_DIRS}")
  foreach(a gmodule-2.0 gthread-2.0 gio-2.0 gobject-2.0 glib-2.0)
    message("finding: ${a}")
    find_library(${a}-lib
      NAMES ${a}
      HINTS ${GLIB_LIB_DIRS})
    message("found: ${${a}-lib}")
    set(GLIB_LIBS ${GLIB_LIBS} ${${a}-lib})
  endforeach()
  message("GLIB_LIBS: ${GLIB_LIBS}")
  target_link_libraries(lycam-c PUBLIC ${GLIB_LIBS})
else()
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLIB REQUIRED gmodule-2.0 gthread-2.0 gio-2.0 gobject-2.0 glib-2.0)
  target_include_directories(lycam-c PRIVATE ${GLIB_INCLUDE_DIRS})
  message("${GLIB_LDFLAGS}")
  target_link_libraries(lycam-c PUBLIC ${GLIB_LDFLAGS}
    -lgobject-2.0 -lgio-2.0 -lgthread-2.0 -lgmodule-2.0)
endif()

if(WIN32)
  set(ZLIB_INCLUDES "d:/linux-root/lib/zlib/include"
    CACHE PATH "path to zlib include directory")
  set(ZLIB_LIBRARIES "d:/linux-root/lib/zlib/lib"
    CACHE PATH "path to zlib library directory")
  target_include_directories(lycam-c PRIVATE ${ZLIB_INCLUDES})
  find_library(zlib zlib ${ZLIB_LIBRARIES})
  target_link_libraries(lycam-c PUBLIC ${zlib})
endif()

if(WIN32)
  set(LIBXML2_INCLUDE_DIRS "d:/linux-root/lib/libxml2/include/libxml2"
    CACHE PATH "path to libxml2 include directory")
  set(LIBXML2_LIBRARIES "d:/linux-root/lib/libxml2/lib"
    CACHE PATH "path to libxml2 library directory")
  target_include_directories(lycam-c PRIVATE ${LIBXML2_INCLUDE_DIRS})
  find_library(libxml2 libxml2 ${LIBXML2_LIBRARIES})
  target_link_libraries(lycam-c PUBLIC ${libxml2})
else()
  find_package(libxml2 REQUIRED)
  target_include_directories(lycam-c PRIVATE ${LIBXML2_INCLUDE_DIRS})
  message("xml2 include dir: ${LIBXML2_INCLUDE_DIRS}")
  target_link_libraries(lycam-c PUBLIC ${LIBXML2_LIBRARIES})
endif()

if(WIN32)
  set(LIBUSB_INCLUDE_DIRS "d:/linux-root/lib/libusb/include"
    CACHE PATH "path to libusb include directory")
  set(LIBUSB_LIB_DIRS "d:/linux-root/lib/libusb/lib/"
    CACHE PATH "path to libusb lib path")
  target_include_directories(lycam-c PRIVATE ${LIBUSB_INCLUDE_DIRS})
  find_library(libusb-1.0 libusb-1.0 ${LIBUSB_LIB_DIRS})
  target_link_libraries(lycam-c PUBLIC ${libusb-1.0})
else()
  pkg_search_module(LIBUSB libusb-1.0)
  target_include_directories(lycam-c PRIVATE ${LIBUSB_INCLUDE_DIRS})
  target_link_libraries(lycam-c PRIVATE ${LIBUSB_LDFLAGS})
endif()

install(TARGETS lycam-c
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
install(FILES
  "lyucamera.h"
  DESTINATION include)

message("install prefix: ${CMAKE_INSTALL_PREFIX}")
