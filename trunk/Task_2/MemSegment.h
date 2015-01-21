#ifndef MEMSEGMENT_H
#define MEMSEGMENT_H

/*%
 * 1. Если код метода реализуется прямо в объявлении, то нужно
 * обязательно писать inline, иначе может возникнуть ситуация, когда
 * некоторые компиляторы (в MSVC2010 точно есть эти грабли) могуть
 * ругаться на повторную реализацию

===
Я поставил, но такого пока не встречал. Все ШАДовские задачи, которые не отправлялись на ревью, 
я решал в одном файле (система работает только с 1 файлом). Там не было разделения на 
объявление и реализацию.

Кроме того, inline на самом деле не всегда встраивается. Будет ли функция на самом деле inline решает компилятор. 
Если функция сложная, то она скорее всего не будет inline. 
Правда, можно встроить функцию в принудительном порядке. В MSVS для этого есть __forceinline.

 *
 * 4. Почему int, а не size_t (это ко всем файлам в задаче относится)?
 */

/*===
1. По условию задачи кол-во ячеек памяти не больше 2^31 - 1. Ячейка памяти для этой задачи - 
неделимая величина. Мы можем объединять ячейки в сегменты разных размеров, но разбивать 
на более мелкие кусочки не можем. Это значит, что для решения задачи нам хватит 
положительной части диапазона int.

2. Кол-во запросов к памяти по условию ограничено ещё меньшим числом (10^5).
*/

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
