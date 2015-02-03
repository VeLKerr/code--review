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

    inline explicit MemSegment(int st, int end):
        start(st),
        end(end),
        isFree(true),
        heapIndex(-1){
    }

    inline explicit MemSegment(int st, int end, bool free):
        start(st),
        end(end),
        isFree(free),
        heapIndex(-1) {
    }

    /**
     *	Get size of the segment
     */
    inline int size() {
        return end - start;
    }
};

#endif // !MEMSEGMENT_H
