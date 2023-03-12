project(MyWindowFramework)

include(FetchContent)

set(SFML_VERSION "2.5.1")

FetchContent_Declare(
    sfml
    GIT_REPOSITORY  https://github.com/SFML/SFML.git
    GIT_TAG         ${SFML_VERSION}
)

FetchContent_GetProperties(sfml)

if(NOT sfml_POPULATED)
    message("SFML populating...")

    FetchContent_Populate(sfml)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()