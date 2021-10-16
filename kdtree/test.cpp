#include "kdtree.hpp"

int main() 
{
    pointVec points;
    point_t pt;
    pt = {0, 0};  points.push_back(pt);
    pt = {1, 0};  points.push_back(pt);
    pt = {0, 1};  points.push_back(pt);
    pt = {1, 1};  points.push_back(pt);
    pt = {.5, .5};  points.push_back(pt);

    KDTree kdtree = KDTree(points);
    kdtree.printTree();

    return 0;
}