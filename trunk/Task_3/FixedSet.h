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
    bool contains(const int number) const;

private:
    std::vector<std::vector<int>> m_hash_table;
    std::vector<std::vector<int>> m_baskets;
    std::vector<HashFunction> m_inner_hashes;

    /**
     *	Functor for hash-function computation.
     */
    mutable HashFunction m_hash_func;

    /**
    *   Create the vector of coefficients, which will used for computing hash.
    *   <para>k_value</para> - quantity of elements in vector with coefficients.
    *   <para>prime_module</para> - right limit of uniform distribution for generation coefs.
    */
    static HashFunction getHashCoefs(const int k_value, const int prime_module);
};

#endif
