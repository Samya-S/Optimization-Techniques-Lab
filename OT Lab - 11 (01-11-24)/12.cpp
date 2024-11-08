#include <bits/stdc++.h>
using namespace std;

vector<vector<double>> A, all_sols, Astar, BstarInverse, table;
vector<double> B;
vector<double> B_min_ratio;
vector<double> Z;
vector<double> delta;
vector<bool> unrestricted, check_basic;
vector<int> equalto, vartype;
vector<double> func_coeff;
vector<int> basis;
vector<int> slack_vars, artificial_vars, surplus_vars;
double numeqs, numvars, num_unrestricted = 0, slack = 0, surplus = 0, artificial = 0, M = -1e16, opti_val = 0;
bool solution_possible = true, degeneracy = false, unbounded = false, maximise, alternate_present = false, to_terminate = false, infeasible = false;
int alt_entering_col = -1;

bool check_is_basic(int col) {
    for (auto e : basis)
        if (e == col)
            return true;
    return false;
}

void print_table(int entering_col) {
    cout << "(B*)" << '\t';

    for (int i = 0; i < table[0].size() - 2; i++) {
        cout << "B" << i + 1 << "*" << '\t';
    }

    cout << "Xb*" << '\t' << "Y" << entering_col << '\t' << "min-ratio" << '\n';

    for (int i = 0; i < table.size(); i++) {
        cout << "a" << basis[i] << "*" << '\t';

        for (int j = 0; j < table[0].size(); j++) {
            cout << table[i][j] << '\t';
        }

        if (i == 0)
            cout << "-";
        else if (table[i][table[0].size() - 1] > 0)
            cout << table[i][table[0].size() - 2] / table[i][table[0].size() - 1];
        else
            cout << "-";

        cout << '\n';
    }
}

int enteringVariable() {
    // print bstarinverse
    cout << "BstarInverse : \n";
    for (int i = 0; i < BstarInverse.size(); i++) {
        for (int j = 0; j < BstarInverse[0].size(); j++) {
            cout << BstarInverse[i][j] << ' ';
        }
        cout << '\n';
    }
    // Calculate zj - cjs
    double minZj_Cj = 0;
    int eVar = -1;
    cout << "Entering variable calculation : \n";
    for (int i = 1; i < Astar[0].size(); i++) {
        if (!check_is_basic(i)) {
            double currZj_Cj = 0;

            for (int k = 0; k < Astar.size(); k++) {
                currZj_Cj += BstarInverse[0][k] * Astar[k][i];
            }
            cout << "Zj - Cj for x" << i << " : " << currZj_Cj << '\n';
            if (currZj_Cj < minZj_Cj) {
                minZj_Cj = currZj_Cj;
                eVar = i;
            }
        }
    }

    return eVar;
}

int leavingVariable() {
    // Calculate min ratio
    double min_ratio = 1e16;
    int lVar = -1;
    for (int i = 1; i < table.size(); i++) {
        if (table[i][table[0].size() - 1] > 0) {
            double ratio = table[i][table[0].size() - 2] / table[i][table[0].size() - 1];
            if (ratio < min_ratio) {
                min_ratio = ratio;
                lVar = i;
            }
        }
    }

    return lVar;
}

