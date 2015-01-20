#ifndef MEMSEGMENT_H
#define MEMSEGMENT_H

/*%
 * 1. Если код метода реализуется прямо в объявлении, то нужно
 * обязательно писать inline, иначе может возникнуть ситуация, когда
 * некоторые компиляторы (в MSVC2010 точно есть эти грабли) могуть
 * ругаться на повторную реализацию
 *
 * 2. Даже если не предусмотрен конструктор по умолчанию, то он все
 * равно неявно создается. Чтобы это ему запретить, нужно объявлять
 * конструкторы с модификатором explicit
 *
 * 3. Инициализацию полей структуры/класса в конструкторе обычно
 * делают следующим образом:
    MemSegment(int st, int end)
      : start(st)
      , end(end)
      , isFree(false)
      , heapIndex(-1)
    {
    }
    Это позволяет компилятору при создании объекта сразу инициализировать
    поля, а не выделять память в стеке при вызове конструктора и не
    выполнять лишнее присваивание
 *
 * 4. Почему int, а не size_t (это ко всем файлам в задаче относится)?
 */

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
