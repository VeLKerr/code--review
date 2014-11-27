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
    Оставлю просто, без const тем более, что в условии дана именно такая сигнатура функции.
    */
    bool contains(const int number);

private:
    std::vector<std::vector<int>> hash_table;
    std::vector<std::vector<int>> baskets;
    std::vector<HashFunction> inner_hashes;

    /**
     *	Functor for hash-function computation.
     */
    HashFunction hash_func;

    /**
    *   Create the vector of coefficients, which will used for computing hash.
    *   <para>k_value</para> - quantity of elements in vector with coefficients.
    *   <para>prime_module</para> - right limit of uniform distribution for generation coefs.
    */
    HashFunction getHashCoefs(const int k_value, const int prime_module);
};

#endif
