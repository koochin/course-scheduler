#ifndef COURSE_GRAPH_H
#define COURSE_GRAPH_H

#include "course_node.h"
#include <vector>

class CourseGraph {
private:
    int V{};
    int E{};
    std::vector<std::vector<int>> graph;
    std::vector<CourseNode> nodes;
public:
    CourseGraph() = delete;
    CourseGraph(const std::vector<std::vector<int>>&, const std::vector<CourseNode>&);
    int get_vertices() {return this->V;};
    int get_edges()    {return this->E;};
    const std::vector<int>& get_neighbours(const int&);
    const CourseNode& get_course(const int&);
};

#endif