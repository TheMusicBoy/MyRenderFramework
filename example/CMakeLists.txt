project(MyWindowFrameworkExample)

add_executable(mwf_example_exe main.cpp)

target_link_libraries(mwf_example_exe PRIVATE mwf_lib)