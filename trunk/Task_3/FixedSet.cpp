#include <random>
#include <numeric>
#include <assert.h>
#include "FixedSet.h" 

using std::vector;

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
    
    /*==*
    Не совсем понятно, зачем переносить assert() сюда.
    Мне кажется, что все ошибки желательно проверять как можно раньше, 
    чтоб не заставлять машину тратить лишнее время и память.
    */
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
        hash_func = getHashCoefs(2, PR_MODULE);
	
	/*= Используй std::copy или std::transform. У тебя все равно программа 
	компилируется только в режиме C++11 (из-за объявления вложенных template-параметров
	без пробелов в FixedSet.h), так что можешь использовать λ-выражения */
        for (size_t i = 0; i < numbers.size(); ++i) {
            ++sizes[hash_func(numbers[i]) % table_size];
        }
	/*= std::accumulate(from, to, 0, λ) */
        //summary_length = std::accumulate(0, table_size, 0, sizes);
        summary_length = 0;
        for (size_t i = 0; i < table_size; ++i) {
            summary_length += pow(sizes[i], 2);
        }
    }

    /*= Явный баг! Если условие внутри while(...) истинно, то программа повиснет) */
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
                hash_table[index].assign(cell_size, DEFAULT_VALUE);
                inner_hashes[index] = getHashCoefs(2, PR_MODULE_INNER);
                added_elements = 0;
                for (size_t j = 0; j < baskets[index].size(); ++j) {
		    /*= Получается очень запутанный код, который не разобрать, если
		      все время не подглядывать в .h-файл.
		      Лучше потратить несколько лишних строк и явно ввести несколько 
		      дополнительных переменных, чем писать столько квадратных
		      скобок в одной строке.
		      P.S. Код надо писать удобным для человека, а не компьютера.
		      Не бойся вводить временные переменные, если это улучшает 
		      читабельность, компилятор все равно их выкенет на этапе оптимизации
		    */
                    if (hash_table[index][inner_hashes[index](baskets[index][j])
                        % cell_size] == DEFAULT_VALUE) {
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

/*= 
  1. const bool -- относится к типу возвращаесого параметра, что для не-ref типов
  совершенно бессмысленно.
  Видимо, имелось ввиду, что метод contains не влияет на поля класса. Тогда ключевое
  слово const нужно писать после закрывающей круглой скобки
  2. Лучше использовать не bool contains(), а семантику STL (либо продублировать
  функциональность еще одним методом): 
    size_t FixedSet::count(int number) const; // возвращает количество элементов,
                                              // что не противоречит использованию
					      // этого метода как bool, поскольку:
                                              // count()==0  ===  contains()==false,
                                              // count()>0   ===  contains()==true.
                                              // bool(size_t >  0) == true  (всегда!)
                                              // bool(size_t == 0) == false
					      */

/*==*
В требованиях задачи есть условие, что contains() должна выполнятся за О(1).
Если же мы проходим все элементы и подсчитываем их, будет О(n).
*/
bool FixedSet::contains(int number) {
    int bucket_num = hash_func(number) % hash_table.size();
    /*= Используй const-ref, чтобы не копировать вектор целиком при создании
      локальной переменной */
    vector<int> bucket = hash_table[bucket_num];
    /*= Аналогичаное замечание (см. выше) про использования вложенных квадратных скобок */
    return bucket.size() > 0 &&
        bucket[inner_hashes[bucket_num](number) % bucket.size()] == number;
}
