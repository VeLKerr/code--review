#include <cstddef>
#include "HashFunction.h" 

using std::vector;

int64_t HashFunction::operator() (int64_t value) {
    int64_t pow = 1;
    int64_t result = 0;
    for (size_t i = 0; i < m_coefs.size(); ++i) {
        result = (result + m_coefs[i] * pow) % m_prime_module;
        pow = pow * value % m_prime_module;
    }
    return result;
}

HashFunction::HashFunction() : m_prime_module(1) {}

HashFunction::HashFunction(const vector<int64_t>& coefs, const int64_t prime_module)
    : m_coefs(coefs), m_prime_module(prime_module) {}
