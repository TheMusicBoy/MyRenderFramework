project(MyWindowFramework)

set(MY_WINDOW_FRAMEWORK_SOURCES
# Source files

)

add_library(mwf_lib
    ${MY_WINDOW_FRAMEWORK_HEADERS}
    ${MY_WINDOW_FRAMEWORK_SOURCES}
)

target_link_libraries(mwf_lib PUBLIC
    sfml-audio
    sfml-graphics
    sfml-system
    sfml-window
)