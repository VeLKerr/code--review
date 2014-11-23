#ifndef FIXEDSET_H
#define FIXEDSET_H

#include "HashFunction.h"

/************************************************************************/
/* This is a class for storing set of integer numbers                   */
/************************************************************************/
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
    FixedSet();
    void initialize(const std::vector<int> &numbers);
    //This function is const because it isn't changes object's fields.
    const bool contains(const int number);
};

#endif