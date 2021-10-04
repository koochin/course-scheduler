cmake_minimum_required (VERSION 3.8)
project("course-scheduler")
set(CMAKE_CXX_STANDARD 17)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
add_executable (course-scheduler "main.cpp" "include/course_node.h" "src/course_node.cpp" 
                                            "include/course_graph.h" "src/course_graph.cpp" 
                                            "include/course_writer.h" "src/course_writer.cpp" 
                                            "include/course_reader.h" "src/course_reader.cpp" 
                                            "include/course_request.h")
target_link_libraries(course-scheduler PRIVATE nlohmann_json::nlohmann_json cpr)