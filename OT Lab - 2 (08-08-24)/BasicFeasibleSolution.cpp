#include <bits/stdc++.h>
using namespace std;

long fact(long n){
    if(n < 2) return 1;
    return n * fact(n-1);
}

long nCr(int n,int r){
    return fact(n)/(fact(r)*fact(n-r));
}

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

// void reorder(vector<vector<double>> &a, vector<double> &b){
//     int n = a.size();
//
//     vector<vector<double>> a1(n);
//     vector<double> b1(n);
//
//     for(int i=0; i<n; i++){
//         int maxInd = max_element(a[i].begin(), a[i].end()) - a[i].begin();
//         a1[maxInd] = a[i];
//         b1[maxInd] = b[i];
//     }
//
//     a = a1; b = b1;
//
//     // cout << "new equations:" << endl;
//     // for(int i=0; i<n; i++){
//     //     for(int j=0; j<n; j++){
//     //         cout << a[i][j] << "x" << j+1 << " ";
//     //         if(j!=n-1) cout << "+ ";
//     //         else cout << "= ";
//     //     }
//     //     cout << b[i] << endl;
//     // }
//
//     return;
// }

void gaussSeidel(vector<vector<double>> &a, vector<double> &b, vector<int> &perm){
    int n = a.size();

    int varCnt = perm.size();
    vector<int> vals;

    for(int i=0; i<varCnt; i++){
        if(perm[i] == 1) 
            vals.push_back(i+1);
    }

    cout << "Given equations:" << endl;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << a[i][j] << "x" << vals[j] << " ";
            if(j!=n-1) cout << "+ ";
            else cout << "= ";
        }
        cout << b[i] << endl;
    }

    // reorder(a, b);

    if (!isDiagonallyDominantMatrix(a)) {
        cout << "The matrix is not diagonally dominant." << endl;
        cout << "The Gauss-Seidel method may not converge." << endl;
        return;
    }

    vector<double> x(n, 0);

    cout << "Considering initial values: ";
    for (int i = 0; i < n; i++) {
        cout << "x" << vals[i] << "=" << x[i] << " ";
    }
    cout << endl;

    double delta = 0.001; // maximum allowed error

    double error;
    do {
        vector<double> prev, y(n);
        prev = x;
        for (int i = 0; i < n; i++) {
            y[i] = (b[i] / a[i][i]);
            for (int j = 0; j < n; j++) {
                if (j != i)
                    y[i] = y[i] - ((a[i][j] / a[i][i]) * x[j]);
            }
            x[i] = y[i];
            // cout << "x" << vals[i] << " = " << fixed << setprecision(6) << y[i] << " ";
            // cout << "x" << vals[i] << " = " << y[i] << " ";
        }
        // cout << endl;

        error = 0;
        for (int i = 0; i < n; i++) {
            error += pow(prev[i]-x[i], 2);
        }
    } while(sqrtl(error) > delta);

    // cout << endl;

    cout << "The final solution is:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << vals[i] << " = " << x[i] << endl;
        // cout << "x" << vals[i] << " = " << fixed << setprecision(6) << x[i] << endl;
    }
    // cout << endl;

    // vector<double> ans(varCnt, 0);
    // for(int i=0; i<n; i++){
    //     ans[vals[i]-1] = x[i];
    // }  

    bool flag = false;
    int cnt = 0;

    for(auto it:x){
        if(it < 0) flag = true;
        if(it == 0) cnt++;
    }

    if((cnt-n+varCnt) == n) {
        if(flag) cout << "This is a non-degenerate basic solution but not feasible." << endl;
        else cout << "This is a nondegenerate basic solution and feasible." << endl;
    }
    else if(cnt){
        if(flag) cout << "This is a degenerate basic solution but not feasible." << endl;
        else cout << "This is a degenerate basic solution and feasible." << endl; 
    }
    else {
        if(flag) cout << "This is a basic solution but not feasible." << endl;
        else cout << "This is a basic solution and feasible." << endl; 
    }

    return;
}

int main() {    
    // cout << "Enter the number of variables: ";
    int n; cin >> n;
    // cout << endl;

    // cout << "Enter the number of equations: ";
    int m; cin >> m;
    // cout << endl;

    bool isInequality;
    // cout << "type 1 if it is a inequality else type 0: ";
    cin >> isInequality;

    vector<vector<double>> a(m, vector<double>(n, 0));
    vector<double> b(m, 0);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            // cout << "a[" << i << ", " << j << "] = ";
            cin >> a[i][j];
        }
    }

    for (int i = 0; i < m; i++) {
        // cout << "b[" << i << "] = ";
        cin >> b[i];
    }
    // cout << endl;

    vector<int> perm(n,0);
    for(int i=0; i<m; i++) perm[i] = 1;
    sort(perm.begin(), perm.end());

    do{
        for(int i=0; i<n; i++){
            cout << "x" << i+1 << "=";
            if(perm[i]) cout << "? ";
            else cout << "0 ";
        }
        cout << endl;

        vector<vector<double>> a1(m);

        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                if(perm[j]==1) 
                    a1[i].push_back(a[i][j]);
            }
        }
        
        gaussSeidel(a1, b, perm);        

        cout << endl;
    } while(next_permutation(perm.begin(), perm.end()));

    return 0;
}