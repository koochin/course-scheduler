#include "../include/course_reader.h"

CourseReader::CourseReader(const std::string file_name) {
    this->file_name = file_name;
}

bool CourseReader::read_courses() {
    this->file.open(this->file_name);
    for (std::string line; std::getline(this->file, line);) {
        std:: string name, number;
        name = line.substr(0, line.find(" "));
        number = line.substr(line.find(" ")+1, line.size()-1);
        if (name == "" || number == "") {
            this->file.close();
            return false;
        }
        this->courses.push_back({name, number});
    }
    this->file.close();
    return true;
}

const std::vector<std::pair<std::string, std::string>>& CourseReader::get_courses() {
    return this->courses;
}