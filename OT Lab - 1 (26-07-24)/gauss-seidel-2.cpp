#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

void gaussSeidel(const vector<vector<double>>& a, const vector<double>& b, vector<double>& x, int n, int maxIterations, double tolerance) {
    for (int iter = 0; iter < maxIterations; ++iter) {
        vector<double> x_old = x;

        for (int i = 0; i < n; ++i) {
            double sum = b[i];
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    sum -= a[i][j] * x[j];
                }
            }
            x[i] = sum / a[i][i];
        }

        // Check for convergence
        double error = 0.0;
        for (int i = 0; i < n; ++i) {
            error += fabs(x[i] - x_old[i]);
        }
        if (error < tolerance) {
            cout << "Converged after " << iter + 1 << " iterations." << endl;
            return;
        }
    }
    cout << "Max iterations reached without convergence." << endl;
}

int main() {
    int n;
    // cout << "Enter the number of equations: ";
    cin >> n;

    vector<vector<double>> a(n, vector<double>(n));
    vector<double> b(n), x(n, 0.0);

    // cout << "Enter the coefficients matrix (aij):" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> a[i][j];
        }
    }

    // cout << "Enter the constants vector (bi):" << endl;
    for (int i = 0; i < n; ++i) {
        cin >> b[i];
    }

    int maxIterations;
    double tolerance;
    // cout << "Enter the maximum number of iterations: ";
    cin >> maxIterations;
    // cout << "Enter the tolerance: ";
    cin >> tolerance;

    gaussSeidel(a, b, x, n, maxIterations, tolerance);

    cout << "Solution:" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "x" << i + 1 << " = " << x[i] << endl;
    }

    return 0;
}