#ifndef FOOTBALL_H
#define FOOTBALL_H

#include <vector>
#include <cstdint>

/**
* Filling Map.
* <para>values</para> - player's efficiencies.
* <para>indexes</para> - player's indexes (keys in the Map).
*/
extern void inputMap(std::vector<int> &values, std::vector<int> &indexes);

/**
* Output solution formatted according to requirements of the preblem.
* <para>indexes</para> - player's indexes (keys in the Map).
* <para>maxSum</para> - cumulative efficiency of the team.
* <para>start</para> - beginning iterator.
* <para>end</para> - ending iterator.
*/
extern void outputAnswer(std::vector<int>::iterator start, std::vector<int>::iterator end, const int64_t maxSum);

/**
* QuickSort realisation for implicit Map.
* <para>values</para> - player's efficiencies.
* <para>indexes</para> - player's indexes (keys in the Map). This parameter may be equal to null_ptr.
* <para>first</para> - index of the beginning of submassive.
* <para>last</para> - index of the ending of submassive.
*/
extern void quickSort(std::vector<int> &values, const size_t first, const size_t last, std::vector<int> *indexes);

/**
* Binary search realization.
* <para>values</para> - player's efficiencies.
* <para>firstIndex</para> - index of the beginning of submassive.
* <para>secondIndex</para> - index of the ending of submassive.
*/
extern int binarySearch(std::vector<int> &values, size_t firstIndex, size_t secondIndex);

#endif