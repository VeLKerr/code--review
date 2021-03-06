﻿#include <random>
#include <numeric>
#include <algorithm>
#include <assert.h>
#include "FixedSet.h" 

#define VARIANT_VAL 2

using std::vector;
using std::iterator;

const int PR_MODULE = 5308417;
const int PR_MODULE_INNER = 514229;
const int DEFAULT_VALUE = -1000000001;

HashFunction FixedSet::getHashCoefs(const int k_value, const int prime_module) {
    assert(prime_module > 1);
    using std::uniform_int_distribution;
    static uniform_int_distribution<int> distribution(0, prime_module - 1);
    static uniform_int_distribution<int> distribution_for_previous_coef(1, prime_module - 1);
    static std::default_random_engine generator;
    vector<int64_t> coefs;

    assert(k_value > 1);
    for (int i = 0; i < k_value - 1; ++i) {
        coefs.push_back(distribution(generator));
    }
    coefs.push_back(distribution_for_previous_coef(generator));
    return HashFunction(coefs, prime_module);
}

FixedSet::FixedSet() {}

void FixedSet::initialize(const vector<int>& numbers) {
    size_t summary_length;
    size_t table_size = 2 * numbers.size();
    vector<int> sizes;
    do {
        sizes.assign(table_size, 0);
        m_hash_func = getHashCoefs(2, PR_MODULE);
        std::for_each(numbers.cbegin(), numbers.cend(), [this, &sizes, &table_size](int number) {
            ++sizes[m_hash_func(number) % table_size];
        });
        summary_length = std::accumulate(sizes.cbegin(), sizes.cbegin() + table_size, 0, [](int sum, int elem) {
            return sum + pow(elem, 2);
        });
    } while (summary_length > 10 * table_size);

    m_baskets.resize(table_size);
    for (size_t i = 0; i < numbers.size(); ++i) {
        m_baskets[m_hash_func(numbers[i]) % table_size].push_back(numbers[i]);
    }
    m_inner_hashes.resize(table_size);
    m_hash_table.resize(table_size);
    
    /*=*
     Ниже -- прохо структурированный код: 
     for { if { do { for { if { -- это 5 уровней вложенности!
     Лучше int added_elements; do {...} -- переменести в отдельную именованную функцию,
     а внешний цикл for заменить на for_each.
     
     P.S. Тут можно развить фантазию и заменить for { if { ... }} на std::copy_if, если
     отдельную именованную функцию реализовать в вите итератора.
     */
    size_t currentIndex = 0;
#if VARIANT_VAL == 1
    vector<vector<int>> tmp_vect(table_size);
    std::copy_if(m_baskets.begin(), m_baskets.end(), tmp_vect.begin(), [this, &currentIndex](const vector<int> &element) -> bool{
        if (element.size() > 0) {
            computeHashes(currentIndex);
            currentIndex++; 
            return true;
        }
        currentIndex++;
        return false;
    });
#else
    std::for_each(m_baskets.begin(), m_baskets.end(), [this, &currentIndex](const vector<int> &element) {
        if (element.size() > 0) {
            computeHashes(currentIndex);
        }
        currentIndex++;
    });
#endif
}

void FixedSet::computeHashes(size_t index) {
    int added_elements;
    do {
        size_t cell_size = 4 * m_baskets[index].size() * m_baskets[index].size();
        m_hash_table[index].assign(cell_size, DEFAULT_VALUE);
        m_inner_hashes[index] = getHashCoefs(2, PR_MODULE_INNER);
        added_elements = 0;

        for (size_t j = 0; j < m_baskets[index].size(); ++j) {
            HashFunction curent_inner_hash = m_inner_hashes[index];
            int current_bascet = m_baskets[index][j];
            if (m_hash_table[index][curent_inner_hash(current_bascet)
                % cell_size] == DEFAULT_VALUE) {
                ++added_elements;
                m_hash_table[index][curent_inner_hash(current_bascet)
                    % cell_size] = current_bascet;
            }
        }
    }
    while (added_elements < m_baskets[index].size());
}

bool FixedSet::contains(int number) const {
    int bucket_num = m_hash_func(number) % m_hash_table.size();
    const vector<int> & bucket = m_hash_table[bucket_num];
    HashFunction hash = m_inner_hashes[bucket_num];
    return bucket.size() > 0 &&
        bucket[hash(number) % bucket.size()] == number;
}
