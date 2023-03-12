set(INCROOT ${PROJECT_SOURCE_DIR}/include/RenderFW/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/RenderFW/)

set(MY_WINDOW_FRAMEWORK_HEADERS
# Header files
    ${INCROOT}/impl/values.hpp
    ${INCROOT}/impl/render_object.hpp
    ${INCROOT}/impl/render_impl.hpp

)

set(MY_WINDOW_FRAMEWORK_SOURCES
# Source files
    ${SRCROOT}/impl/render_object.cpp
    ${SRCROOT}/impl/render_impl.cpp

)

add_library(mwf_lib
    ${MY_WINDOW_FRAMEWORK_HEADERS}
    ${MY_WINDOW_FRAMEWORK_SOURCES}
)

target_include_directories(mwf_lib PUBLIC ${INCROOT})

target_link_libraries(mwf_lib PUBLIC
    sfml-audio
    sfml-graphics
    sfml-system
    sfml-window
)