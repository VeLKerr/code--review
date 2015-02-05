#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "Heap.h"

class MemoryManager {
public:
    /**
     *	Constructor.
     *  <para>cells</para> - quantity of memory slots.
     */
    explicit MemoryManager(const int cells);

    /**
     *	Processing query (i.e. allocation memory for it).
     *  <para>size</para> - quantity of memory slots for query.
     */
    int processQuery(const int size);

    /**
     *	Releasing memory after execution query.
     *  <para>query_num</para> - number of query, after which released memory 
     */
    void free(const int query_num);

private:
    Heap m_heap;

    std::list<MemSegment> m_segments;

    std::vector<Heap::Segment_index> m_query_results;

    /**
    *	The function for comparison of two segments.
    */
    static bool compare_size(const Heap::Segment_index& first,
        const Heap::Segment_index& second);
};

#endif // !MEMORYMANAGER_H
