#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H

#include <vector>
#include <cstdint>

/************************************************************************/
/*This is a class for computing hash-function using double-level hashing*/
/************************************************************************/
class HashFunction {

private:
    std::vector<int64_t> coefs;
    int64_t prime_module;

public:
   /**
    *	Default constructor.
    */
    HashFunction();

   /**
    *	Constructor.
    *  <para>coefs</para> - uniformly-distributed coefficients
    *  <para>prime_module</para> - right limit of uniform distribution for generation coefs.
    */
    HashFunction(const std::vector<int64_t>& coefs, const int64_t prime_module);

    /**
     *	This is a functor. Thanks to it, the HashFunction object has become a function.
     *  It has the same sense as if we wrote <code>int64_t countHash (int64_t value)</code>.
     */
    int64_t operator() (int64_t value);
};

#endif