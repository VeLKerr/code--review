#include <cstddef>
#include "HashFunction.h" 

using std::vector;

int64_t HashFunction::operator() (int64_t value) {
    int64_t pow = 1;
    int64_t result = 0;
    for (size_t i = 0; i < coefs.size(); ++i) {
        result = (result + coefs[i] * pow) % prime_module;
        pow = pow * value % prime_module;
    }
    return result;
}

HashFunction::HashFunction() : prime_module(1) {}

HashFunction::HashFunction(const vector<int64_t>& coefs, const int64_t prime_module)
    : coefs(coefs), prime_module(prime_module) {}
