#include <bits/stdc++.h>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;
int fl = 0;
class Simplex {
private:
    int n, m, artificialCount;
    vector<vector<double>> A;
    vector<double> c;
    vector<double> b;
    vector<double> CB;
    vector<int> B;
    vector<double> delta; 
     int extravar = 0;
    set<int> arti_ind;
    vector<int> isunrestriced;
    vector<int> neg;

public:
    Simplex(int variables, int constraints) : n(variables), m(constraints),
                                              A(constraints, vector<double>(variables + constraints+constraints+variables)),
                                              c(variables + constraints + constraints+variables), // c includes slack and artificial variables
                                              b(constraints),
                                              CB(constraints),
                                              B(constraints),
                                              neg(variables),
                                              isunrestriced(variables),
                                              delta(variables + constraints + constraints),
                                              artificialCount(0) {}

    void readInput() {
        cout<<"1 for maximization problem and 0 for minimization problem"<<endl;
       // int fl;
        cin>>fl;
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
        cout<<"enter 0 for restricted variable and 1 for unrestricted variable"<<endl;
        for(int i = 0 ;i<n;i++) cin>>isunrestriced[i];

        if(fl==0){
            for(int i = 0 ;i<c.size();i++) c[i]*=-1;
        }
        
        vector<string> sign(m);
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
        int cnt = 0;
        for(int i = 0 ;i<m;i++){
            if(sign[i]!="<=") cnt++;
        }
        if(cnt==0){
            cout<<"simplex method will be used here"<<endl;
        }
        else {
            cout<<"two phase method is used here "<<endl;
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
                c[n + extravar] = 0;       // Penalty M in objective function
                B[i] = n + extravar;       // Artificial variable starts as basic
                CB[i] = 0;                // Artificial variable in the basis
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
                c[n + extravar] = 0;       // Penalty M in objective function
                B[i] = n + extravar;       // Artificial variable starts as basic
                CB[i] = 0;                 // Artificial variable in the basis
                artificialCount++;
                arti_ind.insert(n+extravar);
                extravar++;
            }
        }
   
        vector<int> res_var(n);
        for(int i=0; i<n; i++){
            if(isunrestriced[i]){
                res_var[i] = extravar;
                extravar++;
            }
        }
   
        for(int i = 0 ;i<m;i++){
            for(int j = 0 ;j<n;j++){

                if(isunrestriced[j]==1){
                    A[i][n + res_var[j]] = -1.0*A[i][j];  
                    c[n + res_var[j]] =-1.0 *c[j];   
                    neg[j] = n+ res_var[j]; 
                }
            }
        }

