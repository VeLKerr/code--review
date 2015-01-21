#ifndef HEAP_H
#define HEAP_H

#include <list>
#include <vector>
#include <iterator>
#include <functional>
#include "MemSegment.h"

class Heap {
public:
    typedef std::function<bool(const std::list<MemSegment>::iterator& first,
        const std::list<MemSegment>::iterator& second)> Compare_func;
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
    std::list<MemSegment>::iterator get_top() const;

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

    /*% Здесь опечатка в имени: sift --> shift */

    /*===
    Здесь всё правильно.
    sift - просеять.
    shift - сдвинуть.
    Для кучи существует термин именно просеивания 
    (http://neerc.ifmo.ru/wiki/index.php?title=%D0%94%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D0%B0%D1%8F_%D0%BA%D1%83%D1%87%D0%B0) - 
    это восстановление упорядоченности элементов в куче после изменения значения какого-нибудь из них.
    Именно просеивание я и использовал в этих методах.
    */
    void sift_up(int index);
    void sift_down(int index);

    void swap_segments(const int first, const int second);
    void update_index(const int ind);
};

#endif // !HEAP_H
