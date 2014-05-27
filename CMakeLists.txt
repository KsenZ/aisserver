CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

# Project
PROJECT( aisserver )

# Common CMake modules
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake )

SET( AISSERVER_SRC
  src/main.cpp
#  src/aisserver.cpp
)

#SET( AISSERVER_MOC_SRC
#  src/aisserver.h
#)

SET(BIN_TARGET aisserver)

# Building the executable
ADD_EXECUTABLE( ${BIN_TARGET}
    ${AISSERVER_SRC}
    ${AISSERVER_MOC}
)

TARGET_LINK_LIBRARIES(${BIN_TARGET}
    pq
)
