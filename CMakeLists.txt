cmake_minimum_required(VERSION 3.0)

project(diagnostic)

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  /usr/include/glib-2.0
  /usr/lib/x86_64-linux-gnu/glib-2.0/include
  /usr/include/dbus-1.0
  /usr/lib/x86_64-linux-gnu/dbus-1.0/include
#  /usr/include/gio-unix-2.0
  ${PROJECT_SOURCE_DIR}/bluez
)


add_executable(diagnostic
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/src/thread.cpp
  ${PROJECT_SOURCE_DIR}/src/crank-canvas.cpp
)

target_link_directories(diagnostic PUBLIC
  ${PROJECT_SOURCE_DIR}/bluez/gdbus/.libs
  ${PROJECT_SOURCE_DIR}/bluez/src/.libs
)

target_link_libraries(diagnostic PUBLIC
  ${wxWidgets_LIBRARIES}
  glib-2.0
  dbus-1
  gdbus-internal
  shared-mainloop
  explain
  gsl
  GL
#  glfw
  GLEW
)

install(TARGETS diagnostic RUNTIME DESTINATION bin)
