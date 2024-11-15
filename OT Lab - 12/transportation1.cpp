#include <bits/stdc++.h>
using namespace std;

int n, m;

struct node {
    int val;   // Allocated value in the cell
    int cost;  // Cost associated with the cell
    int d;     // Opportunity cost (reduced cost)
    node() {
        val = 0;
        cost = 0;
        d = 0;
    }
};

vector<vector<node>> table;
vector<int> req;    // Demand requirements
vector<int> avail;  // Supply availability
vector<int> u;
vector<int> v;

// Vogel Approximation Method (VAM) for initial basic feasible solution
void implement_vogel() {
    vector<int> temp_avail = avail;
    vector<int> temp_req = req;

    while (true) {
        vector<pair<int, int>> row_penalty(n, {-1, INT_MAX});
        vector<pair<int, int>> col_penalty(m, {-1, INT_MAX});

        for (int i = 0; i < n; i++) {
            if (temp_avail[i] > 0) {
                int first_min = INT_MAX, second_min = INT_MAX, min_index = -1;
                for (int j = 0; j < m; j++) {
                    if (temp_req[j] > 0) {
                        if (table[i][j].cost < first_min) {
                            second_min = first_min;
                            first_min = table[i][j].cost;
                            min_index = j;
                        } else if (table[i][j].cost < second_min) {
                            second_min = table[i][j].cost;
                        }
                    }
                }
                row_penalty[i] = {second_min - first_min, min_index};
            }
        }

        for (int j = 0; j < m; j++) {
            if (temp_req[j] > 0) {
                int first_min = INT_MAX, second_min = INT_MAX, min_index = -1;
                for (int i = 0; i < n; i++) {
                    if (temp_avail[i] > 0) {
                        if (table[i][j].cost < first_min) {
                            second_min = first_min;
                            first_min = table[i][j].cost;
                            min_index = i;
                        } else if (table[i][j].cost < second_min) {
                            second_min = table[i][j].cost;
                        }
                    }
                }
                col_penalty[j] = {second_min - first_min, min_index};
            }
        }

        int max_penalty = -1, row_or_col = -1, index = -1;
        bool is_row = false;

        for (int i = 0; i < n; i++) {
            if (temp_avail[i] > 0 && row_penalty[i].first > max_penalty) {
                max_penalty = row_penalty[i].first;
                index = i;
                row_or_col = row_penalty[i].second;
                is_row = true;
            }
        }

        for (int j = 0; j < m; j++) {
            if (temp_req[j] > 0 && col_penalty[j].first > max_penalty) {
                max_penalty = col_penalty[j].first;
                index = j;
                row_or_col = col_penalty[j].second;
                is_row = false;
            }
        }

        if (max_penalty == -1)
            break;

        if (is_row) {
            int i = index;
            int j = row_or_col;
            int allocation = min(temp_avail[i], temp_req[j]);
            table[i][j].val = allocation;
            temp_avail[i] -= allocation;
            temp_req[j] -= allocation;
        } else {
            int j = index;
            int i = row_or_col;
            int allocation = min(temp_avail[i], temp_req[j]);
            table[i][j].val = allocation;
            temp_avail[i] -= allocation;
            temp_req[j] -= allocation;
        }
    }
}

// Check optimality using u-v method (dual variables)
pair<int, int> check_optimal() {
    u.clear();
    v.clear();
    u.assign(n, INT_MAX);
    v.assign(m, INT_MAX);
    u[0] = 0;

    bool updated;
    for (int k = 0; k < n + m - 1; k++) {
        updated = false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (table[i][j].val != 0) {
                    if (u[i] != INT_MAX && v[j] == INT_MAX) {
                        v[j] = table[i][j].cost - u[i];
                        updated = true;
                    } else if (v[j] != INT_MAX && u[i] == INT_MAX) {
                        u[i] = table[i][j].cost - v[j];
                        updated = true;
                    }
                }
            }
        }
        if (!updated)
            break;
    }

    bool optimal = true;
    pair<int, int> min_ind = {-1, -1};
    int min_val = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (table[i][j].val == 0) {
                table[i][j].d = table[i][j].cost - (u[i] + v[j]);
                if (table[i][j].d < min_val) {
                    min_val = table[i][j].d;
                    min_ind = {i, j};
                    optimal = false;
                }
            }
        }
    }

    if (optimal) {
        return {-1, -1};
    }
    return min_ind;
}

