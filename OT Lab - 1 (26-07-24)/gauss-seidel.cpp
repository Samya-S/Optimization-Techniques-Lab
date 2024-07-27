#include <bits/stdc++.h>
using namespace std;

bool isDiagonallyDominantMatrix(vector<vector<double>> a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                sum += abs(a[i][j]);
            }
        }
        if (sum > abs(a[i][i])) {
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    // cout << "Enter the number of equations: ";
    cin >> n;
    // cout << endl;

    vector<vector<double>> a(n, vector<double>(n, 0));
    vector<double> b(n, 0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // cout << "a[" << i << ", " << j << "] = ";
            cin >> a[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        // cout << "b[" << i << "] = ";
        cin >> b[i];
        // cout << endl;
    }

    if (!isDiagonallyDominantMatrix(a)) {
        cout << "The matrix is not diagonally dominant." << endl;
        cout << "The Gauss-Seidel method may not converge." << endl;
        return 0;
    }

    vector<double> x(n, 0), y(n, 0);

    // cout << "Enter initial values of x" << endl;
    for (int i = 0; i < n; i++) {
        // cout << "x:[" << i << "] = ";
        cin >> x[i];
    }

    // cout << endl << "Enter the no. of iteration(s) : ";
    int iter;
    cin >> iter;
    // cout << endl;

    // cout << endl << "Enter the allowed error: ";
    int delta;
    cin >> delta;
    // cout << endl;

    // int count = 0;
    double maxError = 0;
    do {
        for (int i = 0; i < n; i++) {
            // y[i] = (b[i] / a[i][i]);
            y[i] = b[i];
            for (int j = 0; j < n; j++) {
            //     if (j == i)
            //         continue;
            //     y[i] = y[i] - ((a[i][j] / a[i][i]) * x[j]);
            //     // x[i] = y[i];
            // }

                if (i != j) {
                    y[i] -= a[i][j] * x[j];
                }
            }
            y[i] /= a[i][i];
        }

        maxError = 0;
        for (int i = 0; i < n; i++) {
            maxError = max(maxError, abs(x[i] - y[i]));
            x[i] = y[i];
            // cout << "x" << i + 1 << " = " << y[i] << " ";
            cout << "x" << i + 1 << " = " << fixed << setprecision(6) << y[i] << " ";
        }
        cout << endl;
        // count++;
    } while (maxError > delta);

    cout << "The final solution is:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = " << x[i] << endl;
    }

    return 0;
}
