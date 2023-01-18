#[[
  MIT License

  Copyright (c) 2021 vector-of-bool

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

////////////////////////////////////////////////////////////////////////////////

 PPPPPPPPPPPPPPPP     MMMMMM               MMMMMM  MMMMMM               MMMMMM
 P:::::::::::::::P    M:::::M             M:::::M  M:::::M             M:::::M
 P:::::PPPPPP:::::P   M::::::M           M::::::M  M::::::M           M::::::M
 PP::::P     P:::::P  M:::::::M         M:::::::M  M:::::::M         M:::::::M
   P:::P     P:::::P  M::::::::M       M::::::::M  M::::::::M       M::::::::M
   P:::P     P:::::P  M:::::::::M     M:::::::::M  M:::::::::M     M:::::::::M
   P:::PPPPPP:::::P   M:::::M::::M   M::::M:::::M  M:::::M::::M   M::::M:::::M
   P::::::::::::PP    M::::M M::::M M::::M M::::M  M::::M M::::M M::::M M::::M
   P:::PPPPPPPPP      M::::M  M::::M::::M  M::::M  M::::M  M::::M::::M  M::::M
   P:::P              M::::M   M:::::::M   M::::M  M::::M   M:::::::M   M::::M
   P:::P              M::::M    M:::::M    M::::M  M::::M    M:::::M    M::::M
   P:::P              M::::M     MMMMM     M::::M  M::::M     MMMMM     M::::M
 PP:::::PP            M::::M               M::::M  M::::M               M::::M
 P:::::::P            M::::M               M::::M  M::::M               M::::M
 P:::::::P            M::::M               M::::M  M::::M               M::::M
 PPPPPPPPP            MMMMMM               MMMMMM  MMMMMM               MMMMMM

]]

#[[ Version:

  888     888                           d8b
  888     888                           Y8P
  888     888
  Y88b   d88P  .d88b.  888d888 .d8888b  888  .d88b.  88888b.  d8b
   Y88b d88P  d8P  Y8b 888P"   88K      888 d88""88b 888 "88b Y8P
    Y88o88P   88888888 888     "Y8888b. 888 888  888 888  888
     Y888P    Y8b.     888          X88 888 Y88..88P 888  888 d8b
      Y8P      "Y8888  888      88888P' 888  "Y88P"  888  888 Y8P

  The version of PMM that will be downloaded if no PMM_VERSION was specified
  before include()-ing this file. You can change this version here to update
  the version of PMM that is used by your project: ]]

set(PMM_VERSION_INIT "2.1.0")

# (See the README below for more information)
#

#[[ README:

  8888888b.                        888      888b     d888
  888   Y88b                       888      8888b   d8888
  888    888                       888      88888b.d88888
  888   d88P .d88b.   8888b.   .d88888      888Y88888P888  .d88b.  d8b
  8888888P" d8P  Y8b     "88b d88" 888      888 Y888P 888 d8P  Y8b Y8P
  888 T88b  88888888 .d888888 888  888      888  Y8P  888 88888888
  888  T88b Y8b.     888  888 Y88b 888      888   "   888 Y8b.     d8b
  888   T88b "Y8888  "Y888888  "Y88888      888       888  "Y8888  Y8P

  ####################################################################

  This script is the "entrypoint" for PMM.

  This file should be COPIED and COMMITTED into the source tree of the project
  that wishes to use PMM. Do not use a file(DOWNLOAD), FetchContent(), or other
  pre-build script to download this file: It is intended to live in the source
  tree of its users and very rarely be manually updated. This file is
  in-and-of-itself a FetchContent()-like script that will download and import
  the full PMM code.

  This script will not automatically upgrade the version of PMM that is imported
  unless you specifically request it. You can be assured that the PMM code being
  pulled is deterministic over time.

  The version of PMM that is bootstrapped by this script is controlled by a
  single CMake variable 'PMM_VERSION' that can be set before include()-ing this
  file. Additionally, the 'set(PMM_VERSION_INIT)' line above this readme can be
  modified to control the PMM version.

  HINT: You can run this script with CMake directly via 'cmake -P pmm.cmake' for
  additional command-line functionality. ]]


