#include <bits/stdc++.h>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

class Simplex {
private:
    int n, m, artificialCount;
    vector<vector<double>> A;
    vector<double> c;
    vector<double> b;
    vector<double> CB;
    vector<int> B;
    vector<double> delta;
    const double M = 1e2;  // Large penalty value
     int extravar = 0;
    set<int> arti_ind;

public:
    Simplex(int variables, int constraints) : n(variables), m(constraints),
                                              A(constraints, vector<double>(variables + constraints+constraints)),
                                              c(variables + constraints + constraints), // c includes slack and artificial variables
                                              b(constraints),
                                              CB(constraints),
                                              B(constraints),
                                              delta(variables + constraints + constraints),
                                              artificialCount(0) {}

    void readInput() {
        cout << "Enter the coefficients of the objective function: ";
        for (int i = 0; i < n; i++) {
            cin >> c[i];
        }

        cout << "Enter the coefficients of the constraints:" << endl;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cin >> A[i][j];
            }
        }

        vector<string> sign(m);
        cout << "Enter >=/<=/=: " << endl;
        for(int i = 0 ;i<m;i++) cin>>sign[i];

        cout << "Enter the right-hand side of the constraints: ";
        for (int i = 0; i < m; i++) {
            cin >> b[i];
            if (b[i] < 0) {
                b[i]*=-1;
                for(int j = 0 ;j<n;j++){
                    A[i][j]*=-1;
                }
                if(sign[i]==">=") sign[i] = "<=";
                else if(sign[i]=="<=") sign[i] = ">=";
            }
        }

        // Set up slack and artificial variables
       
        for (int i = 0; i < m; i++) {
            string constraintType = sign[i];           

            if (constraintType == "<=") {
                // Slack variable
                A[i][n + extravar] = 1.0;   // Slack variable coefficient
                c[n + extravar] = 0.0;      // Slack variable in the objective function
                B[i] = n + extravar;        // Slack variables start as basic variables
                CB[i] = 0.0;         // Coefficient of slack variable in the objective
                extravar++;         
            }
            else if (constraintType == "=") {
                // Artificial variable
                A[i][n + extravar] = 1.0;  // Artificial variable coefficient
                c[n + extravar] = -M;       // Penalty M in objective function
                B[i] = n + extravar;       // Artificial variable starts as basic
                CB[i] = -M;                // Artificial variable in the basis
                artificialCount++;
                arti_ind.insert(n+extravar);
                extravar++;
            }
            else if (constraintType == ">=") {
                // Surplus variable
                A[i][n + extravar] = -1.0;  // Surplus variable coefficient
                c[n + extravar] = 0.0;      // Surplus variable in objective function
                extravar++;
                // Artificial variable
                A[i][n +extravar] = 1.0;  // Artificial variable coefficient
                c[n + extravar] = -M;       // Penalty M in objective function
                B[i] = n + extravar;       // Artificial variable starts as basic
                CB[i] = -M;                 // Artificial variable in the basis
                artificialCount++;
                arti_ind.insert(n+extravar);
                extravar++;
            }
        }
    }

    void computeDelta() {
        for (int j = 0; j < n +extravar; j++) {
            double sum = 0.0;
            for (int i = 0; i < m; i++) {
                sum += CB[i] * A[i][j];
            }
            delta[j] = sum - c[j];
        }
    }

    int findEnteringVariable() {
        int entering = -1;
        for (int j = 0; j < n + extravar; j++) {
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
        double minRatio = numeric_limits<double>::infinity();  // Use infinity for comparison
        for (int i = 0; i < m; i++) {
            if (A[i][entering] > 0) {  // Check if pivot element is positive
                double ratio = b[i] / A[i][entering];
                if (ratio < minRatio || (ratio == minRatio && B[i] < B[leaving])) {  // Handle degeneracy
                    minRatio = ratio;
                    leaving = i;
                }
            }
        }
        if (minRatio == numeric_limits<double>::infinity()) {
            cout << "Unbounded solution: no positive entries in the column of the entering variable." << endl;
            return -1;
        }
        return leaving;
    }

    void pivot(int entering, int leaving) {
        double pivotElement = A[leaving][entering];

        // Normalize pivot row
        for (int j = 0; j < n +extravar; j++) {
            A[leaving][j] /= pivotElement;
        }
        b[leaving] /= pivotElement;

        // Update other rows
        for (int i = 0; i < m; i++) {
            if (i != leaving) {
                double factor = A[i][entering];
                for (int j = 0; j < n +extravar; j++) {
                    A[i][j] -= factor * A[leaving][j];
                }
                b[i] -= factor * b[leaving];
            }
        }

        // Update basis
        B[leaving] = entering;
        CB[leaving] = c[entering];
    }

    void printTable() {
        cout << endl << "Simplex Table:" << endl;
        cout << '	' << "XB" << '	' << "CB";
        for (int j = 0; j < n + extravar; j++) {
            cout << '	' << "X" << j + 1;
        }
        cout << '	' << "b" << endl;

        for (int i = 0; i < m; i++) {
            
            cout << '	' << "X" << B[i] + 1 << '	' ;
            if(CB[i]==-100) cout<<"-M";
            else cout<<CB[i];
            for (int j = 0; j < n + extravar; j++) {
                cout << '	' << A[i][j];
            }
            cout << '	' << b[i] << endl;
        }

        cout << "Δj:		";
        for (int j = 0; j < n +extravar; j++) {
            cout << '	' << delta[j];
        }
        cout << endl;
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

            // Check for degenerate solution
            if (b[leaving] == 0) {
                cout << "Degenerate solution detected." << endl;
            }

            pivot(entering, leaving);
        }

        // Print final solution
        cout << "Final Solution:" << endl;
        for (int i = 0; i < m; i++) {
            if(B[i]<n)
            cout << "X" << B[i] + 1 << " = " << b[i] << endl;
        }
        double optimalValue = 0;
        for (int i = 0; i < m; i++) {
            optimalValue += CB[i] * b[i];
        }
        cout << "Optimal value of the objective function: " << optimalValue << endl;

        // Check for infeasibility
        for (int i = 0; i < m; i++) {
             
            if (arti_ind.find(B[i])!=arti_ind.end() && b[i] != 0) {
                cout << "Infeasible solution: artificial variable X" << B[i] + 1 << " is still in the basis." << endl;
                return;
            }
        }
    }
};

int main() {
    int variables, constraints;
    cout << "Enter the number of variables and constraints: ";
    cin >> variables >> constraints;

    Simplex simplex(variables, constraints);
    simplex.readInput();
    simplex.solve();

    return 0;
}