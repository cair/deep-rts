# This module tries to find UWS library and include files
#
# UWS_INCLUDE_DIR, path where to find UWS.h
# UWS_LIBRARY_DIR, path where to find UWS.so
# UWS_LIBRARIES, the library to link against
# UWS_FOUND, If false, do not try to use UWS
#
# This currently works probably only for Linux

FIND_PATH ( UWS_INCLUDE_DIR uWS.h
        /usr/include/uWS
        /usr/include
        )

FIND_LIBRARY ( UWS_LIBRARIES libuWS.so
        /usr/local/lib
        /usr/lib
        )

GET_FILENAME_COMPONENT( UWS_LIBRARY_DIR ${UWS_LIBRARIES} PATH )

SET ( UWS_FOUND "NO" )
IF ( UWS_INCLUDE_DIR )
    IF ( UWS_LIBRARIES )
        SET ( UWS_FOUND "YES" )
    ENDIF ( UWS_LIBRARIES )
ENDIF ( UWS_INCLUDE_DIR )

MARK_AS_ADVANCED(
        UWS_LIBRARY_DIR
        UWS_INCLUDE_DIR
        UWS_LIBRARIES
)