#ifndef MEMSEGMENT_H
#define MEMSEGMENT_H

/**
 *	Segment of memory.
 */
struct MemSegment {
    int start;
    int end;
    bool isFree;
    int heapIndex; //index in memory heap

    MemSegment(int st, int end) {
        this->start = st;
        this->end = end;
        isFree = true;
        heapIndex = -1;
    }

    MemSegment(int st, int end, bool free) {
        this->start = st;
        this->end = end;
        this->isFree = free;
        heapIndex = -1;
    }

    /**
     *	Get size of the segment
     */
    int size() {
        return end - start;
    }
};

#endif // !MEMSEGMENT_H
