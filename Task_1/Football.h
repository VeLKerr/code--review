#ifndef FOOTBALL_H
#define FOOTBALL_H

#include <vector>
#include <cstdint>

/**
* Заполнение элементов Map.
* <para>values</para> - коэффициенты эффективности игроков.
* <para>indexes</para> - номера игроков (ключи в Map).
*/
extern void inputMap(std::vector<int> &values, std::vector<int> &indexes);

/**
* Вывод ответа в формате, соответственном требованиям задачи.
* <para>indexes</para> - номера игроков (ключи в Map).
* <para>maxSum</para> - эффективность команды.
* <para>start</para> - индекс начального элемента.
* <para>end</para> - индекс конечного элемента.
*/
extern void outputAnswer(std::vector<int>::iterator start, std::vector<int>::iterator end, const int64_t maxSum);

/**
* Реализация quickSort для неявной Map.
* QuickSort в задаче используется 2 раза.
* 1-й раз, когда мы сортируем игроков в порядке возрастания эффективности.
* 2-й - когда сортируем игроков, которых нужно взять. Во втором случае эффективности игроков не важны
* т.к. они не выводятся. Во этом случае подаём indexes вместо values и NULL вместо indexes.
* <para>values</para> - коэффициенты эффективности игроков.
* <para>indexes</para> - номера игроков (ключи в Map). Может быть равен null_ptr.
* <para>first</para>- № первого элемента подмассива.
* <para>last</para>- № второго элемента.
*/
extern void quickSort(std::vector<int> &values, const size_t first, const size_t last, std::vector<int> *indexes);

/**
* Реализация бинарного поиска по подмассиву
* <para>values</para> - коэффициенты эффективности игроков.
* <para>firstIndex</para> - индекс начала подмассива.
* <para>secondIndex</para> - индекс конца подмассива.
*/
extern int binarySearch(std::vector<int> &values, size_t firstIndex, size_t secondIndex);

#endif