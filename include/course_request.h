#ifndef COURSE_REQUEST_H
#define COURSE_REQUEST_H

#include <string>
#include <ctime>
#include <vector>
#include <unordered_map>

/*
    http://www.sfu.ca/data-hub/api.html

    example course sections:
    https://www.sfu.ca//bin/wcm/course-outlines?2015/fall/bisc/202
    - year
    - term
    - department
    - courseNumber

    this returns a list of course sections
    - text/value
    - sectionCode

    example course outline:
    https://www.sfu.ca/bin/wcm/course-outlines?2015/summer/educ/100w/d100
    - year
    - term
    - department
    - courseNumber
    - courseSection

    this returns outline with
    - courseSchedule
        - startTime
        - endTime
        - days
*/

const std::string month_to_term[] = {"spring", "spring", "spring", "spring", 
                                     "summer", "summer", "summer", "summer", 
                                     "fall", "fall", "fall", "fall"};

namespace Course_Request {
    const std::string BASE_URL_OUTLINE = "https://www.sfu.ca/bin/wcm/course-outlines?";
    const std::time_t time = std::time(nullptr);
    const tm *local_tm = localtime(&time);
    const std::string YEAR = std::to_string(1900 + local_tm->tm_year);
    const std::string MONTH = std::to_string(1 + local_tm->tm_mon);
    const std::string TERM = month_to_term[local_tm->tm_mon];
    // example: https://www.sfu.ca/bin/wcm/course-outlines?2015/summer/
    const std::string FULL_URL_OUTLINE = BASE_URL_OUTLINE + YEAR + "/" + TERM + "/";
    std::unordered_map<std::string, std::vector<char>> normalize_days{{"Mo", std::vector<char>{'M'}},
                                                                      {"Tu", std::vector<char>{'T'}},
                                                                      {"We", std::vector<char>{'W'}},
                                                                      {"Th", std::vector<char>{'R'}},
                                                                      {"Fr", std::vector<char>{'F'}},
                                                                      {"Mo, We, Fr", std::vector<char>{'M', 'W', 'F'}}};
}

#endif