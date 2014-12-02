#ifndef FIXEDSET_H
#define FIXEDSET_H

#include "HashFunction.h"
 
/************************************************************************/
/* This is a class for storing set of integer numbers                   */
/************************************************************************/
class FixedSet {

    /**
     *	Here is the functions, given in task condition.
     */
public: 
    /*= 
      Тут вполне разумно сделано разделение стадий конструкции и инициализации,
      но конструктор, который создает не до конца доделанный объект, не должен быть доступен
      для использования извне.
      Лучше сделать так:

      public:
          static FixedSet create(const std::vector<int> &number);
      private:
          explicit FixedSet();
          void initialize(const std::vector<int> &numbers);
    */
    /*==*
    Такое объявление не совпадает с требованием задачи.
    (см. problems4.pdf, Task 4-3). Там прописано, какие методы должны быть у класса.
    Я тоже согласен с условием. Мне кажется, что у любой коллекции
    должен быть публичный конструктор без параметров, чтоб мы могли создать
    коллекцию в одном месте, а заполнить в другом.
    
    *=* OK. Условия задачи я не видел :-(
        
    */
    FixedSet();
    
    /**
     *	Initializing the set by the given vector.
     *  <para>numbers</para> - the given vector.
     */
    void initialize(const std::vector<int> &numbers);

    /**
     *	Checking if set contains given value.
     *  <para>number</para> - the given value.
     */

    /*==*
    bool contains(const int number) const 
    ругается на функтор. Считается, что если мы выполнили функтор, то объект изменился.
    Функтор можно заменить на обічную функцию, но тогда будет не так красиво. Система ШАД 
    же на отсутствие const не ругается. Главное, чтобы параметры и возвращаемые типы функций 
    initialize() и contains() были такими, как надо.
    
    *=* По смыслу, contains -- это проверка принадлежности числа множеству, которая,
    с точки зрения внешнего кода, не должна приводить к изменению объекта и может
    применяться к объекту FixedSet, который объявлен как const.    
    То, что при этом изменяется какое-либо внутреннее сосотояние, -- это уже особенность
    реализации, скрытая от внешнего интерфейса. В этом случае можно m_hash_func объявить
    как mutable (этот модификатор отменяет const; применяется исключительно для полей
    класса, которые предназначены для увеличения производительности, например хеши или 
    закэшированные значения).
    
    */
    bool contains(const int number);

private:
    std::vector<std::vector<int>> m_hash_table;
    std::vector<std::vector<int>> m_baskets;
    std::vector<HashFunction> m_inner_hashes;

    /**
     *	Functor for hash-function computation.
     */
    HashFunction m_hash_func;

    /**
    *   Create the vector of coefficients, which will used for computing hash.
    *   <para>k_value</para> - quantity of elements in vector with coefficients.
    *   <para>prime_module</para> - right limit of uniform distribution for generation coefs.
    */
    static HashFunction getHashCoefs(const int k_value, const int prime_module);
};

#endif
