#include <bits/stdc++.h>
using namespace std;

class Simplex {
private:
    int n, m;
    vector<vector<double>> A;
    vector<double> c;
    vector<double> b;
    vector<double> CB;
    vector<double> B;
    vector<double> delta;

public:
    Simplex(int variables, int constraints) : n(variables), m(constraints), A(constraints, vector<double>(variables + constraints)), c(variables + constraints), b(constraints), CB(constraints), B(constraints), delta(variables + constraints) {}

    void readInput() {
        // cout << "Enter the coefficients of the objective function: ";
        for (int i = 0; i < n; i++) {
            cin >> c[i];
        }

        // cout << "Enter the coefficients of the constraints:" << endl;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cin >> A[i][j];
            }
        }

        // cout << "Enter the right-hand side of the constraints: ";
        for (int i = 0; i < m; i++) {
            cin >> b[i];
            if (b[i] < 0) {
                cout << "Infeasible Solution: RHS is negative" << endl;
                exit(0);
            }
        }

        for (int i = 0; i < m; i++) {
            A[i][n + i] = 1.0;
            c[n + i] = 0.0;
            B[i] = n + i;
            CB[i] = 0.0;
        }
    }

    void computeDelta() {
        for (int j = 0; j < n + m; j++) {
            double sum = 0.0;
            for (int i = 0; i < m; i++) {
                sum += CB[i] * A[i][j];
            }
            delta[j] = sum - c[j];
        }
    }

    int findEnteringVariable() {
        int entering = -1;
        for (int j = 0; j < n + m; j++) {
            if (delta[j] < 0) {
                if (entering == -1 || delta[j] < delta[entering]) {
                    entering = j;
                }
            }
        }
        return entering;
    }

    int findLeavingVariable(int entering) {
        int leaving = -1;
        double minRatio = 1e18;
        for (int i = 0; i < m; i++) {
            if (A[i][entering] > 0) {
                double ratio = b[i] / A[i][entering];
                if (ratio < minRatio || (ratio == minRatio && B[i] < B[leaving])) {
                    minRatio = ratio;
                    leaving = i;
                }
            }
        }
        if (minRatio == 1e18) {
            cout << "Unbounded Solution: No positive entries in the column of the entering variable" << endl;
            return -1;
        }
        return leaving;
    }

    void pivot(int entering, int leaving) {
        double pivotElement = A[leaving][entering];

        for (int j = 0; j < n + m; j++) {
            A[leaving][j] /= pivotElement;
        }
        b[leaving] /= pivotElement;

        for (int i = 0; i < m; i++) {
            if (i != leaving) {
                double factor = A[i][entering];
                for (int j = 0; j < n + m; j++) {
                    A[i][j] -= factor * A[leaving][j];
                }
                b[i] -= factor * b[leaving];
            }
        }

        B[leaving] = entering;
        CB[leaving] = c[entering];
    }

    void printTable() {
        cout << "Simplex Table:" << endl;
        cout << "\tXB" << "\tCB";
        for (int j = 0; j < n + m; j++) {
            cout << "\tX" << j + 1;
        }
        cout << "\tb" << endl;

        for (int i = 0; i < m; i++) {
            cout << "\tX" << B[i] + 1 << "\t" << CB[i];
            for (int j = 0; j < n + m; j++) {
                cout << "\t" << A[i][j];
            }
            cout << "\t" << b[i] << endl;
        }

        cout << "Δj:\t\t";
        for (int j = 0; j < n + m; j++) {
            cout << "\t" << delta[j];
        }
        cout << endl
             << endl;
    }

    void solve() {
        while (true) {
            computeDelta();
            printTable();

            int entering = findEnteringVariable();
            if (entering == -1) {
                cout << "Optimal solution found." << endl;
                break;
            }

            int leaving = findLeavingVariable(entering);
            if (leaving == -1) {
                cout << "Problem is unbounded." << endl;
                return;
            }

            if (b[leaving] == 0) {
                cout << "Degerate Solution" << endl;
            }

            pivot(entering, leaving);
        }

        cout << "Final Solution:" << endl;
        for (int i = 0; i < m; i++) {
            cout << "X" << B[i] + 1 << " = " << b[i] << endl;
        }
        double optimalValue = 0;
        for (int i = 0; i < m; i++) {
            optimalValue += CB[i] * b[i];
        }
        cout << "Optimal value of the objective function: " << optimalValue << endl;
    }
};

int main() {
    int n, m;
    // cout << "Enter the number of variables: ";
    cin >> n;
    // cout << "Enter the number of constraints: ";
    cin >> m;

    Simplex simplex(n, m);
    simplex.readInput();
    simplex.solve();

    return 0;
}