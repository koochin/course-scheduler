#include "../include/course_graph.h"

CourseGraph::CourseGraph(const std::vector<std::vector<int>>& graph, const std::vector<CourseNode>& nodes) {
    this->graph = graph;
    this->nodes = nodes;
    this->V = nodes.size();
    int edges{};
    for (const auto& node : graph) {
        edges += node.size();
    }
    this->E = edges;
}

const std::vector<int>& CourseGraph::get_neighbours(const int& node) {
    return this->graph[node];
}

const CourseNode& CourseGraph::get_course(const int& node) {
    return this->nodes[node];
}