#ifndef COURSE_WRITER_H
#define COURSE_WRITER_H

#include "course_graph.h"
#include "course_node.h"
#include <fstream>
#include <string>
#include <vector>

class CourseWriter {
private:
    std::string file_name{};
    std::ofstream file;
public:
    CourseWriter() = delete;
    CourseWriter(const std::string);
    bool write_graph(const std::vector<CourseNode>&);
};

#endif