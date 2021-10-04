# University Course Scheduler

Reads a list of courses from a file and generates all non-conflicting schedules. *Work in progress*.

## Dependencies

- C++17
- CMake
- [libcpr/cpr](https://github.com/libcpr/cpr)
- [nlohmann/json](https://github.com/nlohmann/json)

## Installation

1. You will need a package manager, for [vcpkg](https://vcpkg.io/en/index.html) follow:
```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install nlohmann-json
./vcpkg install cpr
./vcpkg integrate install
```
2. Install CMake:
```
sudo apt install cmake
```
3. Then proceed with:
```
git clone git@github.com:koochin/course-scheduler.git
cd course-scheduler
mkdir build
cd build
cmake -B build/ -S .. -DCMAKE_TOOLCHAIN_FILE=/home/liam/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```
**Note:** replace `/home/liam/` with the path to your vcpkg root directory  

4. Then run with:
```
./course-scheduler ../test.txt
```
`./course-scheduler [FILENAME]` takes one input **FILENAME** which is the relative path from `build/` to the file containing a list of courses. See [test.txt](https://github.com/koochin/course-scheduler/blob/main/test.txt) for an example.

## Input

Each course should be on a single line escaped with `\n` and follow Simon Fraser Universities' [naming convention](https://www.sfu.ca/students/calendar/2021/fall/courses.html) of `[DEPARTMENT] [NUMBER]` (example CMPT 120). There should be a single space between **DEPARTMENT** and **NUMBER**.

## Output

The computed schedules will be placed in a file with name "**FILENAME schedules**" in the same location as the input **FILENAME**.

## About

This application is written in C++17 and utilizes the Standard Library including `std::thread` for multithreading. It uses [cpr](https://github.com/libcpr/cpr) to make requests to Simon Fraser University's [Public APIs](https://www.sfu.ca/data-hub/api.html) which contains detailed course information and Niels Lohmann's [JSON](https://github.com/nlohmann/json) library for parsing of the responses. The current implementation leverages a parallelized [recursive backtracking search algorithm](https://en.wikipedia.org/wiki/Backtracking) to generate all possible schedules and then filters valid schedules with a parallelized [sweeping line algorithm](https://en.wikipedia.org/wiki/Sweep_line_algorithm). Although course scheduling is a well studied problem and may be better solved with [CSP](https://en.wikipedia.org/wiki/Constraint_satisfaction_problem) an average schedule has no more than five courses per semester which provides a small enough search space for proof of concept.
