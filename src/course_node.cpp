#include "../include/course_node.h"

CourseNode::CourseNode(const std::string& department, const std::string& course, const std::string& type, const std::string& section) {
    this->course_department = department;
    this->course_number = course;
    this->course_type = type;
    this->section_number = section;
}

void CourseNode::set_department(const std::string& department) {
    this->course_department = department;
}

void CourseNode::set_number(const std::string& number) {
    this->course_number = number;
}

void CourseNode::set_type(const std::string& type) {
    this->course_type = type;
    this->info = this->course_department + " " + this->course_number  + "\n" +
                 this->course_type       + " " + this->section_number;
}

void CourseNode::set_section(const std::string& section) {
    this->section_number = section;
}

void CourseNode::set_day(const char& day, const std::string& start_time, const std::string& end_time) {
    this->days[day] = {start_time, end_time};
}

std::string CourseNode::get_info() const {
    std::string s = this->info;
    for (auto& [k, v] : days) {
        s += "\n" + std::string(1, k) + " " + v.first + "-" + v.second;
    }
    return s;
}

void CourseNode::add_child(const CourseNode& node) {
    children.push_back(node);
}