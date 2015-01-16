#include "Heap.h"

using std::function;
using std::list;

Heap::Heap(function<bool(const list<MemSegment>::iterator& first,
    const list<MemSegment>::iterator& second)> compare) {
    this->m_compare = compare;
}

bool Heap::isEmpty() {
    return m_segments.size() == 0;
}

list<MemSegment>::iterator Heap::get_top() {
    return m_segments.at(0);
}

list<MemSegment>::iterator Heap::remove_last() {
    swap_segments(0, m_segments.size() - 1);
    list<MemSegment>::iterator last = m_segments.back();
    last->heapIndex = -1;
    m_segments.pop_back();
    sift_down(0);
    return last;
}

void Heap::insert_segment(const list<MemSegment>::iterator segment) {
    m_segments.push_back(segment);
    int last = m_segments.size() - 1;
    update_index(last);
    sift_up(last);
}

void Heap::release_segment(list<MemSegment>::iterator& segment) {
    int ind = segment->heapIndex;
    swap_segments(m_segments.size() - 1, ind);

    m_segments.pop_back();
    segment->heapIndex = -1;


    if (ind < m_segments.size()) {
        sift_down(ind);
        sift_up(ind);
    }
}

int Heap::left_index(const int ind) {
    return ind * 2 + 1;
}

int Heap::right_index(const int ind) {
    return left_index(ind) + 1;
}

int Heap::parent_index(const int ind) {
    return (ind + 1) / 2 - 1;
}

void Heap::sift_up(int index) {
    bool should_proceed = true;
    do {
        int parent = parent_index(index);
        if (parent >= 0 && m_compare(m_segments[parent], m_segments[index])) {
            swap_segments(index, parent);
            index = parent;
        } else {
            should_proceed = false;
        }
    } while (should_proceed);
}

void Heap::sift_down(int index) {
    bool should_proceed = true;
    do {
        int left = left_index(index);
        int right = right_index(index);
        int largest;

        if (left < m_segments.size() && m_compare(m_segments[index], m_segments[left])) {
            largest = left;
        } else {
            largest = index;
        }

        if (right < m_segments.size() && m_compare(m_segments[largest], m_segments[right])) {
            largest = right;
        }

        if (largest != index) {
            swap_segments(index, largest);
            index = largest;
        } else {
            should_proceed = false;
        }
    } while (should_proceed);
}

void Heap::swap_segments(const int first, const int second) {
    iter_swap(m_segments.begin() + first, m_segments.begin() + second);
    update_index(first);
    update_index(second);
}

void Heap::update_index(const int ind) {
    m_segments.at(ind)->heapIndex = ind;
}