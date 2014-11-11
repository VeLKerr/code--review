#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>
#include "assert.h"
#include "Football.h"

/*=*=*=*
Когда становится слишком много using std::something,
лучше писать эти строки там, где они фактически используются.
Заключение в фигурные скобки ограничивает область видимости using.
*/

using std::swap;
using std::cin;
using std::cout;
using std::endl;
using std::accumulate;
using std::copy;
using std::vector;
using std::ostream_iterator;
using std::for_each;

int main() {
    vector<int> efficacies;
    vector<int> indexes;
    //In fact,these two arrays are the map.
    inputMap(efficacies, indexes);
    
    /*=*=*=*
    В этой строке опять ошибка, которая проявится при efficacies.size()==0
    */
    quickSort(efficacies, 0, efficacies.size() - 1, &indexes);

    size_t maxIndex = binarySearch(efficacies, 0, efficacies.size()); //find the greatest number of the player whose
    //effectiveness isn't larger than the total efficiency of the 1st and 2nd
    vector<int>::iterator startIt = efficacies.begin();
    int64_t maxSum = accumulate(startIt, startIt + maxIndex + 1, 0LL); //calculation the efficiency of the primary team
    int64_t sum = maxSum;
    size_t minIndex = 0;

    for (size_t i = 1; i + 1 < efficacies.size(); ++i) {
        sum -= efficacies.at(i - 1); //deleting i-th player from the team
        while (maxIndex + 1 < efficacies.size() && //looking for players that satisfy the condition of the current team 
            efficacies.at(maxIndex + 1) <= efficacies.at(i) + efficacies.at(i + 1)) {
            sum += efficacies.at(maxIndex + 1); //and add them to the team
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
      ==
      К сожаленю, я не нашёл никаких возможностей сразу получить итератор, который указывает не
      на начало или конец, а на какой-то индекс (аналог list.listIterator(int n) в Java).
      Единственная возможность передвинуть итератор на какой-то индекс - это advance().
      Но во всех функции присутствует 2 итератора (начальный и конечный).
      Тогда чтоб избавиться от antipattern'a "итератор + число", нам нужно:
      1. Объявить 2 итератора, установленных на начало.
      2. Прописать необходимые advance().
      3. Подать эти итераторы в функцию.
      Т.е. такой код:
        vector<int>::iterator minIndexIt = indexes.begin();
        outputAnswer(minIndexIt + minIndex, minIndexIt + maxIndex + 1, maxSum);
      превратится в такой:
        vector<int>::iterator minIndexIt = indexes.begin();
        vector<int>::iterator maxIndexIt = indexes.begin();
        std::advance(minIndexIt, minIndex);
        std::advance(maxIndexIt, maxIndex + 1);
        outputAnswer(minIndexIt, maxIndexIt, maxSum);
      ==
      P.S. А не проще minIndex сразу вычислять в виде итератора?
      ==
      Мне кажется, не проще. minIndex и maxIndex используется ещё в quickSort и цикле.
      Если заменить числа на итераторы, прийдётся всё время использовать
      std::distance(), что сильно усложнит код.
      
      *=*=*=* OK. Тут действительно будет усложнение кода, не сопоставимое с постановкой задачи,
      так что оставь как есть. Но нужно обязательно написать комментарий, что "данная строка 
      является хаком ради эффективности и работает только для random access iterator".
      
    */  
    outputAnswer(minIndexIt + minIndex, minIndexIt + maxIndex + 1, maxSum);

    /*=*=*=*
      Что-то я не обратил внимания на эту строку в прошлый раз.
      Убери паузу, либо засунь ее под #ifdef ... #endif.
      Программа должна завершать свою работу, а не ждать чего-либо.
      Кроме того, это платформо-зависимый код! (аргумент system -- это системная команда)
      */
    system("pause");
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
    ==== Можно и так, но вообще, это нужно формально доказывать.

    == Доказательство.
    Т.к. maxIndex = (firstIndex + secondIndex) / 2, то всегда выполняется firstIndex <= maxIndex <= secondIndex. 
    Это значит, что firstIndex во время работы только увеличивается, а secondIndex - только уменьшается.
    Поэтому, если на вход даны валидные индексы, они валидными и останутся.
    
    *=*=*=* English, please
    
    */
    assert(firstIndex <= values.size() && secondIndex <= values.size());
    size_t maxIndex = (firstIndex + secondIndex) / 2;
    const int64_t sum = values.at(firstIndex) + values.at(firstIndex + 1);
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
    int numberOfCandidates;
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

void outputAnswer(vector<int>::iterator start, vector<int>::iterator end, const int64_t maxSum) {
    cout << maxSum << endl;
    /*== Действительно, было бы легче сделать так:
        copy(start, end, ostream_iterator<int>(cout, " "));
    если бы не надо было выводить элементы, увеличенные на 1.
    А как в этом итераторе на лету изменять значения я пока не разобрался.
    *=*=*=*
    transform(start, end, ostream_iterator<int>(cout, " "), bind1st(plus<int>(), 1));
    
    Лучше так, чем заводить отдельную функцию в глобальном пространстве имен.
    P.S. Посмотри содержимое <algorithm> и <functional>, там вообще много чего есть интересного
    */
    for_each(start, end, outputElement);
    cout << endl;
}