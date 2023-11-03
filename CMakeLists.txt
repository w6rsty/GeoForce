cmake_minimum_required(VERSION 3.20)

project(learngl LANGUAGES CXX C)

aux_source_directory(src/ MAIN_SRC)
aux_source_directory(src/render MAIN_SRC)

add_executable(main ${MAIN_SRC})
target_precompile_headers(main PRIVATE include/pch.hpp)
set(EXPORT_COMPILE_COMMANDS ON)

target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PUBLIC include)
target_include_directories(main PUBLIC include/render)

add_subdirectory(3rdlibs)

target_link_libraries(main PUBLIC glfw imgui glm stb_image glad)

target_include_directories(main PUBLIC 3rdlibs/glfw/include)
target_include_directories(main PUBLIC 3rdlibs/imgui)
target_include_directories(main PUBLIC 3rdlibs/glad/include)

