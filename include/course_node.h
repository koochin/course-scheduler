#ifndef COURSE_NODE_H
#define COURSE_NODE_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class CourseNode {
private:
    std::string course_department{};
    std::string course_number{};
    std::string course_type{};
    std::string section_number{};
    std::string info{};
    std::unordered_map<char, std::pair<std::string, std::string>> days;
    // the LEC is the parent and LAB/TUT is child
    std::vector<CourseNode> children;
public:
    CourseNode() = default;
    CourseNode(const std::string&, const std::string&, const std::string&, const std::string&);
    std::string get_department() const {return this->course_department;}
    std::string get_number() const     {return this->course_number;}
    std::string get_type() const       {return this->course_type;}
    std::string get_section() const    {return this->section_number;}
    const std::unordered_map<char, std::pair<std::string, std::string>>& get_days() const {return this->days;}
    std::string get_info() const;
    std::vector<CourseNode> get_children() const {return this->children;}
    void set_department(const std::string&);
    void set_number(const std::string&);
    void set_type(const std::string&);
    void set_section(const std::string&);
    void set_day(const char&, const std::string&, const std::string&);
    void add_child(const CourseNode&);
};

#endif