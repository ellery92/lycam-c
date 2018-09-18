file(GLOB lycam-c-SRCS
  *.c)

add_library(lycam-c SHARED ${lycam-c-SRCS})

target_include_directories(lycam-c PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_compile_definitions(lycam-c
  PRIVATE "-DRAVIS_COMPILATION"
  PRIVATE "-DARV_H_INSIDE"
  PRIVATE -DARAVIS_DATA_DIR="/usr/local/arvais_data"
  PRIVATE -DARAVIS_API_VERSION="0.6")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
target_include_directories(lycam-c PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(lycam-c INTERFACE ${GLIB_LDFLAGS})

find_package(libxml2 REQUIRED)
target_include_directories(lycam-c PRIVATE ${LIBXML2_INCLUDE_DIRS})
message("xml2 include dir: ${LIBXML2_INCLUDE_DIRS}")
target_link_libraries(lycam-c INTERFACE ${LIBXML2_LIBRARIES})

pkg_search_module(LIBUSB libusb-1.0)
target_include_directories(lycam-c PRIVATE ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(lycam-c INTERFACE ${LIBUSB_LDFLAGS})
