# - Find python libraries
# This module finds if Python is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  PYTHON_FOUND           - have the Python libs been found
#  PYTHON_LIBRARIES       - path to the python library
#  PYTHON_INCLUDE_DIRS    - path to where Python.h is found
#

find_program(PYTHON_EXECUTABLE python)

EXECUTE_PROCESS(
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils import sysconfig; print sysconfig.get_python_inc()"
    OUTPUT_VARIABLE PYTHON_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

EXECUTE_PROCESS(
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils import sysconfig; print sysconfig.get_python_version()"
    OUTPUT_VARIABLE _CURRENT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

EXECUTE_PROCESS (
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils import sysconfig; print sysconfig.get_config_var('LIBDIR')"
    OUTPUT_VARIABLE _LIBDIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

EXECUTE_PROCESS (
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils import sysconfig; print sysconfig.get_config_var('prefix')"
    OUTPUT_VARIABLE _PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
IF(_CURRENT_VERSION MATCHES "\\d*\\.\\d*")
    STRING(REPLACE "." "" _CURRENT_VERSION_NO_DOTS ${_CURRENT_VERSION})
ENDIF()

FIND_LIBRARY(PYTHON_LIBRARY
    NAMES python${_CURRENT_VERSION_NO_DOTS} python${_CURRENT_VERSION}
    PATHS
        ${_LIBDIR}
        ${_PREFIX}/lib
        ${_PREFIX}/libs
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\${_CURRENT_VERSION}\\InstallPath]/libs
    # Avoid finding the .dll in the PATH.  We want the .lib.
    NO_DEFAULT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
)

# Python Should be built and installed as a Framework on OSX
IF(APPLE)
    message(${PYTHON_INCLUDE_DIR})
    STRING(REGEX MATCH ".*\\.framework" _FRAMEWORK_PATH "${PYTHON_INCLUDE_DIR}")
    IF(_FRAMEWORK_PATH)
        SET (PYTHON_LIBRARY "${_FRAMEWORK_PATH}/Python")
    ENDIF()
ENDIF(APPLE)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Python DEFAULT_MSG PYTHON_EXECUTABLE PYTHON_LIBRARY PYTHON_INCLUDE_DIR)

MARK_AS_ADVANCED(
    PYTHON_EXECUTABLE
    PYTHON_LIBRARY
    PYTHON_INCLUDE_DIR
)

MACRO(ADD_PYTHON_MODULE name)

    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR})
    ADD_LIBRARY(${name} MODULE ${ARGN})
    TARGET_LINK_LIBRARIES(${name} ${PYTHON_LIBRARY})

    SET_TARGET_PROPERTIES(${name} PROPERTIES PREFIX "")
    IF(CMAKE_HOST_WIN32)
        SET_TARGET_PROPERTIES(${name} PROPERTIES SUFFIX ".pyd")
    ENDIF(CMAKE_HOST_WIN32)

    # Store the module in the source tree
    GET_TARGET_PROPERTY(FILEPATH ${name} LOCATION)
    ADD_CUSTOM_COMMAND(
        TARGET ${name} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} 
        ARGS -E copy ${FILEPATH} ${CMAKE_CURRENT_SOURCE_DIR})

ENDMACRO(ADD_PYTHON_MODULE)
