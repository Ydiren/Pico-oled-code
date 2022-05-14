# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/andy/Projects/github/RaspberryPi/Pico/pico-sdk/tools/elf2uf2"
  "/Users/andy/Projects/github/RaspberryPi/Pico/Pico-oled-code/c/build/elf2uf2"
  "/Users/andy/Projects/github/RaspberryPi/Pico/Pico-oled-code/c/build/elf2uf2"
  "/Users/andy/Projects/github/RaspberryPi/Pico/Pico-oled-code/c/build/elf2uf2/tmp"
  "/Users/andy/Projects/github/RaspberryPi/Pico/Pico-oled-code/c/build/elf2uf2/src/ELF2UF2Build-stamp"
  "/Users/andy/Projects/github/RaspberryPi/Pico/Pico-oled-code/c/build/elf2uf2/src"
  "/Users/andy/Projects/github/RaspberryPi/Pico/Pico-oled-code/c/build/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/andy/Projects/github/RaspberryPi/Pico/Pico-oled-code/c/build/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
