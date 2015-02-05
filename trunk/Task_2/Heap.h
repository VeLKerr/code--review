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
     *	Type for heap index of memory segment.
     * 
     *  Don't change <code>std::list</code> to something else type of collection!
     *  Otherwise, the program may not work correctly.
     */
    typedef std::list<MemSegment>::iterator Segment_index;

    typedef std::function<bool(const Segment_index& first,
        const Segment_index& second)> Compare_func;
    /**
     *	Constructor.
     *  <para>compare</para> - the function for comparison of two segments.
     */
    inline Heap(Compare_func compare) : m_compare(compare) {
    };

    /**
     *	Check if heap is empty.
     */
    bool isEmpty() const;

    /**
     *	Returns head (0th) segment of the heap.
     */
    Segment_index get_top() const;

    /**
     *	Removes last segment.
     */
    Segment_index remove_last();

    /**
     *	Insert segment from list to heap.
     */
    void insert_segment(const Segment_index segment);

    /**
     *	Release memory of the corresponding segment.
     */
    void release_segment(Segment_index& segment);

private:
    std::vector<Segment_index> m_segments;

   /**
    *	The function for comparison of two segments.
    */
    Compare_func m_compare;

    static inline int left_index(const int ind) {
        return ind * 2 + 1;
    }

    static inline int right_index(const int ind) {
        return left_index(ind) + 1;
    }

    static inline int parent_index(const int ind) {
        return (ind + 1) / 2 - 1;
    }

    void sift_up(int index);
    void sift_down(int index);

    void swap_segments(const int first, const int second);
    void update_index(const int ind);
};

#endif // !HEAP_H
