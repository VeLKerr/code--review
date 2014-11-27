#ifndef FIXEDSET_H
#define FIXEDSET_H

#include "HashFunction.h"
 
/************************************************************************/
/* This is a class for storing set of integer numbers                   */
/************************************************************************/

/*= 
  1. См. общие замечания из HashFunction.h
  2. Документация на public часть класса оформляется в стиле Doxygen,
    а private -- можно и обычными комментариями. У тебя сделано наоборот
*/
class FixedSet {

private:
    std::vector<std::vector<int>> hash_table;
    std::vector<std::vector<int>> baskets;
    std::vector<HashFunction> inner_hashes;
    HashFunction hash_func;

   /**
    *   Create the vector of coefficients, which will used for computing hash.
    *   <para>k_value</para> - quantity of elements in vector with coefficients.
    *   <para>prime_module</para> - right limit of uniform distribution for generation coefs.
    */
    HashFunction getHashCoefs(const int k_value, const int prime_module);

public: //Here is the functions, given in task condition.
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
    void initialize(const std::vector<int> &numbers);

    /*= Этот комментарий избыточен, посколько описывает стандартную практику.
      Кроме того, он не соответствует действительности :-)
      (см. замечание в .cpp-файле) */
    //This function is const because it isn't changes object's fields.
    const bool contains(const int number);
};

#endif
