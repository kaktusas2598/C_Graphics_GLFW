cmake_minimum_required(VERSION 3.16)

project(C_Graphics_GLFW C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ----------------------------
# Source files (auto include)
# ----------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.c
)

add_executable(app ${SOURCES})

# ----------------------------
# Include directories
# ----------------------------
target_include_directories(app PRIVATE include)

# GLAD include path
target_include_directories(app PRIVATE
    vendor/glad/include
)

# GLAD source (THIS is the "compile GLAD" step)
target_sources(app PRIVATE
    vendor/glad/src/glad.c
)

# ----------------------------
# Platform-specific linking
# ----------------------------

# MSYS2 / MinGW
if (MINGW)
    target_link_libraries(app
        glfw3
        opengl32
    )
endif()

