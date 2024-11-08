#include <bits/stdc++.h>
#include <cmath>
#include <vector>
using namespace std;

void combinations(vector<int>& elements, int k, vector<vector<int>>& result, vector<int>& current, int start) {
    if (k == 0) {
        result.push_back(current);
        return;
    }
    for (int i = start; i <= elements.size() - k; i++) {
        current.push_back(elements[i]);
        combinations(elements, k - 1, result, current, i + 1);
        current.pop_back();
    }
}

const double TOLERANCE = 0.001;
const int MAX_ITERATIONS = 10000;

bool isDiagonallyDominant(const vector<vector<double>>& A) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        double diag = fabs(A[i][i]);
        double sum = 0.0;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                sum += fabs(A[i][j]);
            }
        }
        if (diag < sum) {
            return false;
        }
    }
    return true;
}

bool makeDominant(vector<vector<double>>& mat, vector<double>& c) {
    int n = mat.size();
    vector<int> rows(n);
    iota(rows.begin(), rows.end(), 0);
    do {
        vector<vector<double>> tempmat(n, vector<double>(n));
        vector<double> b(n);

        for (int i = 0; i < n; i++) {
            tempmat[i] = mat[rows[i]];
            b[i] = c[rows[i]];
        }
        if (isDiagonallyDominant(tempmat)) {
            mat = tempmat;
            c = b;
            return true;
        }
    } while (next_permutation(rows.begin(), rows.end()));
    return false;
}

void gaussSeidel(const vector<vector<double>>& A, const vector<double>& b, vector<double>& x) {
    int n = A.size();
    vector<double> x_old(n);

    for (int k = 0; k < MAX_ITERATIONS; ++k) {
        x_old = x;

        for (int i = 0; i < n; ++i) {
            double sum = 0.0;

            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    sum += A[i][j] * x[j];
                }
            }

            x[i] = (b[i] - sum) / A[i][i];
        }

        double max_diff = 0.0;
        for (int i = 0; i < n; ++i) {
            max_diff = max(max_diff, fabs(x[i] - x_old[i]));
        }

        if (max_diff < TOLERANCE) {
            return;
        }
    }
}

int main() {
    int n, m;
    vector<vector<double>> final;
    cout << "Enter value of n and m: ";
    cin >> n >> m;
    int p;
    cout << "Type 1 if you want to maximize z, else type 0: ";
    cin >> p;

    vector<double> coeff(n, 0);
    cout << "enter the coefficient of the expression you have to maximize or minimize" << endl;
    for (int i = 0; i < n; i++) cin >> coeff[i];
    if (p == 0) {
        for (int i = 0; i < n; i++) coeff[i] = -1.0 * coeff[i];
    }

    vector<vector<double>> A(m, vector<double>(n, 0));
    vector<double> b(m);
    cout << "Enter the augmented matrix (coefficients):" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> A[i][j];
        }
    }

    cout << "Enter the values of vector b: " << endl;
    for (int i = 0; i < m; i++) cin >> b[i];
    vector<double> a = b;

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    vector<vector<int>> combo;
    vector<int> curr;

    combinations(indices, m, combo, curr, 0);
    double maxi = -1e9;

    for (auto c : combo) {
        vector<vector<double>> AA(m, vector<double>(m));
        b = a;
        vector<double> x(m, 0.0);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                AA[i][j] = A[i][c[j]];
            }
        }

        if (makeDominant(AA, b)) {
            gaussSeidel(AA, b, x);
            vector<double> ans(n, 0.0);

            cout << "Solution:" << endl;
            int cnt = 0;
            for (int i = 0; i < m; ++i) {
                cout << "x" << c[i] + 1 << " = " << x[i] << endl;
                ans[c[i]] = x[i];
            }
            cout << "Other variables are taken as 0" << endl;

            bool flag = 0;
            for (int i = 0; i < m; i++) {
                if (x[i] < 0)
                    flag = 1;
                if (x[i] == 0.0)
                    cnt++;
            }
            if ((cnt + n - m) == m) {
                if (flag == 1)
                    cout << "This is a nondegenerate basic solution but not feasible." << endl;
                else {
                    cout << "This is a nondegenerate basic solution and feasible." << endl;
                    final.push_back(ans);
                }
            } else if (cnt) {
                if (flag == 1)
                    cout << "This is a degenerate basic solution but not feasible." << endl;
                else {
                    cout << "This is a degenerate basic solution and feasible." << endl;
                    final.push_back(ans);
                }
            } else {
                if (flag == 1)
                    cout << "This is a basic solution but not feasible." << endl;
                else {
                    cout << "This is a basic solution and feasible." << endl;
                    final.push_back(ans);
                }
            }

        } else {
            cout << "The matrix can't be converted to diagonally dominant." << endl;
        }
    }
    int u = 1;
    vector<double> res(n);
    for (auto ff : final) {
        double tmpmaxi = 0.0;
        for (int i = 0; i < n; i++) {
            tmpmaxi += (coeff[i] * ff[i]);
        }
        if (tmpmaxi > maxi) {
            res = ff;
            maxi = tmpmaxi;
        }
        cout << "objective value of feasible solution " << u << "is : " << tmpmaxi << endl;
        u++;
    }

    cout << "Optimal value is " << maxi << endl;
    cout << "Optimal solution is " << endl;
    for (int i = 0; i < n; i++) {
        cout << res[i] << " ";
    }
    cout << endl;

    return 0;
}
