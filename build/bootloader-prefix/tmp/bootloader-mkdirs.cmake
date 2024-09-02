# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/cicero/esp/esp-idf/components/bootloader/subproject"
  "/home/cicero/projects/tcc/software-esp/hydro-sense/build/bootloader"
  "/home/cicero/projects/tcc/software-esp/hydro-sense/build/bootloader-prefix"
  "/home/cicero/projects/tcc/software-esp/hydro-sense/build/bootloader-prefix/tmp"
  "/home/cicero/projects/tcc/software-esp/hydro-sense/build/bootloader-prefix/src/bootloader-stamp"
  "/home/cicero/projects/tcc/software-esp/hydro-sense/build/bootloader-prefix/src"
  "/home/cicero/projects/tcc/software-esp/hydro-sense/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/cicero/projects/tcc/software-esp/hydro-sense/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/cicero/projects/tcc/software-esp/hydro-sense/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
