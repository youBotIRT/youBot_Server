PROJECT(youBot_Client)
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




SET(YOUBOT_CLIENT
  ${PROJECT_SOURCE_DIR}/src/Client.cpp      
  ${PROJECT_SOURCE_DIR}/src/Message.cpp   
  ${PROJECT_SOURCE_DIR}/src/youBot_IDs.hpp      
  ${PROJECT_SOURCE_DIR}/src/youBot_Request.hpp
  ${PROJECT_SOURCE_DIR}/src/Client.hpp      
  ${PROJECT_SOURCE_DIR}/src/Message.hpp   
  ${PROJECT_SOURCE_DIR}/src/youBot_Reply.cpp
  ${PROJECT_SOURCE_DIR}/src/Kinematics.cpp  
  ${PROJECT_SOURCE_DIR}/src/Watchdog.cpp  
  ${PROJECT_SOURCE_DIR}/src/youBot_Reply.hpp
  ${PROJECT_SOURCE_DIR}/src/Kinematics.hpp  
  ${PROJECT_SOURCE_DIR}/src/Watchdog.hpp  
  ${PROJECT_SOURCE_DIR}/src/youBot_Request.cpp
)

ADD_LIBRARY(youBot_Client  ${YOUBOT_CLIENT})
TARGET_LINK_LIBRARIES(youBot_Client youbotclient)


