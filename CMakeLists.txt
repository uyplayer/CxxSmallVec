cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(CxxSmallVec
        VERSION 0.1.0
        DESCRIPTION "A lightweight, modern C++ alternative to Boost.Container’s `small_vector` — **faster, lighter, and more flexible.**"
        HOMEPAGE_URL "https://github.com/uyplayer/CxxSmallVec"
        LANGUAGES CXX C
)



file(GLOB_RECURSE CXXSMALLVEC_HEADERS
        include/*.h
        include/*.hpp
)

file(GLOB_RECURSE CXXSMALLVEC_SOURCES
        src/*.cpp
        src/*.cxx
)



add_library(CxxSmallVec STATIC  
        ${CXXSMALLVEC_HEADERS}
        ${CXXSMALLVEC_SOURCES}
)

target_include_directories(CxxSmallVec
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
