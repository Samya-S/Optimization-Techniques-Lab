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

bool makeDominant(vector<vector<double>> &A,vector<double> &c){
    int n = A.size();

    vector<int> rows(n);
    iota(rows.begin(),rows.end(),0);

    do{
        vector<vector<double>> temp(n,vector<double>(n));
        vector<double> b(n);

        for(int i = 0 ;i<n;i++){
            temp[i] = A[rows[i]];
            b[i] = c[rows[i]];
        }
        if(isDiagonallyDominantMatrix(A)){
            A = temp;
            c = b;
            return true;
        }
    } while(next_permutation(rows.begin(),rows.end()));

    return false;
}

void solve(const vector<vector<double>>& A, const vector<double>& b, vector<double>& x) {
    int n = A.size();
    vector<double> x_old(n);

    for (int k = 0; k < 100000; ++k) {
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

        if (max_diff < 0.001) {
            return;
        }
    }
}

void gaussSeidel(vector<vector<double>> &a, vector<double> &b, vector<int> &perm, vector<vector<double>> &final){
    int m = a.size();

    int n = perm.size();
    vector<int> vals;

    for(int i=0; i<n; i++){
        if(perm[i] == 1) 
            vals.push_back(i+1);
    }

    cout << "Given equations:" << endl;
    for(int i=0; i<m; i++){
        for(int j=0; j<m; j++){
            cout << a[i][j] << "x" << vals[j] << " ";
            if(j!=m-1) cout << "+ ";
            else cout << "= ";
        }
        cout << b[i] << endl;
    }

    if (!makeDominant(a, b)) {
        cout << "The matrix is not diagonally dominant." << endl;
        cout << "The Gauss-Seidel method may not converge." << endl;
        return;
    }

    vector<double> x(m, 0);

    cout << "Considering initial values: ";
    for (int i=0; i<m; i++) {
        cout << "x" << vals[i] << "=" << x[i] << " ";
    }
    cout << endl;

    // double delta = 0.001; // maximum allowed error
    // int iterations = 10000;
    //
    // double error;
    // do {
    //     vector<double> prev, y(m);
    //     prev = x;
    //     for (int i=0; i<m; i++) {
    //         double sum = 0;
    //         // y[i] = (b[i] / a[i][i]);
    //         for (int j=0; j<m; j++) {
    //             if (j != i)
    //                 // y[i] = y[i] - ((a[i][j] / a[i][i]) * x[j]);
    //                 sum += a[i][j]*x[j];
    //         }
    //         // x[i] = y[i];
    //         x[i] = (b[i] - sum) / a[i][i];
    //         // cout << "x" << vals[i] << " = " << fixed << setprecision(6) << y[i] << " ";
    //         // cout << "x" << vals[i] << " = " << y[i] << " ";
    //     }
    //     // cout << endl;
    //
    //     error = 0;
    //     for (int i=0; i<m; i++) {
    //         error += pow(prev[i]-x[i], 2);
    //     }
    // } while(sqrtl(error) > delta && iterations--);
    // // cout << endl;
    solve(a, b, x);

    cout << "The final solution is:" << endl;
    for (int i=0; i<m; i++) {
        cout << "x" << vals[i] << " = " << x[i] << endl;
        // cout << "x" << vals[i] << " = " << fixed << setprecision(6) << x[i] << endl;
    }
    // cout << endl;

    vector<double> ans(n, 0);
    for(int i=0; i<m; i++){
        ans[vals[i]-1] = x[i];
    }

    bool flag = false;
    int cnt = 0;

    for(auto it:x){
        if(it < 0) flag = true;
        if(it == 0) cnt++;
    }

    if((cnt-m+n) == m) {
        if(flag) cout << "This is a non-degenerate basic solution but not feasible." << endl;
        else cout << "This is a non-degenerate basic solution and feasible." << endl;
    }
    else if(cnt){
        if(flag) cout << "This is a degenerate basic solution but not feasible." << endl;
        else cout << "This is a degenerate basic solution and feasible." << endl; 
    }
    else {
        if(flag) cout << "This is a basic solution but not feasible." << endl;
        else cout << "This is a basic solution and feasible." << endl; 
    }

    // for(auto it:ans) cout << it << " "; cout << endl;

    if(!flag) final.push_back(ans);

    return;
}

int main() {    
    // cout << "Enter the number of variables: ";
    int n; cin >> n;
    // cout << endl;

    // cout << "Enter the number of equations: ";
    int m; cin >> m;
    // cout << endl;

    bool maximize;
    // cout << "type 1 if you want to maximize z else (minimize) type 0 ";
    cin >> maximize;

    vector<double> coeff(n, 0);
    for(int  i=0; i<n; i++) 
        cin >> coeff[i];
    
    if(maximize == 0){
        for(int i=0; i<n; i++)  
            coeff[i] = -1.0 * coeff[i];
    }

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

    vector<vector<double>> final;

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
        
        gaussSeidel(a1, b, perm, final);        

        cout << endl;
    } while(next_permutation(perm.begin(), perm.end()));

    vector<double> optimalSoln(n);
    double maxx = -1e9;

    for(auto it:final){
        double tmpMax = 0;

        // for(auto i:it) cout << i << " "; cout << endl;

        for(int i=0; i<n; i++){
            tmpMax += (coeff[i] * it[i]);
        }
        // cout << tmpMax << endl;

        if(tmpMax > maxx){
            optimalSoln = it;
            maxx = tmpMax;
        }
    }

    cout << "Optimal value is: " << maxx << endl;
    cout << "Optimal solution is: ";

    for(auto &it:optimalSoln){
        cout << it << " ";
    }
    cout << endl;

    return 0;
}