void revised_simplex_method() {
    int eVar, iter = 0;

    while ((eVar = enteringVariable()) + 1) {
        // caluclate Y(eVar)
        vector<double> y(Astar.size(), 0);

        for (int j = 0; j < y.size(); j++) {
            for (int i = 0; i < BstarInverse.size(); i++) {
                y[j] += BstarInverse[j][i] * Astar[i][eVar];
            }
        }

        // update table
        for (int j = 0; j < y.size(); j++) {
            table[j][table[0].size() - 1] = y[j];
        }

        // min ratio calculation and leaving variable and PRINT TABLE
        int lVar = leavingVariable();

        cout << "\nTable at iteration " << ++iter << ": \n";
        print_table(eVar);

        if (lVar == -1) {
            cout << "Unbounded solution\n";
            return;
        } else {
            // table update (row operations)

            for (int i = 0; i < table.size(); i++) {
                if (i == lVar) {
                    for (int j = 0; j < table[0].size(); j++) {
                        table[i][j] /= table[lVar][table[0].size() - 1];
                    }
                } else {
                    double factor = table[i][table[0].size() - 1] / table[lVar][table[0].size() - 1];

                    for (int j = 0; j < table[0].size(); j++) {
                        table[i][j] -= factor * table[lVar][j];
                    }
                }
            }
        }

        // Bstar change and basis update
        for (int i = 0; i < table.size(); i++) {
            for (int j = 0; j < table[0].size() - 2; j++) {
                BstarInverse[i][j + 1] = table[i][j];
            }
        }

        check_basic[basis[lVar]] = false;
        basis[lVar] = eVar;
        check_basic[eVar] = true;
    }

    cout << "\nOptimal solution found\n";

    double opti_val = 0;
    for (int i = 1; i < table.size(); i++) {
        opti_val += table[i][table[0].size() - 2] * func_coeff[basis[i] - 1];
    }

    cout << "Optimal value : " << opti_val << '\n';
    cout << "\nOptimal solution : \n";

    for (int i = 0; i < table.size(); i++) {
        if (i)
            cout << "x" << basis[i];
        else
            cout << "Z";

        cout << " = " << table[i][table[0].size() - 2] << '\n';
    }
    return;
}

