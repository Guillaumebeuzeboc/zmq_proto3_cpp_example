cmake_minimum_required(VERSION 2.8.3)
project(zmq_test)

add_compile_options(-std=c++11)

find_package(Protobuf REQUIRED)
find_package(ZeroMQ REQUIRED)

##### Build messages
file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/msg/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
add_library(messages STATIC ${ProtoSources} ${ProtoHeaders})
target_link_libraries(messages ${PROTOBUF_LIBRARY})

###########
## Build ##
###########
include_directories(
  include
  ${ZeroMQ_INCLUDE_DIR}
  ${Protobuf_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
)

add_executable(${PROJECT_NAME}_sub src/subscriber.cpp)
add_executable(${PROJECT_NAME}_pub src/publisher.cpp)


## Specify libraries to link a library or executable target against
 target_link_libraries(${PROJECT_NAME}_sub
     messages
     ${PROTOBUF_LIBRARIES}
     ${ZeroMQ_LIBRARY}
 )
 target_link_libraries(${PROJECT_NAME}_pub
     messages
     ${PROTOBUF_LIBRARIES}
     ${ZeroMQ_LIBRARY}
 )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_mqtt_test.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
