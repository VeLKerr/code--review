#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "Heap.h"

class MemoryManager {
public:
    /**
     *	Constructor.
     *  <para>slots</para> - quantity of memory slots.
     */
     /*>>
     Имена переменных signals или slots -- не очень хорошие.
     Это делает невозможным совместное использование кода с Qt (в частности Python.h этим страдает),
     кроме того неправильно работает подстветска синтаксиса в QtCreator, и, возможно, еще каких-нибудь
     IDE.
     */
    explicit MemoryManager(const int slots);

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
    
    /*>>
     * В Яндексе сказали, что это изврат. Обычно в таких случаях они пользуются обычными указателями.
     * Я поискал использования в коде, -- у тебя один раз используется декремент итератора, поэтому
     * без перепроектирования перейти на указатели не получится.
     *
     * Компромиссное решение: объяви typedef'ом этот итератор и дай ему какое-нибудь осмысленное
     * имя. При этом обязательно нужен комментарий вида: "не меняй std::list на что-нибудь другое,
     * иначе работать не будет".
     * P.S. Больше так не делай
     */
    std::vector<std::list<MemSegment>::iterator> m_query_results;

    /**
    *	The function for comparison of two segments.
    */
    static bool compare_size(const std::list<MemSegment>::iterator& first,
        const std::list<MemSegment>::iterator& second);
};

#endif // !MEMORYMANAGER_H
