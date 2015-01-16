#ifndef HEAP_H
#define HEAP_H

#include <list>
#include <vector>
#include <iterator>
#include <functional>
#include "MemSegment.h"

class Heap {
public:
    /**
     *	Constructor.
     *  <para>compare</para> - the function for comparison of two segments.
     */
    Heap(std::function<bool(const std::list<MemSegment>::iterator& first,
        const std::list<MemSegment>::iterator& second)> compare);

    /**
     *	Check if heap is empty.
     */
    bool isEmpty();

    /**
     *	Returns head (0th) segment of the heap.
     */
    std::list<MemSegment>::iterator get_top();

    /**
     *	Removes last segment.
     */
    std::list<MemSegment>::iterator remove_last();

    /**
     *	Insert segment from list to heap.
     */
    void insert_segment(const std::list<MemSegment>::iterator segment);

    /**
     *	Release memory of the corresponding segment.
     */
    void release_segment(std::list<MemSegment>::iterator& segment);

private:
    std::vector<std::list<MemSegment>::iterator> m_segments;

   /**
    *	The function for comparison of two segments.
    */
    std::function<bool(const std::list<MemSegment>::iterator& first,
        const std::list<MemSegment>::iterator& second)> m_compare;

    static int left_index(const int ind);
    static int right_index(const int ind);
    static int parent_index(const int ind);

    void sift_up(int index);
    void sift_down(int index);

    void swap_segments(const int first, const int second);
    void update_index(const int ind);
};

#endif // !HEAP_H
