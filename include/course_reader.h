#ifndef COURSE_READER_H
#define COURSE_READER_H

#include <fstream>
#include <utility>
#include <vector>
#include <string>

class CourseReader {
private:
    std::string file_name{};
    std::ifstream file;
    std::vector<std::pair<std::string, std::string>> courses;
public:
    CourseReader() = delete;
    CourseReader(const std::string);
    bool read_courses();
    const std::vector<std::pair<std::string, std::string>>& get_courses();
};

#endif