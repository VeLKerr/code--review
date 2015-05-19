#ifndef COMPANY_H
#define COMPANY_H

#include <algorithm>
#include <vector>


/*%
 *% Чем подтверждено утверждение "works much faster"?
 *% Программа пропускалась через профилировщик?
 *% Вот реализация обращения к элементу vector<bool>
  reference
    operator[](size_type __n)
    {
      return *iterator(this->_M_impl._M_start._M_p
               + __n / int(_S_word_bit), __n % int(_S_word_bit));
    }

 *% Здесь не такой уж большой overhead для доступа. А в случае работы с векторам
 *% больших размеров, получается экономия по памяти.
 %*/

/**
 *	I used vector<char> instead of vector<bool> because it works much faster.
 *  vector<bool> uses a bitmap compressing for storing its elements.
 */

/*% Начиная с C++11 можно писать двойные угловые скобки без пробела между ними %*/
typedef std::vector<std::vector<size_t> > AdjacencyList;

/*% если используется соглашение о lower_case именах, то оно распространяется,
 *% в том числе, и на аббревиатуры %*/
extern void DFS_directed(size_t vertex, const AdjacencyList &direct,
    std::vector<size_t> &order, std::vector<char> &used);

extern void DFS_inversed(size_t vertex, const AdjacencyList &inverse,
    std::vector<char> &component, std::vector<char> &used);

extern void update_min_compon_size(const std::vector<char> &component,
    const AdjacencyList &inverse, size_t &minComponentSize);

extern void read_conditions(AdjacencyList &direct, AdjacencyList &inverse);

#endif // !COMPANY_H
