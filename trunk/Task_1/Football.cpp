#include <iostream>
#include <numeric>
#include <algorithm>
#include "assert.h"
#include "Football.h"

//#define NDEBUG -- раскомментировать, чтоб отключить assert.

using std::swap;
using std::cin;
using std::cout;
using std::endl;
using std::advance;
using std::accumulate;
using std::vector;

void outputElement(int num);

int main() {
    vector<int> efficacies;
    vector<int> indexes;
    //По сути, 2 вышеописанных массива - это, Map. В задании запрещено использовать Map в явном виде.
    inputMap(efficacies, indexes);
    quickSort(&efficacies, 0, efficacies.size() - 1, &indexes);

    size_t maxIndex = binarySearch(efficacies, 0, efficacies.size()); //найдём наибольший № игрока, эффективность которого
    //не больше суммарной эффективности 1-го и 2-го
    vector<int>::iterator startIt = efficacies.begin();
    vector<int>::iterator endIt = efficacies.begin();
    advance(endIt, maxIndex + 1);
    long long maxSum = accumulate(startIt, endIt, 0); //рассчитаем эффективность первичной комманды
    long long sum = maxSum;
    size_t minIndex = 0; //минимальный индекс игрока в команде

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

    quickSort(&indexes, minIndex, maxIndex, nullptr);
    vector<int>::iterator minIndexIt = indexes.begin();
    vector<int>::iterator maxIndexIt = indexes.begin();
    advance(minIndexIt, minIndex);
    advance(maxIndexIt, maxIndex + 1);
    outputAnswer(indexes, maxSum, minIndexIt, maxIndexIt);
    system("pause");
    return 0;
}

/*=
3.  Не забываем про const
4.  Индексы далеко не всегда укладываются в тип int. Лучше использовать size_t: 
    во-первых, на 64-битных компьютерах область значений больше (вдруг через N лет этот код будет использоватся
    на компьютерах с десятками гигабайт оперативной памяти), а во-вторых, использование беззнакового типа 
    позволит компилятору отследить ошибки неправильного вызова функции с передачей отрицательного 
    индекса в качестве аргумента

  void quickSort(vector<int> &values, const size_t first, const size_t last, vector<size_t> *indexes)   
 */
void quickSort(vector<int> *values, const int first, const int last, vector<int> *indexes) {
    assert(first < values->size() && last < values->size());
    int i = first;
    int j = last;
    int x = values->at((first + last) / 2);
    do {
        while (values->at(i) < x && i < values->size()) {
            i++;
        }
        while (values->at(j) > x && j > 0) {
            j--;
        }
        if (i <= j) {
            if (values->at(i) > values->at(j)) {
                swap(values->at(i), values->at(j));
                if (indexes != nullptr) {
                    swap(indexes->at(i), indexes->at(j));
                }
            }
            i++;
            j--;
        }
    }
    while (i <= j && i < values->size() && j > 0);
    if (i < last) {
        quickSort(values, i, last, indexes);
    }
    if (first < j) {
        quickSort(values, first, j, indexes);
    }
}

/*== См. выше замечание про передачу аргументов через указатели, const и size_t */
int binarySearch(vector<int> &values, size_t firstIndex, size_t secondIndex) {
    /*= Будет не лишним добавить внутрь while дополнительные проверки, например, на выход за границы массива */
    assert(firstIndex <= values.size() && secondIndex <= values.size()); //лучше добавить в начале функции assert. 
    //Если поданы правильные индексы, дальше по ходу работы, выход за пределы массива не произойдёт.
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

long long countSum(const vector<int> &values, const size_t start, const size_t end) {
    long long sum = 0;
    for (size_t i = start; i <= end; ++i) {
        sum += values.at(i);
    }
    return sum;
}

/*==  См. выше замечание про передачу аргументов через указатели */
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

/*== 
1. См. выше замечание про передачу аргументов через указатели, const и size_t 
2. Зачем передавать сам вектор, его начало и конец? Достаточно итераторов начала и конца,
в этом случае алгоритм можно обобщить для использования другого типа контейнера (например list)
*/
void outputAnswer(vector<int> &indexes, const long long maxSum, vector<int>::iterator start, vector<int>::iterator end) {
    cout << maxSum << endl;
    std::for_each(start, end, outputElement); //дописать лямбду!!!!
    cout << endl;
}

void outputElement(int num){
	cout << num + 1 << " ";
}