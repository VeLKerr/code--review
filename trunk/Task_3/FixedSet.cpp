#include <random>
#include <numeric>
#include <algorithm>
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
    Не совсем понятно, зачем переносить assert() сюда?
    Мне кажется, что все ошибки желательно проверять как можно раньше, 
    чтоб не заставлять машину тратить лишнее время и память.
    
    Продублировать assert в данном месте полезно для читаемости кода.
    На производительность это не влияет, поскольку во многих реализациях C++
    assert -- это не функция, а макрос, который игнорируется в Release-режиме.
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
        m_hash_func = getHashCoefs(2, PR_MODULE);
	
	/*= Используй std::copy или std::transform. У тебя все равно программа 
	компилируется только в режиме C++11 (из-за объявления вложенных template-параметров
	без пробелов в FixedSet.h), так что можешь использовать λ-выражения */

        /*==*
        А чем плох обычный for_each?
        
        В данном случае -- тоже не плох. В предыдущей реализации у тебя был цикл for
        P.S. захват всех переменных в lambda-выражении -- избыточен.
        Лучше передавать только используемые переменные + this, если используются поля класса.
        
        */
        std::for_each(numbers.cbegin(), numbers.cend(), [&](int number) {
            ++sizes[m_hash_func(number) % table_size];
        });
        summary_length = std::accumulate(sizes.cbegin(), sizes.cbegin() + table_size, 0, [](int sum, int elem) {
            return sum + pow(elem, 2);
        });
    }
    /*= Явный баг! Если условие внутри while(...) истинно, то программа повиснет) */

    /*==*
    Это не так. Хэш-функция каждый раз генерируется с новыми коэффициентами, а значит элементы sizes будут 
    всё время новые => суммарная длина тоже будет принимать разные значения. Т.е. нет никакой гарантии, что если
    1 раз у нас значение summary_length > 10 * table_size = true, то на новой итерации оно не изменится.
    
    *=* Sorry, не обратил внимание, что while относится к предыдущему блоку do {}, а не 
    выполняет пустую инструкцию. Лучше в таких случаях писать закрывающую блок скобку     
    } и конструкцию while(..) в одной строке.
    */
    while (summary_length > 10 * table_size);

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
    for (size_t index = 0; index < table_size; ++index) {
        if (m_baskets[index].size() > 0) {
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
                            % cell_size] = m_baskets[index][j];
                    }
                }
            }
            while (added_elements < m_baskets[index].size());
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

*=* OK.
*/
bool FixedSet::contains(int number) {
    int bucket_num = m_hash_func(number) % m_hash_table.size();
    /*= Используй const-ref, чтобы не копировать вектор целиком при создании
      локальной переменной */
    /*==*
    Я не могу сюда подавать параметром элемент inner_hashes т.к, опять же,
    сигнатура функции жёстко прописана в требованиях.

    Можно сделать вот так...: 
    vector<int> bucket = (&hash_table)->at(bucket_num);
    но это, по сути, то же самое.
    
    *=* Подсказка:
    const ElementType & element = container_variable[index];
    */
    vector<int> bucket = m_hash_table[bucket_num];
    HashFunction hash = m_inner_hashes[bucket_num];
    return bucket.size() > 0 &&
        bucket[hash(number) % bucket.size()] == number;
}
