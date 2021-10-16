//
// refer : 
#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
using namespace std;


using point_t = std::vector< double >;
using pointIndex = std::pair< std::vector<double>, size_t >;

class KDNode {
public:
    using KDNodePtr = std::shared_ptr< KDNode >;
    point_t x;
    size_t index;
    KDNodePtr left;
    KDNodePtr right;

    KDNode();
    KDNode(const point_t &, const size_t &, const KDNodePtr &, const KDNodePtr &);
    KDNode(const pointIndex &, const KDNodePtr &, const KDNodePtr &);
    ~KDNode();

};


using KDNodePtr = std::shared_ptr< KDNode >;

KDNodePtr NewKDNodePtr() ;

using pointIndexArr = std::vector< pointIndex >;
using pointVec = std::vector< point_t >;

inline void sort_on_index(
    pointIndexArr::iterator & begin,
    pointIndexArr::iterator & end,
    size_t index
                    );

class KDTree{
    KDNodePtr root;

    KDNodePtr make_tree( pointIndexArr::iterator &begin,
                        pointIndexArr::iterator &end,
                        const size_t &length,
                        const size_t &level
                         );
    void printNode(KDNodePtr n){
        assert(n.get() != NULL);
        cout << n.get()->x[0] << "|" << n.get()->x[1] << endl;
        if (n.get()->left != NULL) printNode(n.get()->left);
        if (n.get()->right != NULL) printNode(n.get()->right);
    }

    public:
    KDTree() = default;
    explicit KDTree(pointVec);

    void printTree(){
        printNode(root);
    }

};
#endif