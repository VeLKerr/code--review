#include <iostream>
#include <algorithm>
#include "Task.h"

#define CHECKING 0

using std::cin;
using std::cout;
using std::endl;

int main() {
    std::ios_base::sync_with_stdio(false);
    int slots_cnt;
    cin >> slots_cnt;
    int query_cnt;
    cin >> query_cnt;
    MemoryManager memory_manager(slots_cnt);

    for (int query_ind = 0; query_ind < query_cnt; ++query_ind) {
        executeQuery(memory_manager);
    }
#if CHECKING == 1
    system("pause");
#endif
    return 0;
}

void executeQuery(MemoryManager &memory_manager) {
    int query_mem;
    cin >> query_mem;
    if (query_mem > 0) {
        int allocation_result = memory_manager.processQuery(query_mem);
        if (allocation_result != -1) {
            allocation_result++;
        }
        cout << allocation_result << endl;
    } else if (query_mem < 0) {
        memory_manager.free(-query_mem - 1);
    }
}
