#include "../include/course_writer.h"

CourseWriter::CourseWriter(const std::string file_name) {
    this->file_name = file_name;
}

bool CourseWriter::write_graph(const std::vector<CourseNode>& schedule) {
    this->file.open(this->file_name, std::ios::app);
    for (int i = 0; i < schedule.size(); i++) {
        this->file << schedule[i].get_info() << "\n";
    }
    this->file << "\n";
    this->file.close();
    return true;
}