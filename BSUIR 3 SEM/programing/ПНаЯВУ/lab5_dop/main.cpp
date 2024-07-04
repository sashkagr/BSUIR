#include <cstdio>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int N = 6;
const int SIDES = 4;
const int MASK = (1 << N) - 1;
vector<int> possible(N * N, 0);
vector<int> s(SIDES * N, 0);
vector<int> e(SIDES * N, 0);
vector<int> inc(SIDES * N, 0);
vector<vector<int>> results(N, vector<int>(N, 0));

vector<int> my_clues;
vector<int> order;

void set_value(int x, int v) {
    int m = MASK ^ (1 << v);
    int s_row = x - x % N;
    int s_col = x % N;
    for (int i = 0; i < N; i++) {
        possible[s_row + i] &= m;
        possible[s_col + i * N] &= m;
    }
    possible[x] = 1 << v;
}

int check_unique() {
    int n_decides = 0;
    for (int i = 0; i < SIDES / 2 * N; i++) {

        map<int, vector<int> > possible_indices;
        for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
            for (int l = 0; l < N; l++)
                if ((1 << l) & possible[j]) {
                    possible_indices[l].push_back(j);
                }
        }

        for (auto const &iter : possible_indices) {
            int val = iter.first;
            if (iter.second.size() == 1) {
                int idx = iter.second[0];
                if (possible[idx] != (1 << val)) {
                    n_decides++;
                    set_value(idx, val);
                }
            }
        }
    }
    return n_decides;
}

int count_possible(int val) {
    int n = 0;
    while (val) {
        n += val & 1;
        val >>= 1;
    }
    return n;
}

bool valid() {
    for (int i = 0; i < SIDES * N; i++) {
        if (my_clues[i] == 0) continue;

        bool is_decided = true;
        for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
            if (count_possible(possible[j]) != 1) {
                is_decided = false;
                break;
            }
        }

        if (is_decided) {
            int largest = 0, n_clue = 0;
            for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
                if (largest < possible[j]) {
                    n_clue++;
                    largest = possible[j];
                }
            }
            if (n_clue != my_clues[i]) return false;
        }
    }

    return true;
}

void write_results() {
    for (int i = 0; i < N * N; i++) {
        int x = i / N, y = i % N;
        for (int j = 0; j < N; j++) {
            if ((1 << j) == possible[i]) {
                results[x][y] = j + 1;
                break;
            }
        }
    }
}

bool dfs(int idx) {
    if (idx >= order.size()) {
        if (valid()) {
            write_results();
            return true;
        }
        return false;
    }

    int i = order[idx];
    vector<int> possible_bak(N * N, 0);
    possible_bak = possible;
    for (int j = 0; j < N; j++) {
        int m = (1 << j) & possible[i];
        if (m == 0) continue;

        set_value(i, j);
        bool found = false;
        if (valid()) {
            found = dfs(idx + 1);
        }
        if (found) {
            return true;
        }
        possible = possible_bak;
    }
    return false;
}

vector<vector<int>> SolvePuzzle(const vector<int> &clues) {
    my_clues = clues; // Присваиваем переданные подсказки моей переменной my_clues

    for (int i = 0; i < N * N; i++) possible[i] = MASK; // Инициализируем вектор possible значениями MASK

    // Определение начальных значений s, e и inc для каждой из 4 сторон (верх, низ, лево, право)
    for (int i = 0; i < N; i++) {
        s[i] = i; // Начальная позиция для верхней стороны
        e[i] = (N - 1) * N + i; // Начальная позиция для нижней стороны
        inc[i] = N; // Шаг для верхней и нижней стороны
    }

    for (int i = 0, j = N; i < N; i++, j++) {
        s[j] = i * N + N - 1; // Начальная позиция для левой стороны
        e[j] = i * N; // Начальная позиция для правой стороны
        inc[j] = -1; // Шаг для левой и правой стороны
    }

    for (int i = 0, j = 2 * N; i < N; i++, j++) {
        s[j] = N * N - 1 - i; // Начальная позиция для нижней стороны слева направо
        e[j] = N - 1 - i; // Начальная позиция для верхней стороны справа налево
        inc[j] = -N; // Шаг для нижней стороны слева направо и верхней справа налево
    }

    for (int i = 0, j = 3 * N; i < N; i++, j++) {
        s[j] = (N - i - 1) * N; // Начальная позиция для верхней стороны слева направо
        e[j] = (N - i) * N - 1; // Начальная позиция для нижней стороны справа налево
        inc[j] = 1; // Шаг для верхней стороны слева направо и нижней справа налево
    }

    for (int i = 0; i < SIDES * N; i++)
    {
        if (my_clues[i] == 0) continue; // Пропускаем, если подсказка отсутствует
        for (int j = s[i], k = 0; k < N; j += inc[i], k++) {
            int m = MASK;
            for (int l = N + k - my_clues[i] + 1; l < N; l++) m ^= 1 << l; // Устанавливаем биты в 0 для недопустимых значений
            possible[j] &= m; // Применяем маску к возможным значениям
        }
    }

    // Проверка уникальности и удаление значений, которые уже решены
    while (check_unique() > 0);

    vector<pair<int, int>> idx_npos;
    for (int i = 0; i < N * N; i++) {
        int n_possible = count_possible(possible[i]); // Подсчет количества возможных значений в клетке
        if (n_possible > 1) {
            idx_npos.push_back(make_pair(n_possible, i)); // Добавляем клетку в вектор с количеством возможных значений
        }
    }

    // Сортируем клетки по количеству возможных значений
    sort(idx_npos.begin(), idx_npos.end());
    order.clear();
    for (int i = 0; i < idx_npos.size(); i++) {
        order.push_back(idx_npos[i].second); // Сортируем клетки в порядке возрастания количества возможных значений
    }
    dfs(0); // Запускаем поиск решения

    vector<vector<int>> r;
    for (int i = 0; i < N; i++) {
        vector<int> vec;
        for (int j = 0; j < N; j++) vec.push_back(results[i][j]); // Собираем результат в двумерный вектор
        r.push_back(vec);
    }
    return r; // Возвращаем решение пазла
}

int main()
{
    N=6;
    vector<int> clues = {
            0, 0, 0, 0, 0, 0,  // Left
            0, 0, 0, 0, 0, 0,  // Right
            0, 0, 0, 0, 0, 0,  // Top
            0, 0, 0, 0, 0, 0  // Bottom
    };
//    while (true)
//    {
//        cout << "enter size: ";
//        cin >> N;
//        if (N >= 4 && N <= 6) break;
//    }
//    for (int i = 1; i <= N * SIDES; i++)
//    {
//        cout << i << ") ";
//        int x;
//        cin >> x;
//        clues.push_back(x);
//    }


    vector<vector<int>> solvedPuzzle = SolvePuzzle(clues);
    for (int i = 0; i < solvedPuzzle.size(); i++)
    {
        for (int j = 0; j < solvedPuzzle.at(i).size(); j++)
        {
            int x = solvedPuzzle.at(i).at(j);
            if (x == 0)
            {
                cout << "No solution\n";
                return 0;
            }
            cout << x << " ";
        }
        cout << endl;
    }

    return 0;
}