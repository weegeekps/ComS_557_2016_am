# Main cmake file 
# gl-breakout

cmake_minimum_required(VERSION 2.6)

# Main project name is GL_Breakout_Program
project(GL_Breakout_Program)


set(CFILE_PATH  "C:/Users/amorris/Documents/Github/HCI-557-CG/SDK/cmake")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/" ${CMAKE_MODULE_PATH} "C:/Users/amorris/Documents/Github/HCI-557-CG/SDK/cmake")

message(STATUS "Source directory ${CMAKE_SOURCE_DIR}")
message(STATUS "SDK directory C:/Users/amorris/Documents/Github/HCI-557-CG/SDK/cmake")


# Find packages
FIND_PACKAGE(glew REQUIRED)
FIND_PACKAGE(glm REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)


# Include dirs
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLFW3_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})

# Add all files to the configuration
file(GLOB GL_Breakout_Program_SRC
	./*.cpp
	./*.h
	./gl_common/*.cpp
	./gl_common/*.h
)

# Create an executable
add_executable(GL_Breakout_Program ${GL_Breakout_Program_SRC})


# Add link directories
# Note required for this project
IF(WIN32)
	link_directories(C:/Users/amorris/Documents/Github/HCI-557-CG/SDK/lib)
ELSE(WIN32)
	link_directories(C:/Users/amorris/Documents/Github/HCI-557-CG/SDK/lib)
	link_directories(/usr/local/lib)
	link_directories(/usr/local/Cellar/glfw3/3.0.4/lib)
	link_directories(/usr/local/Cellar/glew/1.11.0/lib)
ENDIF(WIN32)


# Add libraries
target_link_libraries(GL_Breakout_Program ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${OPENGL_LIBRARIES} )
