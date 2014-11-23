#include <random>
#include <assert.h>
#include "FixedSet.h"

using std::vector;

HashFunction FixedSet::getHashCoefs(const int k_value, const int prime_module) {
    assert(k_value > 1 && prime_module > 1);
    using std::uniform_int_distribution;
    /*"static" in this function used for increasing performance.
    In this case variables distribution and distribution_for_previous_coef
    initializes only once. It initializes during first calling of this function.
    In this screenshot I will give a performance comparison with "static" and 
    without it: http://prntscr.com/598jcj .
    */
    static uniform_int_distribution<int> distribution(0, prime_module - 1);
    static uniform_int_distribution<int> distribution_for_previous_coef(1, prime_module - 1);
    static std::default_random_engine generator;
    vector<int64_t> coefs;
    for (size_t i = 0; i < k_value - 1; ++i) {
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
        hash_func = getHashCoefs(2, 5308417);
        for (size_t i = 0; i < numbers.size(); ++i) {
            ++sizes[hash_func(numbers[i]) % table_size];
        }
        summary_length = 0;
        for (size_t i = 0; i < table_size; ++i) {
            summary_length += pow(sizes[i], 2);
        }
    }
    while (summary_length > 10 * table_size);
    baskets.resize(table_size);
    for (size_t i = 0; i < numbers.size(); ++i) {
        baskets[hash_func(numbers[i]) % table_size].push_back(numbers[i]);
    }
    inner_hashes.resize(table_size);
    hash_table.resize(table_size);
    for (size_t index = 0; index < table_size; ++index) {
        if (baskets[index].size() > 0) {
            int added_elements;
            do {
                size_t cell_size = 4 * baskets[index].size() * baskets[index].size();
                hash_table[index].assign(cell_size, -1000000001);
                inner_hashes[index] = getHashCoefs(2, 514229);
                added_elements = 0;
                for (size_t j = 0; j < baskets[index].size(); ++j) {
                    if (hash_table[index][inner_hashes[index](baskets[index][j])
                        % cell_size] == -1000000001) {
                        ++added_elements;
                        hash_table[index][inner_hashes[index](baskets[index][j])
                            % cell_size] = baskets[index][j];
                    }
                }
            }
            while (added_elements < baskets[index].size());
        }
    }
}

const bool FixedSet::contains(int number) {
    int bucket_num = hash_func(number) % hash_table.size();
    vector<int> bucket = hash_table[bucket_num];
    return bucket.size() > 0 &&
        bucket[inner_hashes[bucket_num](number) % bucket.size()] == number;
}