int main() {
    cout << "Enter the number of equations : ";
    cin >> numeqs;
    cout << "Enter the number of variables : ";
    cin >> numvars;
    unrestricted.resize(numvars);

    cout << "Enter the number of unrestricted variables : ";
    cin >> num_unrestricted;

    if (num_unrestricted)
        cout << "Enter the indices of the unrestricted variables : ";

    for (int i = 0; i < num_unrestricted; i++) {
        double ind;
        cin >> ind;
        unrestricted[ind - 1] = true;
    }

    for (int i = 0; i < numvars; i++) {
        vartype.push_back(0);
        if (unrestricted[i])
            vartype.push_back(-1);
    }

    // -1 for <=, 0 for =, 1 for >=
    cout << "Enter the inequality of the equations (-1/0/1) : ";
    double varno = 0;
    int exactly_equalto = 0;
    for (int i = 0; i < numeqs; i++) {
        double eq;
        cin >> eq;

        if (eq == -1) {
            slack++;
        } else if (eq == 1) {
            surplus++;
            artificial++;
        } else if (eq == 0) {
            exactly_equalto++;
            artificial += 2;
            equalto.push_back(eq);
        }
        equalto.push_back(eq);
    }

    int og_eqs = numeqs;
    numeqs += exactly_equalto;
    A.resize(numeqs);
    double extravars = 0;
    int eq_counter = 0;

    for (int i = 0; i < numeqs; i++) {
        cout << "Enter the coefficients of equation " << ++eq_counter << " :\n";
        int j;
        for (j = 0; j < numvars; j++) {
            double coeff;
            cin >> coeff;
            A[i].push_back(coeff);
            if (unrestricted[j]) {
                A[i].push_back(-1 * coeff);
            }
        }

        for (int k = 0; k < extravars; k++) A[i].push_back(0);

        double constant;
        cin >> constant;
        B.push_back(constant);

        bool eq_encountered = false;
        if (equalto[i] == 0) {
            B.push_back(-1 * constant);
            eq_encountered = true;
            slack += 2;
            artificial -= 2;
            equalto[i] = -1;
            equalto[i + 1] = -1;
            vector<double> temp_eq = A[i];
            for (auto &ele : temp_eq) ele *= -1;
            A[i + 1] = temp_eq;
        }

        if (equalto[i] == -1) {
            extravars++;
            vartype.push_back(1);
            basis.push_back(vartype.size() - 1);
            A[i].push_back(1);
        } else if (equalto[i] == 1) {
            // change sign from >= to <=
            equalto[i] = -1;
            artificial--;
            surplus--;
            for (auto &ele : A[i]) ele *= -1;
            extravars++;
            vartype.push_back(1);
            basis.push_back(vartype.size() - 1);
            A[i].push_back(1);
            B[i] *= -1;
        }

        if (eq_encountered) {
            i++;
            if (equalto[i] == -1) {
                extravars++;
                vartype.push_back(1);
                A[i].push_back(0);
                basis.push_back(vartype.size() - 1);
                A[i].push_back(1);
            }
        }
    }

    for (int i = 0; i < numeqs; i++) {
        for (int k = A[i].size(); k < (numvars + extravars + num_unrestricted); k++) {
            A[i].push_back(0);
        }
    }

    int inp;
    cout << "Is this a maximisation problem? (0/1) : ";
    cin >> inp;

    cout << "Enter the coefficients of the function to be optimized : ";
    if (inp == 0)
        maximise = false;
    else
        maximise = true;

    for (int i = 0; i < numvars; i++) {
        double coeff;
        cin >> coeff;
        func_coeff.push_back(coeff * (maximise == true ? 1 : -1));
        if (unrestricted[i])
            func_coeff.push_back(coeff * (maximise == true ? -1 : 1));
    }

    //  3 5 2 1 2 -1 1 0 3 5 2 -3 -4 60 4 4 6 2 -1 72 5 4 2 0 0 100 1 5 10 8 3 4

    for (int i = 0; i < vartype.size(); i++) {
        int e = vartype[i];
        if (e) {
            // vartype 1 for slack
            if (e == 1)
                slack_vars.push_back(i);
            // vartype 2 for surplus
            else if (e == 2)
                surplus_vars.push_back(i);
            // vartype 3 for artificial
            else
                artificial_vars.push_back(i);
        }
    }

    check_basic.assign(A[0].size(), false);
    for (auto s : slack_vars) {
        check_basic[s] = true;
    }
    for (auto s : artificial_vars) {
        check_basic[s] = true;
    }

    for (int i = func_coeff.size(); i < A[0].size(); i++) {
        if (vartype[i] == 1 || vartype[i] == 2) {
            func_coeff.push_back(0);
        } else if (vartype[i] == 3) {
            func_coeff.push_back(0);
        }
    }

    Astar.resize(A.size() + 1);
    Astar[0].push_back(1);
    for (int i = 0; i < A[0].size(); i++) {
        Astar[0].push_back(func_coeff[i] * -1);
    }

    for (int i = 1; i < A.size() + 1; i++) {
        Astar[i].push_back(0);
        for (int j = 0; j < A[0].size(); j++) {
            Astar[i].push_back(A[i - 1][j]);
        }
    }

    // for (int i = 0; i<A.size()+1; i++){
    //     for (int j = 0; j<A[0].size()+1; j++){
    //         cout << Astar[i][j] << ' ';
    //     }
    //     cout << '\n';
    // }

    vector<int> basis_temp;
    basis_temp.push_back(0);
    for (int i = 0; i < numeqs; i++) basis_temp.push_back(basis[i] + 1);
    basis = basis_temp;

    BstarInverse.resize(Astar.size(), vector<double>(Astar.size()));

    for (int i = 0; i < basis.size(); i++) {
        for (int ro = 0; ro < Astar.size(); ro++) {
            BstarInverse[ro][i] = Astar[ro][basis[i]];
        }
    }

    // create table
    table.assign(numeqs + 1, vector<double>(numeqs + 2, 0));

    for (int i = 1; i < table.size(); i++) {
        for (int j = 0; j < table[0].size() - 2; j++) {
            if (i - 1 == j) {
                table[i][j] = 1;
            } else {
                table[i][j] = 0;
            }
        }
        table[i][table[0].size() - 2] = B[i - 1];
    }

    // for (int i = 0; i<table.size(); i++){
    //     for (int j = 0; j<table[0].size(); j++){
    //         cout << table[i][j] << ' ';
    //     }
    //     cout << '\n';
    // }

    // cout << '\n';
    // for (int i = 0; i<BstarInverse.size(); i++){
    //     for (int j =  0; j<BstarInverse[0].size(); j++){
    //         cout << BstarInverse[i][j] << ' ';
    //     }
    //     cout << '\n';
    // }

    // cout << '\n';
    // for (auto e : basis) cout << e << ' ';

    revised_simplex_method();

    return 0;
}

// 3 2 0 0 1 -1 3 1 3 4 3 6 1 2 4 1 -2 -1