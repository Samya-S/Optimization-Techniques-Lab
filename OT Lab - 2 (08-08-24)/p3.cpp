#include <bits/stdc++.h>
#include <vector>
#include <cmath>
using namespace std;
void combinations(vector<int> &elements , int k , vector<vector<int>> &result,vector<int> &current , int start ){
    if(k==0){
        result.push_back(current);
        return;
    }
    for(int i = start ;i<=elements.size()-k ;i++){
        current.push_back(elements[i]);
        combinations(elements,k-1,result,current,i+1);
        current.pop_back();
    }
}

const double TOLERANCE = 0.001;
const int MAX_ITERATIONS = 1000;

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
        if (diag <= sum) {
            return false;
        }
    }
    return true;
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
          //  cout << "Converged after " << k + 1 << " iterations." << endl;
            return;
        }
    }

   //cout << "Maximum number of iterations reached." << endl;
}

int main(){
   int n,m;

    cout << "Enter value of n and m : ";
    cin >> n>>m;
    int  ineq;
    cout << "type 1 if it is a inequality else type 0: ";
    cin>>ineq;
    int add = 0 ;
    if(ineq) add = m;


    vector<vector<double>> A(m, vector<double>(n+add,0));
    vector<double> b(m);
    cout << "Enter the augmented matrix (coefficients):" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> A[i][j];
        }
      
    }
    int tmp = n;
    if(ineq){
    for(int i = 0 ;i<m;i++) {A[i][tmp] =1;tmp++;}
    }
     cout << "Enter the values of vector b " << endl;
    for(int i = 0 ;i<m;i++) cin >> b[i];
    if(ineq) cout<<"WE WILL USE SLACK VARIABLES HERE"<<endl;

    vector<int> indices(n+add);
    iota(indices.begin(),indices.end(),0);
    vector<vector<int>> combo;
    vector<int> curr;
    combinations(indices,m,combo,curr,0);
    for(auto c:combo){
        vector<vector<double>> AA(m, vector<double>(m));
         vector<double> x(m,0.0);
       for (int i = 0; i < m; ++i) {
           for (int j = 0; j < m; ++j) {
                  AA[i][j] = A[i][c[j]];
            }
      
        }
    
    if (isDiagonallyDominant(AA)) {
      // cout << "The matrix is diagonally dominant." << endl;
        gaussSeidel(AA, b, x);

        cout << "Solution:" << endl;
        int cnt = 0;
        for(int i = 0 ;i<m;i++){
            if(c[i]>=n && x[i]<0){
                cout<<"slack variable can't be nagative"<<endl;
                continue;
            }
        }
        for (int i = 0; i < m; ++i) {
            cout << "x" << c[i] + 1 << " = " << x[i] << endl;
            
        }
        cout<<"other variables are taken as 0"<<endl;

        bool flag = 0;
        for(int i = 0 ;i<m;i++){
            if(x[i]<0){ flag = 1;}
            if(x[i]==0.0) cnt++;
        }
        if((cnt+ n+add - m) ==m) {
            if(flag==1)  cout << "This is a nondegenerate basic solution but not feasible." << endl;
            else cout << "This is a nondegenerate basic solution and  feasible." << endl;
        }
        else if(cnt){
              if(flag==1)  cout << "This is a degenerate basic solution but not feasible." << endl;
            else cout << "This is a degenerate basic solution and  feasible." << endl; 
        }
        else {
              if(flag==1)  cout << "This is a basic solution but not feasible." << endl;
            else cout << "This is a basic solution and  feasible." << endl; 
        }




   } else {
       cout << "The matrix is not diagonally dominant." << endl;
   }



    }
    return 0 ;

}