#[[ Options:

   .d88888b.           888    d8b
  d88P" "Y88b          888    Y8P
  888     888          888
  888     888 88888b.  888888 888  .d88b.  88888b.  .d8888b  d8b
  888     888 888 "88b 888    888 d88""88b 888 "88b 88K      Y8P
  888     888 888  888 888    888 888  888 888  888 "Y8888b.
  Y88b. .d88P 888 d88P Y88b.  888 Y88..88P 888  888      X88 d8b
   "Y88888P"  88888P"   "Y888 888  "Y88P"  888  888  88888P' Y8P
              888
              888
              888

  ##############################################################

  The lines below set options that control how PMM is bootstrapped. These
  options can all be tweaked in one of two ways:

    - Change the default value of the option here in pmm.cmake in the associated
      call to pmm_option(). This is useful if you want to make a customized
      version of PMM and pmm.cmake to distribute and reuse between projects.

    - Set the associated variable before include()-ing this file. This is
      preferred for temporary changes, or for one-off projects that need custom
      values.

  The average user will not need to modify any variables except PMM_VERSION.

  Each option is documented below. Additional options and controls for PMM are
  documented in the README of the PMM repository.

  ]]

# (Helpful macro to set a variable if it isn't already set)
macro(pmm_option varname)
    if(NOT DEFINED "${varname}" AND NOT DEFINED "CACHE{${varname}}")
        set("${varname}" "${ARGN}")
    endif()
endmacro()

#[[ PMM_VERSION

  d8888b. .88b  d88. .88b  d88.         db    db d88888b d8888b. .d8888. d888888b  .d88b.  d8b   db
  88  `8D 88'YbdP`88 88'YbdP`88         88    88 88'     88  `8D 88'  YP   `88'   .8P  Y8. 888o  88
  88oodD' 88  88  88 88  88  88         Y8    8P 88ooooo 88oobY' `8bo.      88    88    88 88V8o 88
  88~~~   88  88  88 88  88  88         `8b  d8' 88~~~~~ 88`8b     `Y8b.    88    88    88 88 V8o88
  88      88  88  88 88  88  88          `8bd8'  88.     88 `88. db   8D   .88.   `8b  d8' 88  V888
  88      YP  YP  YP YP  YP  YP C88888D    YP    Y88888P 88   YD `8888Y' Y888888P  `Y88P'  VP   V8P

  'PMM_VERSION' will controls what version of PMM will be bootstrapped by this
  file.

  The initial value is controlled by 'PMM_VERSION_INIT', which is defined near
  the top of this file. Permanently updating the PMM version used by the
  project should be done by modifying 'PMM_VERSION_INIT' above directly rather
  than setting 'PMM_VERSION'

  ]]

pmm_option(PMM_VERSION ${PMM_VERSION_INIT})

#[[ PMM_URL_BASE

  d8888b. .88b  d88. .88b  d88.         db    db d8888b. db              d8888b.  .d8b.  .d8888. d88888b
  88  `8D 88'YbdP`88 88'YbdP`88         88    88 88  `8D 88              88  `8D d8' `8b 88'  YP 88'
  88oodD' 88  88  88 88  88  88         88    88 88oobY' 88              88oooY' 88ooo88 `8bo.   88ooooo
  88~~~   88  88  88 88  88  88         88    88 88`8b   88              88~~~b. 88~~~88   `Y8b. 88~~~~~
  88      88  88  88 88  88  88         88b  d88 88 `88. 88booo.         88   8D 88   88 db   8D 88.
  88      YP  YP  YP YP  YP  YP C88888D ~Y8888P' 88   YD Y88888P C88888D Y8888P' YP   YP `8888Y' Y88888P

  'PMM_URL_BASE' sets the root URL from which all versions of PMM can be
  downloaded. This value is used to construct 'PMM_URL'.

  ]]

pmm_option(PMM_URL_BASE "https://vector-of-bool.github.io/pmm")

