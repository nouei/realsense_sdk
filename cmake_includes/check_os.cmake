if(WIN32)
include(${ROOT_DIR}/cmake_includes/config_windows.cmake)
else()
include(${ROOT_DIR}/cmake_includes/config_linux.cmake)
endif()