/*==
На семинарах в ШАДе наоборот советовали писать не indexes[i], a indexes.at(i) т.к. at()
контролирует выход за границы массиива, выбрасывая out_of_range.

OK. Возражение принимается.
*/

#include <iostream>
#include <numeric>
#include <algorithm>
#include "assert.h"
#include "Football.h"

/*==== Эта строка не нужна. 
Включение/отключение NDEBUG (как и других специальный макросов) должно определяться конфигурацией сборки (опции компилятора -DИМЯ_ДЕФАЙНА),
а не редактированием текста.
Представь себе, что ты каждый раз комментируешь/разкомментируешь это строку для отладки. Эта правка является коммитом или нет? А вот VCS 
будут это коммитом считать.
*/
//#define NDEBUG //-- раскомментировать, чтоб отключить assert.

using std::swap;
using std::cin;
using std::cout;
using std::endl;
using std::accumulate;
using std::vector;

int main() {
    vector<int> efficacies;
    vector<int> indexes;
    //По сути, 2 вышеописанных массива - это, Map.
    inputMap(efficacies, indexes);
    quickSort(efficacies, 0, efficacies.size() - 1, &indexes);

    size_t maxIndex = binarySearch(efficacies, 0, efficacies.size()); //найдём наибольший № игрока, эффективность которого
    //не больше суммарной эффективности 1-го и 2-го
    vector<int>::iterator startIt = efficacies.begin();

    /*==
    Сделал, как Вы сказали, но не уверен, что нужно использовать accumulate().
    Вектор у нас содержит int, следовательно, accumulate() тоже вернёт int. В условии задачи сказано, что эффектиность может
    достигать 2^31 - 1, значит при сложении возможно переполнение. В моей же функции всё заносить в long long.
    */
    
    /*=====
    Вот реализация std::accumulate:
    
    template <class InputIterator, class T>
    T accumulate (InputIterator first, InputIterator last, T init)
    {
        while (first!=last) {
          init = init + *first;
          ++first;
        }
        return init;
    }
    
    Принципиальным здесь является тип шаблонного параметра T init. Если T==long long, но никаких проблем
    (для этого нужно вместо константы 0 писать 0LL).
    
    P.S. В Wiki ШАДа написана фигня по поводу использования long long. Я спросил у разработчиков из Яндекса, зачем такой изврат,
    когда есть нормальные типы данных в <cstdint>, мне подвтердили гипотезу: в реальности они вместо long long пользуются
    более короткой и понятной записью: int64_t, а почему в Wiki рекомендуется long long -- загадка природы. При решении следующих задач
    рекомендуется использовать типы данных из <cstdint> (см. C++ reference).
    
    */
    long long maxSum = accumulate(startIt, startIt + maxIndex + 1, 0); //рассчитаем эффективность первичной комманды
    long long sum = maxSum;
    size_t minIndex = 0;

    /*==== ОШИБКА!
    Что будет, если efficacies.size() == 0 ?
    Подсказка: typeof(std::vector.size()) == size_t == unsigned long long
    */
    for (size_t i = 1; i < efficacies.size() - 1; ++i) {
        sum -= efficacies.at(i - 1); //удаляем из комманды i-го игрока.
        while (maxIndex + 1 < efficacies.size() && //для текущей комманды ищем всех игроков, удовлетворяющих условию
            efficacies.at(maxIndex + 1) <= efficacies.at(i) + efficacies.at(i + 1)) {
            sum += efficacies.at(maxIndex + 1); //и добавляем их в команду
            if (sum > maxSum) {
                maxSum = sum;
                minIndex = i;
            }
            maxIndex++;
        }
    }

    quickSort(indexes, minIndex, maxIndex, nullptr);

    vector<int>::iterator minIndexIt = indexes.begin();
    
    /*==== 
      (итератор + число) = очень вредная привычка!
      
      В случае vector это работает, поскольку там random access iterator,
      но лучше таких конструкций избегать.
      
      P.S. А не проще minIndex сразу вычислять в виде итератора?
    */  
      
    outputAnswer(minIndexIt + minIndex, minIndexIt + maxIndex + 1, maxSum);

    return 0;
}

void quickSort(vector<int> &values, const size_t first, const size_t last, vector<int> *indexes) {
    assert(first < values.size() && last < values.size());
    int i = first;
    int j = last;
    int x = values.at((first + last) / 2);
    do {
        while (values.at(i) < x && i < values.size()) {
            i++;
        }
        while (values.at(j) > x && j > 0) {
            j--;
        }
        if (i <= j) {
            if (values.at(i) > values.at(j)) {
                swap(values.at(i), values.at(j));
                if (nullptr != indexes) {
                    swap(indexes->at(i), indexes->at(j));
                }
            }
            i++;
            j--;
        }
    }
    while (i <= j && i < values.size() && j > 0);
    if (i < last && i < values.size()) {
        quickSort(values, i, last, indexes);
    }
    if (first < j && j > 0) {
        quickSort(values, first, j, indexes);
    }
}

int binarySearch(vector<int> &values, size_t firstIndex, size_t secondIndex) {
    /*= 
    Вы писали: "Будет не лишним добавить внутрь while дополнительные проверки, например, на выход за границы массива".
    По-моему, лучше добавить 1 assert в начале функции. 
    Если на вход поданы правильные индексы, дальше по ходу работы, выход за пределы массива не произойдёт.
    
    ==== Можно и так, но вообще, это нужно формально доказывать.
    */
    assert(firstIndex <= values.size() && secondIndex <= values.size());
    size_t maxIndex = (firstIndex + secondIndex) / 2;
    long long const sum = values.at(firstIndex) + values.at(firstIndex + 1);
    while (firstIndex < secondIndex) {
        if (values.at(maxIndex) >= sum) {
            secondIndex = maxIndex;
        }
        else {
            firstIndex = maxIndex + 1;
        }
        maxIndex = (firstIndex + secondIndex) / 2;
    }
    return maxIndex;
}

void inputMap(vector<int> &values, vector<int> &indexes) {
    int numberOfCandidates; //кол-во кандидатов в команду
    cin >> numberOfCandidates;
    values.resize(numberOfCandidates);
    indexes.resize(numberOfCandidates);
    for (int i = 0; i < values.size(); ++i) {
        cin >> values.at(i);
        indexes.at(i) = i;
    }
}

/*===== 
Зачем заводить эту функцию?
Подсказка: std::ostream_iterator
*/

/**
* Вывод одного числа.
* <para>num</para> - число.
*/
void outputElement(int num) {
    cout << num + 1 << " ";
}

void outputAnswer(vector<int>::iterator start, vector<int>::iterator end, const long long maxSum) {
    cout << maxSum << endl;
    std::for_each(start, end, outputElement);
    cout << endl;
}