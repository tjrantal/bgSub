# Properties->C/C++->General->Additional Include Directories
#include_directories ("${PROJECT_SOURCE_DIR}/lib")
#include_directories ("D:/timo/programming/cpp/ocvBuildVCC/install/include")
#LINK_DIRECTORIES("D:/timo/programming/cpp/ocvBuildVCC/lib/Release")

file (GLOB SOURCE_FILES "*.h" "*.cpp")

# Collect sources into the variable SIMPLE_FUNCTION_SOURCES  
file (GLOB lbp_FUNCTION_SOURCES
      "lbp/*.h"
      "lbp/*.cpp")
	  
source_group("" FILES ${SOURCE_FILES})
source_group("lbp" FILES ${lbp_FUNCTION_SOURCES})

#ADD dependent libraries include and link directories
INCLUDE_DIRECTORIES( ${OpenCV_DIR}/install/include )

if (WIN32)
	message("Executable WIN32")
	add_executable (bgSub ${SOURCE_FILES} ${lbp_FUNCTION_SOURCES})
endif(WIN32)
if(UNIX)
	add_executable (bgSub ${SOURCE_FILES} ${lbp_FUNCTION_SOURCES})
endif(UNIX)

#message("LIBAVFORMAT " ${LIBAVFORMAT_LIBRARIES})

# Properties->Linker->Input->Additional Dependencies
if(UNIX)
	target_link_libraries (bgSub ${OpenCV_LIBS} -lX11)#${LIBAVFORMAT_LIBRARIES})# ${GSL_LIBRARIES})
endif(UNIX)
if(WIN32)
	target_link_libraries (bgSub ${OpenCV_LIBS})
endif(WIN32)
#message("wxWidgets wxWidgets_LIBRARIES ${wxWidgets_LIBRARIES}")
 
# Creates a folder "executables" and adds target 
# project (bgSub.vcproj) under it
set_property(TARGET bgSub PROPERTY FOLDER "executables")
 
# Adds logic to INSTALL.vcproj to copy bgSub.exe to destination directory
install (TARGETS bgSub
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
