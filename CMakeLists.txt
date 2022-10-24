cmake_minimum_required(VERSION 3.19)

#########################
# Project configuration #
#########################

# Only define a project when this CMakeLists.txt is in the "root" (i.e., this is a standtalone build)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        project(TRANSPIRATION
                VERSION 0.1
                DESCRIPTION "MLIR-based transpiler for Fully Homomorphic Encryption (FHE)."
                LANGUAGES CXX C)
        set(TRANSPIRATION_STANDALONE_BUILD TRUE)
endif()

######################
# SEAL Configuration #
######################

find_package(SEAL 4.0 CONFIG) # sets SEAL_FOUND variable

###########################
# MLIR/LLVM Configuration #
###########################
if(TRANSPIRATION_STANDALONE_BUILD)

        set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR ON)

        # Locate MLIR, which recursively locates LLVM
        find_package(MLIR REQUIRED CONFIG)
        message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")
        message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

        set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/bin)
        set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/lib)

        list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
        list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

        include(TableGen)
        include(AddLLVM)
        include(AddMLIR)
        include(HandleLLVMOptions)

        include_directories(${LLVM_INCLUDE_DIRS})
        include_directories(${MLIR_INCLUDE_DIRS})
        include_directories(${PROJECT_SOURCE_DIR}/include)
        include_directories(${PROJECT_BINARY_DIR}/include)
        link_directories(${LLVM_BUILD_LIBRARY_DIR})
        add_definitions(${LLVM_DEFINITIONS})

        #TODO (Implementation) Handling of unit-tests, e.g. like in CIRCT https://github.com/llvm/circt/blob/fe1ddfc6e3cd2af7d4fa333897d2a4da8d4521f4/CMakeLists.txt#L84-L121

        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
                "${CMAKE_CURRENT_BINARY_DIR}/lib${LLVM_LIBDIR_SUFFIX}")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

else() # Not a standalone build
        #TODO Test out nested builds!
        message(WARNING "Non-Standalone build for Transpiration is untested/unsupported.")
endif()

###############################
# Transpiration Configuration #
###############################

list(APPEND CMAKE_MODULE_PATH "${MLIR_MAIN_SRC_DIR}/cmake/modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
include(AddTranspiration)

# Installing the headers and docs needs to depend on generating any public
# tablegen'd targets.
add_custom_target(transpiration-headers)
set_target_properties(transpiration-headers PROPERTIES FOLDER "Misc")
add_custom_target(transpiration-doc)

# Add MLIR and LLVM headers to the include path
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})

# Add Transpiration files to the include path
include_directories(${TRANSPIRATION_MAIN_INCLUDE_DIR})
include_directories(${TRANSPIRATION_INCLUDE_DIR})

##############################
# Compiler Setup (esp. MSVC) #
##############################

# Global flags aren't good CMAKE style, but these should work across pretty much all compilers
set(CXXFLAGS "--coverage --pedantic-errors -Wall -WError")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard to conform to")

##################################################
# Download and install nlohmann-json if required #
##################################################

find_package(nlohmann_json QUIET)
if (NOT nlohmann_json_FOUND)
    message("Downloading nlohmann_json")
    include(FetchContent)
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    set(JSON_SystemInclude ON CACHE INTERNAL "")
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
    FetchContent_Declare(
            nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG v3.7.3)
    FetchContent_MakeAvailable(nlohmann_json)
endif ()

###################
# Directory Setup #
###################
add_subdirectory(include/transpiration)
add_subdirectory(src)

install(DIRECTORY include/transpiration
  DESTINATION include
  COMPONENT transpiration-headers
  FILES_MATCHING
  PATTERN "*.def"
  PATTERN "*.h"
  PATTERN "*.inc"
  PATTERN "*.td"
  PATTERN "*.sv"
)

install(DIRECTORY ${TRANSPIRATION_INCLUDE_DIR}/transpiration
  DESTINATION include
  COMPONENT transpiration-headers
  FILES_MATCHING
  PATTERN "*.def"
  PATTERN "*.h"
  PATTERN "*.gen"
  PATTERN "*.inc"
  PATTERN "*.td"
  PATTERN "CMakeFiles" EXCLUDE
  PATTERN "config.h" EXCLUDE
)

if (NOT LLVM_ENABLE_IDE)
    add_llvm_install_targets(install-transpiration-headers
        DEPENDS transpiration-headers
        COMPONENT transpiration-headers
        
    )
endif()

add_subdirectory(cmake/modules)

# Set RPATH to $ORIGIN on all targets.
function(set_rpath_all_targets dir)
    get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirectories})
        set_rpath_all_targets(${subdir})
    endforeach()

    get_directory_property(LCL_TARGETS DIRECTORY ${dir} BUILDSYSTEM_TARGETS)
    set_property(TARGET ${LCL_TARGETS} PROPERTY INSTALL_RPATH "$ORIGIN/../lib")
endfunction()

option(STANDALONE_INSTALL "Create an 'install' for packaging which doesn't \
         require installation" off)
if (STANDALONE_INSTALL)
    message(STATUS "Setting an $ORIGIN-based RPATH on all executables")
    set_rpath_all_targets(${CMAKE_CURRENT_SOURCE_DIR})
endif()

################
# Installation #
################

include(GNUInstallDirs)

####################################
## TESTING
##
# added only if this is the root project
####################################
if (TRANSPIRATION_STANDALONE_BUILD)
    message("Enabling Transspiration Tests")
    enable_testing()
    add_subdirectory(test)
endif ()

##############################
# TARGET: benchmarks
#
# A collection of benchmarks
# added only if this is the root project
##############################
if (TRANSPIRATION_STANDALONE_BUILD)
  if (SEAL_FOUND)
      add_subdirectory(test/bench)
      add_subdirectory(test/IR/BGV)
  endif (SEAL_FOUND)
endif ()