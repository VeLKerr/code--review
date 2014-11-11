#include <iostream>
#include <numeric>
#include <algorithm>
#include <functional>
#include <iterator>
#include "assert.h"
#include "Football.h"

using std::vector;

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
    int64_t maxSum = std::accumulate(startIt, startIt + maxIndex + 1, 0LL); //calculation the efficiency of the primary team
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

    vector<int>::const_iterator minIndexIt = indexes.begin();    
    outputAnswer(minIndexIt + minIndex, minIndexIt + maxIndex + 1, maxSum);
    return 0;
}

void quickSort(vector<int> &values, const size_t first, const size_t last, vector<int> *indexes) {
    using std::swap;

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
    ________TODO: Перенести в файл с терией.
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
    using std::cin;

    int numberOfCandidates;
    cin >> numberOfCandidates;
    values.resize(numberOfCandidates);
    indexes.resize(numberOfCandidates);
    for (int i = 0; i < values.size(); ++i) {
        cin >> values.at(i);
        indexes.at(i) = i;
    }
}

void outputAnswer(vector<int>::const_iterator start, vector<int>::const_iterator end, const int64_t maxSum) {
    using std::cout;
    using std::endl;

    cout << maxSum << endl;
    transform(start, end, std::ostream_iterator<int>(cout, " "), std::bind1st(std::plus<int>(), 1));
    cout << endl;
}