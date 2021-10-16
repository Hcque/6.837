#include "kdtree.hpp"
#include <algorithm>

KDNode::KDNode() = default;
KDNode::KDNode(const point_t & _p, const size_t & _i, const KDNodePtr & _left, const KDNodePtr & _right):
 x(_p), index(_i), left(_left), right(_right) {}

KDNode::KDNode(const pointIndex &_pI, const KDNodePtr &_l, const KDNodePtr &_r) : 
x(_pI.first), index(_pI.second), left(_l), right(_r)
{
   
}
KDNode::~KDNode() = default;

KDNodePtr KDTree::make_tree(pointIndexArr::iterator &begin,
                        pointIndexArr::iterator &end,
                        const size_t &length,
                        const size_t &level
                         )
{
   if (begin == end) return NewKDNodePtr(); 
   
   // n elements to sort
   // std::nth_element(begin, begin+length/2, end, [&](pointIndex &lhs, pointIndex &rhs) {
   //    return lhs.first[level] < rhs.first[level];
   // } );

   int dim = begin->first.size();
   auto middle = begin + length/2;
   auto left_l = begin;     auto left_r = middle;
   auto right_l = middle+1;     auto right_r = end;
   auto len_l = length/2;
   auto len_r = length - length/2 - 1;


   auto left = make_tree(left_l, left_r, len_l, level+1 % dim);
   auto right = make_tree(left_l, left_r, len_r, level+1 % dim);
   return std::make_shared< KDNode >(*middle, left, right);

}

KDNodePtr NewKDNodePtr() 
{
    KDNodePtr mynode = std::make_shared< KDNode >();
    return mynode;
}


inline void sort_on_index(
    pointIndexArr::iterator & begin,
    pointIndexArr::iterator & end,
    size_t index
                    )
{

}
KDTree:: KDTree(pointVec point_arr){
   pointIndexArr arr;
   for (int i = 0; i < point_arr.size(); ++i){
      arr.push_back( pointIndex( point_arr[i],i) );
   }
   auto begin = arr.begin();
   auto end = arr.end();

   root = KDTree::make_tree(begin, end, arr.size(), 0);
}

