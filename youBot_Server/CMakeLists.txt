PROJECT(youBot_Server)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


########### cmake configuration and compiler flags ###########
SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_C_FLAGS_DEBUG "-fPIC -Wall -g")
SET(CMAKE_CXX_FLAGS_DEBUG "-fPIC -Wall -g")
SET(CMAKE_C_FLAGS_RELEASE "-fPIC -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-fPIC -Wall")
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #static libraries
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #executables
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib) #dynamic libraries (plugins)


FIND_LIBRARY(OODL_YOUBOT_LIBRARIES NAMES "YouBotDriver" "soem"
  PATHS
  ${youbot_driver_PACKAGE_PATH}/lib
  $ENV{YOUBOTDIR}/lib
  $ENV{YOUBOT_LIB}
  $ENV{ROBOTPKG_BASE}/lib
  ENV LD_LIBRARY_PATH
  ENV LIBRARY_PATH
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  NO_DEFAULT_PATH
)

FIND_PATH(OODL_YOUBOT_INCLUDE_DIR NAMES youbot/YouBotBase.hpp
  PATHS
  ${youbot_driver_PACKAGE_PATH}/
  $ENV{YOUBOTDIR}
  $ENV{YOUBOT_DIR}
  $ENV{ROBOTPKG_BASE}/include/youbot/
  ENV CPATH
  /usr/include/youbot/
  /usr/local/include/youbot/
  /opt/local/include/youbot/
  NO_DEFAULT_PATH
)

SET(OODL_YOUBOT_INCLUDE_DIR ${OODL_YOUBOT_INCLUDE_DIR} ${OODL_YOUBOT_INCLUDE_DIR}/soem/src/)


FIND_PATH(YOUBOT_CLIENT_SRC NAMES Client.hpp
  PATHS
  ${PROJECT_SOURCE_DIR}/../youBot_Client/src/
  NO_DEFAULT_PATH
)

FIND_LIBRARY(YOUBOT_CLIENT_LIB NAMES "youBot_Client"
  PATHS
  ${PROJECT_SOURCE_DIR}/../youBot_Client/lib/
  NO_DEFAULT_PATH
)



INCLUDE_DIRECTORIES(
${YOUBOT_CLIENT_SRC}
${OODL_YOUBOT_INCLUDE_DIR}
)

ADD_EXECUTABLE(youBot_Server
  ${PROJECT_SOURCE_DIR}/src/AbstractAction.cpp     
  ${PROJECT_SOURCE_DIR}/src/JointController.cpp  
  ${PROJECT_SOURCE_DIR}/src/PTPMotion.hpp
  ${PROJECT_SOURCE_DIR}/src/AbstractAction.hpp     
  ${PROJECT_SOURCE_DIR}/src/JointController.hpp  
  ${PROJECT_SOURCE_DIR}/src/Ramp_Generator.cpp
  ${PROJECT_SOURCE_DIR}/src/ArmCommander.cpp       
  ${PROJECT_SOURCE_DIR}/src/LinearVelocity.cpp   
  ${PROJECT_SOURCE_DIR}/src/Ramp_Generator.hpp
  ${PROJECT_SOURCE_DIR}/src/ArmCommander.hpp       
  ${PROJECT_SOURCE_DIR}/src/LinearVelocity.hpp   
  ${PROJECT_SOURCE_DIR}/src/SingleJoint.cpp
  ${PROJECT_SOURCE_DIR}/src/BaseCommander.cpp      
  ${PROJECT_SOURCE_DIR}/src/Loop_Rates.hpp       
  ${PROJECT_SOURCE_DIR}/src/SingleJoint.hpp
  ${PROJECT_SOURCE_DIR}/src/BaseCommander.hpp      
  ${PROJECT_SOURCE_DIR}/src/MotionCommander.cpp  
  ${PROJECT_SOURCE_DIR}/src/Trajectory.cpp
  ${PROJECT_SOURCE_DIR}/src/ControlManagement.cpp  
  ${PROJECT_SOURCE_DIR}/src/MotionCommander.hpp  
  ${PROJECT_SOURCE_DIR}/src/Trajectory.hpp
  ${PROJECT_SOURCE_DIR}/src/ControlManagement.hpp  
  ${PROJECT_SOURCE_DIR}/src/PID.cpp              
  ${PROJECT_SOURCE_DIR}/src/youBot_Server.cpp
  ${PROJECT_SOURCE_DIR}/src/GripperCommander.cpp   
  ${PROJECT_SOURCE_DIR}/src/PID.hpp
  ${PROJECT_SOURCE_DIR}/src/GripperCommander.hpp   
  ${PROJECT_SOURCE_DIR}/src/PTPMotion.cpp
)

TARGET_LINK_LIBRARIES(youBot_Server
${YOUBOT_CLIENT_LIB}
${OODL_YOUBOT_LIBRARIES}
boost_thread
zmq
msgpack
)




