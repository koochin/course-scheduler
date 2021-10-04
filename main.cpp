#include "include/course_graph.h"
#include "include/course_node.h"
#include "include/course_reader.h"
#include "include/course_writer.h"
#include "include/course_request.h"
#include <iostream>
#include <thread>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <mutex>
#include <vector>
#include <array>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#define DEBUG 0 // for printing


void validate_function(std::vector<std::vector<CourseNode>>& answers, std::mutex& index_mutex,
                       std::vector<int>& valid, std::mutex& valid_mutex, int& i) {
    /* 
        validate using sweeping line
    */
    int schedule_index = answers.size();
    index_mutex.lock();
    schedule_index = i++;
    index_mutex.unlock();
    while (schedule_index < answers.size()) {
        std::unordered_map<char, std::array<int, 2400>> line;
        auto schedule = answers[schedule_index];
        for (const auto& course : schedule) {
            for (const auto& [day, times] : course.get_days()) {
                std::string start_string = times.first;
                start_string.erase(start_string.find(":"));
                std::string end_string = times.second;
                end_string.erase(end_string.find(":"));
                int start = std::stoi(start_string);
                int end = std::stoi(end_string);
                for (int k = start; k <= end; k++) {
                    line[day][k]++;
                }
            }
        }
        bool no_conflict = true;
        for (const auto& [_, val] : line) {
            for (const auto& v : val) {
                if (v > 1) {
                    no_conflict = false;
                    break;
                }
            }
            if (!no_conflict) break;
        }
        if (no_conflict) {
            std::lock_guard<std::mutex> lock(valid_mutex);
            valid.push_back(schedule_index);
        }
        index_mutex.lock();
        schedule_index = i++;
        index_mutex.unlock();
    }
}

void recursive_search(std::vector<std::vector<CourseNode>>& answers, std::mutex& answers_mutex, 
                      const std::vector<std::vector<CourseNode>>& courses, int i, int j, std::vector<CourseNode>& list) {
    if (courses.empty()) return;
    if (j == courses.size()) {
        std::lock_guard<std::mutex> lock(answers_mutex);
        answers.push_back(list);
        return;
    }
    if (j == 0) {
        list.push_back(courses[j][i]);
        for (auto& child : courses[j][i].get_children()) {
            // LAB/TUT
            list.push_back(child);
            recursive_search(answers, answers_mutex, courses, 0, j+1, list);
            list.pop_back();
        }
        list.pop_back();
        return;
    }
    for (const auto& course : courses[j]) {
        // LEC
        list.push_back(course);
        auto children = course.get_children();
        for (auto& child : children) {
            // LAB/TUT
            list.push_back(child);
            recursive_search(answers, answers_mutex, courses, 0, j+1, list);
            list.pop_back();
        }
        if (children.empty()) {
            recursive_search(answers, answers_mutex, courses, 0, j+1, list);
        }
        list.pop_back();
    }
}

void search_function (std::vector<std::vector<CourseNode>>& answers,
                      std::mutex& answers_mutex,
                      const std::vector<std::vector<CourseNode>>& courses, int i) {
    std::vector<CourseNode> v;
    recursive_search(answers, answers_mutex, courses, i, 0, v);
}