#[[ PMM_URL

  d8888b. .88b  d88. .88b  d88.         db    db d8888b. db
  88  `8D 88'YbdP`88 88'YbdP`88         88    88 88  `8D 88
  88oodD' 88  88  88 88  88  88         88    88 88oobY' 88
  88~~~   88  88  88 88  88  88         88    88 88`8b   88
  88      88  88  88 88  88  88         88b  d88 88 `88. 88booo.
  88      YP  YP  YP YP  YP  YP C88888D ~Y8888P' 88   YD Y88888P

  'PMM_URL' is a URL that points to the "directory" that contains the PMM
  source code for a given version.

  The default value is the subdirectory of '${PMM_URL_BASE}' with a name of
  '${PMM_VERSION}'.

  ]]

pmm_option(PMM_URL "${PMM_URL_BASE}/${PMM_VERSION}")

#[[ PMM_DIR

  d8888b. .88b  d88. .88b  d88.         d8888b. d888888b d8888b.
  88  `8D 88'YbdP`88 88'YbdP`88         88  `8D   `88'   88  `8D
  88oodD' 88  88  88 88  88  88         88   88    88    88oobY'
  88~~~   88  88  88 88  88  88         88   88    88    88`8b
  88      88  88  88 88  88  88         88  .8D   .88.   88 `88.
  88      YP  YP  YP YP  YP  YP C88888D Y8888D' Y888888P 88   YD

  'PMM_DIR' is the base directory where PMM will write the bootstrapped copy
  of PMM. The default is a subdirectory of the CMake build directory qualified
  by the version of PMM that was downloaded.

  ]]

pmm_option(PMM_DIR "${CMAKE_BINARY_DIR}/_pmm/${PMM_VERSION}")

#[[ PMM_MODULE

  d8888b. .88b  d88. .88b  d88.         .88b  d88.  .d88b.  d8888b. db    db db      d88888b
  88  `8D 88'YbdP`88 88'YbdP`88         88'YbdP`88 .8P  Y8. 88  `8D 88    88 88      88'
  88oodD' 88  88  88 88  88  88         88  88  88 88    88 88   88 88    88 88      88ooooo
  88~~~   88  88  88 88  88  88         88  88  88 88    88 88   88 88    88 88      88~~~~~
  88      88  88  88 88  88  88         88  88  88 `8b  d8' 88  .8D 88b  d88 88booo. 88.
  88      YP  YP  YP YP  YP  YP C88888D YP  YP  YP  `Y88P'  Y8888D' ~Y8888P' Y88888P Y88888P

  'PMM_MODULE' is the absolute path to the pmm.cmake script (this file). It
  should not be changed or set unless you *really* know what you are doing.

  ]]

pmm_option(PMM_MODULE "${CMAKE_CURRENT_LIST_FILE}")

################################################################################
################################################################################
################################################################################
################################################################################

#[[

  The code below is the implementation of the bootstrapping process for PMM.
  You should not change any of the code below.

  ]]

# The file that we first download
set(_PMM_ENTRY_FILE "${PMM_DIR}/entry.cmake")

# Guard against multiple processes trying to use the PMM dir simultaneously
file(LOCK "${PMM_DIR}/_init-pmm"
        GUARD PROCESS
        TIMEOUT 10
        RESULT_VARIABLE _lock_res
        )
if(NOT _lock_res STREQUAL "0")
    message(WARNING "PMM entry didn't lock the directory [${PMM_DIR}] successfully (${_lock_res}). We'll continue as best we can.")
    set(_pmm_init_did_lock FALSE)
else()
    set(_pmm_init_did_lock TRUE)
endif()

if(NOT EXISTS "${_PMM_ENTRY_FILE}" OR PMM_ALWAYS_DOWNLOAD)
    file(
            DOWNLOAD "${PMM_URL}/entry.cmake"
            "${_PMM_ENTRY_FILE}.tmp"
            STATUS pair
    )
    list(GET pair 0 rc)
    list(GET pair 1 msg)
    if(rc)
        message(FATAL_ERROR "Failed to download PMM entry file: ${msg}")
    endif()
    file(RENAME "${_PMM_ENTRY_FILE}.tmp" "${_PMM_ENTRY_FILE}")
endif()

set(_PMM_BOOTSTRAP_VERSION 4)
include("${_PMM_ENTRY_FILE}")

if(_pmm_init_did_lock)
    file(LOCK "${PMM_DIR}/_init-pmm" RELEASE)
endif()