#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "Heap.h"

class MemoryManager {
public:
    /**
     *	Constructor.
     *  <para>slots</para> - quantity of memory slots.
     */
    /*% См. замечание про explicit в MemSegment.h */
    MemoryManager(const int slots);

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
    std::vector<std::list<MemSegment>::iterator> m_query_results;

    /**
    *	The function for comparison of two segments.
    */
    static bool compare_size(const std::list<MemSegment>::iterator& first,
        const std::list<MemSegment>::iterator& second);
};

#endif // !MEMORYMANAGER_H