// Function to determine direction and perform allocations
pair<int, int> check_dir(pair<int, int> q) {
    if (table[q.first][m - 1].val != 0 &&
        table[n - 1][q.second].val != 0 &&
        table[n - 1][m - 1].val != 0) {
        return {4, min({table[q.first][m - 1].val,
                        table[n - 1][q.second].val,
                        table[n - 1][m - 1].val})};
    }

    if (table[q.first][0].val != 0 &&
        table[0][q.second].val != 0 &&
        table[0][0].val != 0) {
        return {1, min({table[q.first][0].val,
                        table[0][q.second].val,
                        table[0][0].val})};
    }

    if (table[0][q.second].val != 0 &&
        table[q.first][m - 1].val != 0 &&
        table[0][m - 1].val != 0) {
        return {2, min({table[0][q.second].val,
                        table[q.first][m - 1].val,
                        table[0][m - 1].val})};
    }

    if (table[q.first][0].val != 0 &&
        table[n - 1][q.second].val != 0 &&
        table[n - 1][0].val != 0) {
        return {3, min({table[q.first][0].val,
                        table[n - 1][q.second].val,
                        table[n - 1][0].val})};
    }

    return {-1, -1};
}

// Adjust allocations based on direction
void adjust_allocations(pair<int, int> entering_cell) {
    auto direction_info = check_dir(entering_cell);
    if (direction_info.first == -1) {
        cout << "No valid direction for adjustment.\n";
        return;
    }

    int adjustment_value = direction_info.second;
    switch (direction_info.first) {
    case 1:
        table[entering_cell.first][entering_cell.second].val += adjustment_value;
        table[entering_cell.first][0].val -= adjustment_value;
        table[0][entering_cell.second].val -= adjustment_value;
        table[0][0].val += adjustment_value;
        break;
    case 2:
        table[entering_cell.first][entering_cell.second].val += adjustment_value;
        table[0][entering_cell.second].val -= adjustment_value;
        table[entering_cell.first][m - 1].val -= adjustment_value;
        table[0][m - 1].val += adjustment_value;
        break;
    case 3:
        table[entering_cell.first][entering_cell.second].val += adjustment_value;
        table[entering_cell.first][0].val -= adjustment_value;
        table[n - 1][entering_cell.second].val -= adjustment_value;
        table[n - 1][0].val += adjustment_value;
        break;
    case 4:
        table[entering_cell.first][entering_cell.second].val += adjustment_value;
        table[entering_cell.first][m - 1].val -= adjustment_value;
        table[n - 1][entering_cell.second].val -= adjustment_value;
        table[n - 1][m - 1].val += adjustment_value;
        break;
    }
}

void print_final_cost() {
    int total_cost = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (table[i][j].val > 0) {
                total_cost += table[i][j].val * table[i][j].cost;
            }
        }
    }
    cout << "The total transportation cost is: " << total_cost << endl;
}

int main() {
    cout << "Enter the number of sources: ";
    cin >> n;
    cout << "Enter the number of destinations: ";
    cin >> m;
    table.resize(n, vector<node>(m));

    cout << "Enter the costs matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> table[i][j].cost;
        }
    }

    cout << "Enter the availability:\n";
    avail.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> avail[i];
    }

    cout << "Enter the requirements:\n";
    req.resize(m);
    for (int i = 0; i < m; i++) {
        cin >> req[i];
    }

    implement_vogel();
    cout << "The initial basic feasible solution (IBFS) is:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << table[i][j].val << " ";
        }
        cout << endl;
    }

    pair<int, int> optimal_check;
    for (int k = 0; k < 100; k++) {
        optimal_check = check_optimal();
        if (optimal_check == make_pair(-1, -1)) {
            cout << "Optimal solution reached." << endl;
            break;
        }

        adjust_allocations(optimal_check);

        cout << "Iteration " << k + 1 << ":\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << table[i][j].val << " ";
            }
            cout << endl;
        }
    }

    print_final_cost();

    return 0;
}
