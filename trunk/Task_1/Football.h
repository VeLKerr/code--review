/*==
1. Совместимость с С не предполагается.
==== OK.

2. Я понимаю, что было бы удобнее объявить свой namespace и там написать using std::vector, 
но Visual Studio ругается на namespace'ы вида:

namespace football {
    using std::vector;

    //other content
}
Это обсуждается на многих форумах потому что стандарт разрешает так писать.
==== Visual Studio (особенно 2008/2010) -- это нечто нецензурное, не стоит под него подстраиваться.
Если требуется обеспечить совместимость с VC, то придется писать полностью std::vector в объявлениях.
Еще раз повторюсь -- писать using в header-файлах НЕЛЬЗЯ!

Пример коллизии:
файл external_project.cpp
----------------
#include <SomeGreatLibrary/Vector.h>
#include "Footbal.h" // вдруг кому-то стало очень нужно

void foo() {
  SomeGreatLibrary::Vector vector; 
  // оппс... имя vector уже занято! А ведь здесь никто явно не просит using std::vector!
  ...
}

===== Вижу русские буквы :-)
Давай договоримся, что CodeReview/вопросы/замечания и пр. учебный процесс -- на русском, 
а остальное -- English only. В реальной практике нужно ориентироваться на командный процесс
разработки, и совсем не очевидно, что все разработчики в команде знают русский язык.
*/

#ifndef FOOTBALL_H
#define FOOTBALL_H
#endif

#include <vector>


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
extern void outputAnswer(std::vector<int>::iterator start, std::vector<int>::iterator end, const long long maxSum);

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