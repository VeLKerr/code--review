#include "MemoryManager.h"

using std::list;
using std::vector;

bool MemoryManager::compare_size(const list<MemSegment>::iterator& first,
    const list<MemSegment>::iterator& second) {
    if (first->size() != second->size()) {
        return first->size() < second->size();
    } else {
        return first->start > second->start;
    }
}

MemoryManager::MemoryManager(const int slots):m_heap(&MemoryManager::compare_size){
    MemSegment segment_of_mem(0, slots);
    m_segments.push_back(segment_of_mem);
    m_heap.insert_segment(m_segments.begin());
}

int MemoryManager::processQuery(const int size) {
    if (m_heap.isEmpty()) {
        m_query_results.push_back(m_segments.end());
        return -1;
    }
    int top_size = m_heap.get_top()->size();

    if (top_size < size) {
        m_query_results.push_back(m_segments.end());
        return -1;
    }

    list<MemSegment>::iterator seg_element = m_heap.remove_last();
    if (top_size == size) {
        seg_element->isFree = false;
        m_query_results.push_back(seg_element);
        return seg_element->start;
    } else {
        MemSegment busy_block = MemSegment(seg_element->start, seg_element->start + size, false);
        MemSegment free_block = MemSegment(seg_element->start + size, seg_element->end);

        list<MemSegment>::iterator busy_element = m_segments.insert(seg_element, busy_block);
        m_query_results.push_back(busy_element);

        list<MemSegment>::iterator free_element = m_segments.insert(seg_element, free_block);
        m_heap.insert_segment(free_element);

        m_segments.erase(seg_element);
        return busy_block.start;
    }
}

void MemoryManager::free(const int query_num) {
    m_query_results.push_back(m_segments.end());

    list<MemSegment>::iterator busy_element = m_query_results[query_num];
    if (busy_element == m_segments.end()) {
        return;
    }

    int start = busy_element->start;
    int end = busy_element->end;

    if (busy_element != m_segments.begin()) {
        list<MemSegment>::iterator previous = busy_element;
        --previous;

        if (previous->isFree) {
            start = previous->start;

            m_heap.release_segment(previous);
            m_segments.erase(previous);
        }
    }

    list<MemSegment>::iterator next = busy_element;
    ++next;

    if (next != m_segments.end() && next->isFree) {
        end = next->end;

        m_heap.release_segment(next);
        m_segments.erase(next);

        next = busy_element;
        ++next;
    }
    m_segments.erase(busy_element);

    MemSegment free_block = MemSegment(start, end);
    list<MemSegment>::iterator free_element = m_segments.insert(next, free_block);
    m_heap.insert_segment(free_element);
}