        // if(fl==0){
        //     for(int i = 0 ;i<c.size();i++) c[i]*=-1;
        // }
    }

    void computeDelta() {
        for (int j = 0; j < n +extravar; j++) {
            double sum = 0.0;
            for (int i = 0; i < m; i++) {
                sum += CB[i] * A[i][j];
            }
            
            delta[j] = sum - c[j];
         //   if(j==3) cout<<c[j]<<endl;
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
        cout << "Simplex Table:" << endl;
        cout << "   " << "XB" << "  " << "CB";
        for (int j = 0; j < n + extravar; j++) {
            cout << "   " << "X" << j + 1;
        }
        cout << "   " << "b" << endl;

        for (int i = 0; i < m; i++) {
            
            cout << "   " << "X" << B[i] + 1 << "   " ;
            if(CB[i]==-100) cout<<"-M";
            else cout<<CB[i];
            for (int j = 0; j < n + extravar; j++) {
                cout << "   " << A[i][j];
            }
            cout << "   " << b[i] << endl;
        }

        cout << "Δj:        ";
        for (int j = 0; j < n +extravar; j++) {
            cout << "   " << delta[j];
        }
        cout << endl;
    }
    
    void checkAlternativeSolution() {
        for (int j = 0; j < n; j++) {
            if (delta[j] == 0 && find(B.begin(), B.end(), j) == B.end()) {
                cout << "Alternative solution exists due to non-basic variable X" << j + 1 << " with zero reduced cost." << endl;
                // Perform an additional pivot to compute the alternative solution
                int entering = j;
                int leaving = findLeavingVariable(entering);
                if (leaving == -1) {
                    cout << "Problem is unbounded." << endl;
                    return;
                }
                pivot(entering, leaving);

                // Output the alternative solution
                vector<double> soln(n,0.0);
                for (int i = 0; i < m; i++) {
                    if(B[i]<n){
                    double vall = b[i];
                    int neg_ind = neg[B[i]];
                    if(isunrestriced[B[i]] && (find(B.begin(),B.end(),neg_ind)!=B.end())){
                        int ind = find(B.begin(),B.end(),neg_ind) - B.begin();
                        vall-=b[ind];
                    }
                     soln[B[i]] = vall;
                   // cout << "X" << B[i] + 1 << " = " << vall << endl;

                    }
                }
                for(int i = 0 ;i<n;i++){
                    cout << "X" << i + 1 << " = " << soln[i] << endl;
                }
                return;
            }
        }
        cout << "No alternative solution exists." << endl;
    }

    void two_phase(){
        //Phase 1
        cout << "Phase 1:" << endl;
        vector<double> temp = c;
        for(int i=0; i<(int)c.size(); i++){
            double coeff = 0;
            if(arti_ind.find(i)!=arti_ind.end())
                coeff = -1;
            c[i] = coeff;
        }
        for(int i=0; i<(int)B.size(); i++){
            CB[i] = c[B[i]];
        }
        while (true) {
            computeDelta();
            printTable();

            int entering = findEnteringVariable();
            if (entering == -1) {
                cout << "Phase 1 completed." << endl;
                break;
            }

            int leaving = findLeavingVariable(entering);
            if (leaving == -1) {
                cout << "Problem is unbounded." << endl;
                return;
            }

            pivot(entering, leaving);
        }

        //Phase 2
        for(int i=0; i<m; i++){
            if(arti_ind.find(B[i])!=arti_ind.end()){
                if(b[i]!=0){
                    cout << "Solution does not exist." << endl;
                    return;
                }
            }
        }
        cout << "Phase 2:" << endl;
        c = temp;
        for(int i=0; i<(int)B.size(); i++){
            CB[i] = c[B[i]];
        }
        for(int j=0; j<(int)A[0].size(); j++){
            if(arti_ind.find(j)!=arti_ind.end() && find(B.begin(), B.end(), j) == B.end()){
                for(int i=0; i<m; i++) 
                    A[i][j]=0;
            }
        }
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
        vector<double> soln(n,0.0);
        for (int i = 0; i < m; i++) {
            if(B[i]<n){
            double vall = b[i];
            int neg_ind = neg[B[i]];
            if(isunrestriced[B[i]] && (find(B.begin(),B.end(),neg_ind)!=B.end())){
                int ind = find(B.begin(),B.end(),neg_ind) - B.begin();
                vall-=b[ind];
            }
            soln[B[i]] = vall;
          //  cout << "X" << B[i] + 1 << " = " << vall << endl;

            }
        }
        for(int i = 0 ;i<n;i++){
            cout << "X" << i + 1 << " = " << soln[i] << endl;
        }
        
        double optimalValue = 0;
        for (int i = 0; i < m; i++) {
            optimalValue += CB[i] * b[i];
        }
        if(fl==0)  optimalValue*=-1.0;
        cout << "Optimal value of the objective function: " << optimalValue << endl;

        // Check for infeasibility
        for (int i = 0; i < m; i++) {
             
            if (arti_ind.find(B[i])!=arti_ind.end() && b[i] != 0) {
                cout << "Infeasible solution: artificial variable X" << B[i] + 1 << " is still in the basis." << endl;
                return;
            }
        }

        checkAlternativeSolution();

    }

    void solve() {
        if(artificialCount){
            two_phase(); return;
        }

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
        vector<double> soln(n,0.0);
        for (int i = 0; i < m; i++) {
            if(B[i]<n){
            double vall = b[i];
            int neg_ind = neg[B[i]];
            if(isunrestriced[B[i]] && (find(B.begin(),B.end(),neg_ind)!=B.end())){
                int ind = find(B.begin(),B.end(),neg_ind) - B.begin();
                vall-=b[ind];
            }
            soln[B[i]] = vall;
          //  cout << "X" << B[i] + 1 << " = " << vall << endl;

            }
        }
        for(int i = 0 ;i<n;i++){
            cout << "X" << i + 1 << " = " << soln[i] << endl;
        }
        
        double optimalValue = 0;
        for (int i = 0; i < m; i++) {
            optimalValue += CB[i] * b[i];
        }
        if(fl==0)  optimalValue*=-1.0;
        cout << "Optimal value of the objective function: " << optimalValue << endl;

        // Check for infeasibility
        for (int i = 0; i < m; i++) {
             
            if (arti_ind.find(B[i])!=arti_ind.end() && b[i] != 0) {
                cout << "Infeasible solution: artificial variable X" << B[i] + 1 << " is still in the basis." << endl;
                return;
            }
        }

        checkAlternativeSolution();

    }

};

int main() {
    int variables, constraints;
    cout << "Enter the number of variables and constraints: ";
    cin >> variables >> constraints;

    Simplex simplex_bigm(variables, constraints);
    simplex_bigm.readInput();
    simplex_bigm.solve();

    return 0;
}