void request_function (std::string& course_name, std::string& course_number, std::vector<CourseNode>& sections) {
    std::string request_url = Course_Request::FULL_URL_OUTLINE 
                              + course_name + "/" + course_number;
    cpr::Response r_sections = cpr::Get(cpr::Url{request_url});
    auto course_sections = nlohmann::json::parse(r_sections.text);
    for (auto& section : course_sections) {
        // text/value
        auto it = section.find("text");
        std::string course_section = *it;
        
        std::string outline_url = request_url + "/" + course_section;
        cpr::Response r_outline = cpr::Get(cpr::Url{outline_url});
        auto outline = nlohmann::json::parse(r_outline.text);
        
        // sectionCode
        auto it2 = outline.find("courseSchedule");
        auto schedule = *it2;

        std::string course_type = "";
        CourseNode cn(course_name, course_number, course_type, course_section);
        for (auto& s : schedule) {
            it = s.find("sectionCode");
            std::string course_type = *it;
            cn.set_type(course_type);

            it = s.find("days");
            std::string days = *it;
            std::vector<char> normalized_days;
            if (Course_Request::normalize_days.find(days) != Course_Request::normalize_days.end()) {
               normalized_days = Course_Request::normalize_days[days];
            }
            it = s.find("startTime");
            std::string start_time = *it;
            it = s.find("endTime");
            std::string end_time = *it;
            for (const auto& day : normalized_days) {
                cn.set_day(day, start_time, end_time);
            }
        }
        if (sections.empty() || course_section.substr(0, 2) != sections.back().get_section().substr(0, 2)) {
            sections.push_back(cn);
        }
        else {
            sections.back().add_child(cn);
        }
    }
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        std::cout << "Please enter file name to read from" << std::endl;
        return 0;
    }
    std::string file_name(argv[1]);
    std::cout << "Reading from: " << file_name << std::endl;
    CourseReader cr = CourseReader(file_name);
    std::string write_file_name = file_name;
    while (write_file_name.size() && write_file_name.back() != '.') {
        write_file_name.pop_back();
    }
    if (write_file_name.size()) write_file_name.pop_back();
    write_file_name += " schedules.txt";
    CourseWriter cw = CourseWriter(write_file_name);
    if (!cr.read_courses()) {
        std::cout << "Failed to read courses from: " << file_name << std::endl;
        return 0;
    }
    std::cout << "Successfully read courses" << std::endl;
    auto course_names = cr.get_courses();
    for (const auto& [course, number] : course_names) {
        std:: cout << course << " " << number << std::endl;
    }
    std::cout << "Requesting course information from SFU REST API" << std::endl;
    std::vector<std::thread> thread_pool(course_names.size());
    std::vector<std::vector<CourseNode>> courses(course_names.size());
    for (int i = 0; i < thread_pool.size(); i++) {
        thread_pool[i] = std::thread(request_function, 
                                     std::ref(course_names[i].first), 
                                     std::ref(course_names[i].second),
                                     std::ref(courses[i]));
    }
    for (auto& thread : thread_pool) {
        thread.join();
    }
    std::cout << "Finished all requests" << std::endl;
    #if DEBUG
    for (auto& sections : courses) {
        std::cout << "Number of sections: " << sections.size() << std::endl;
        for (auto& section : sections) {
            std::cout << section.get_info() << std::endl;
            for (auto& other : section.get_children()) {
                std::cout << other.get_info() << std::endl;
            }
        }
    }
    #endif

    std::mutex answers_mutex;
    std::vector<std::vector<CourseNode>> answers;
    std::unordered_set<std::string> seen;

    std::cout << "Generating schedules" << std::endl;
    for (int i = 0; courses.size() && i < courses[0].size(); i++) {
        thread_pool[i] = std::thread(search_function, std::ref(answers), 
                                                      std::ref(answers_mutex), 
                                                      std::ref(courses), i);
    }
    for (auto& thread : thread_pool) {
        thread.join();
    }
    std::cout << "Number of schedules generated: " << answers.size() << std::endl;
    
    std::cout << "Validating schedules" << std::endl;
    std::vector<int> valid;
    std::mutex valid_mutex;
    std::mutex index_mutex;
    int answers_index = 0;
    for (int i = 0; i < thread_pool.size(); i++) {
        thread_pool[i] = std::thread(validate_function, std::ref(answers), 
                                                        std::ref(index_mutex),
                                                        std::ref(valid), 
                                                        std::ref(valid_mutex), 
                                                        std::ref(answers_index));
    }
    for (auto& thread : thread_pool) {
        thread.join();
    }
    std::cout << "Number of valid schedules: " << valid.size() << std::endl;

    std::cout << "Writing to file: " << write_file_name << std::endl;
    for (const auto& i : valid) {
        cw.write_graph(answers[i]);
    }
    std::cout << "Done writing" << std::endl;
    std::cout << "Exiting" << std::endl;
}