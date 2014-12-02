#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include "Task.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

void inputData(vector<int> &vec) {
    using std::istream_iterator;
    int size;
    cin >> size;
    vec.resize(size);

    std::generate_n(vec.begin(), vec.size(), []() {
        int current;
        cin >> current;
        return current;
    });
}

void checking(FixedSet &set) {
    int queriesCnt;
    cin >> queriesCnt;
    for (int i = 0; i < queriesCnt; ++i) {
        int value;
        cin >> value;
        if (set.contains(value)) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    vector<int> vect;
    inputData(vect);

    FixedSet set;
    set.initialize(vect);
    
    checking(set);
    return 0;
}
