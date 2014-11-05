#ifndef FOOTBALL_H //Совместимость с С не предполагается.
#define FOOTBALL_H
#endif

#include <vector>

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
extern void quickSort(std::vector<int> *values, const int first, const int last, std::vector<int> *indexes);

/**
* Заполнение элементов Map.
* <para>values</para> - коэффициенты эффективности игроков.
* <para>indexes</para> - номера игроков (ключи в Map).
*/
extern void inputMap(std::vector<int> *values, std::vector<int> *indexes);

/**
* Вывод ответа в формате, соответственном требованиям задачи.
* <para>indexes</para> - номера игроков (ключи в Map).
* <para>maxSum</para> - эффективность команды.
* <para>start</para> - индекс начального элемента.
* <para>end</para> - индекс конечного элемента.
*/
extern void outputAnswer(std::vector<int> *indexes, const long long maxSum, const size_t start, const size_t end);

/**
* Реализация бинарного поиска по подмассиву
* <para>values</para> - коэффициенты эффективности игроков.
* <para>firstIndex</para> - индекс начала подмассива.
* <para>secondIndex</para> - индекс конца подмассива.
*/
extern int binarySearch(std::vector<int> *values, size_t firstIndex, size_t secondIndex);

/**
* Рассчитать суммарную эффективность подмассива.
* <para>values</para> - коэффициенты эффективности игроков.
* <para>start</para> - индекс начального элемента.
* <para>end</para> - индекс конечного элемента.
*/
//extern long long countSum(const std::vector<int> &values, const size_t start, const size_t end)