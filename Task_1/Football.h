#ifndef FOOTBALL_H //������������� � � �� ��������������.
#define FOOTBALL_H
#endif

#include <vector>

/**
* ���������� quickSort ��� ������� Map.
* QuickSort � ������ ������������ 2 ����.
* 1-� ���, ����� �� ��������� ������� � ������� ����������� �������������.
* 2-� - ����� ��������� �������, ������� ����� �����. �� ������ ������ ������������� ������� �� �����
* �.�. ��� �� ���������. �� ���� ������ ����� indexes ������ values � NULL ������ indexes.
* <para>values</para> - ������������ ������������� �������.
* <para>indexes</para> - ������ ������� (����� � Map). ����� ���� ����� null_ptr.
* <para>first</para>- � ������� �������� ����������.
* <para>last</para>- � ������� ��������.
*/
extern void quickSort(std::vector<int> *values, const int first, const int last, std::vector<int> *indexes);

/**
* ���������� ��������� Map.
* <para>values</para> - ������������ ������������� �������.
* <para>indexes</para> - ������ ������� (����� � Map).
*/
extern void inputMap(std::vector<int> *values, std::vector<int> *indexes);

/**
* ����� ������ � �������, ��������������� ����������� ������.
* <para>indexes</para> - ������ ������� (����� � Map).
* <para>maxSum</para> - ������������� �������.
* <para>start</para> - ������ ���������� ��������.
* <para>end</para> - ������ ��������� ��������.
*/
extern void outputAnswer(std::vector<int> *indexes, const long long maxSum, const size_t start, const size_t end);

/**
* ���������� ��������� ������ �� ����������
* <para>values</para> - ������������ ������������� �������.
* <para>firstIndex</para> - ������ ������ ����������.
* <para>secondIndex</para> - ������ ����� ����������.
*/
extern int binarySearch(std::vector<int> *values, size_t firstIndex, size_t secondIndex);

/**
* ���������� ��������� ������������� ����������.
* <para>values</para> - ������������ ������������� �������.
* <para>start</para> - ������ ���������� ��������.
* <para>end</para> - ������ ��������� ��������.
*/
//extern long long countSum(const std::vector<int> &values, const size_t start, const size_t end)