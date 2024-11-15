#include <bits/stdc++.h>
using namespace std;

#define sizeOf(a) a.size()

class dual_simplex{
public:
    int rows, cols;
    vector<vector<float>> A; // Coefficient matrix
    vector<float> B; // RHS of constraints
    vector<float> C; // Coefficient of the objective function
    vector<int> basicVars; // Indices of basic variables
    vector<float> D; // Delta values
    bool dual_simplex_possible = 1;
    bool isMaximize = 1;
    bool optimality = 0;

    dual_simplex(vector<vector<float>> matrix, vector<float> b, vector<float> c, vector<int> basis){
        rows = matrix.size();
        cols = matrix[0].size();
        A = matrix;
        B = b;
        C = c;
        basicVars = basis;
        printTableau();
        iterate();
    }

    void calculateDelta() {
        D.clear();
        for (int j = 0; j < cols; j++) {
            float delta = 0;
            for (int i = 0; i < sizeOf(A); i++) {
                delta += C[basicVars[i]] * A[i][j];
            }
            delta -= C[j];
            if (abs(delta) < 10e-6)
                delta = 0;
            D.push_back(delta);
        }
    }

    void printMatrix(const vector<float> &matrix) {
        cout << setw(10) << " " << setw(10) << "cj";
        for (float val : matrix) {
            cout << setw(10) << val; // Adjust width to align properly
        }
        cout << endl;
    }

    void printbasicvars(){
        cout << "Basic Variables : \t";
        for (auto var : basicVars)
            cout << "x" << var+1 << " ";
        cout << endl;
        return;
    }

    void printnonbasicvars(){
        cout << "Non Basic Variables : \t";
        for (int i  = 0; i < cols; i++){
            if (find(basicVars.begin(), basicVars.end(), i)==basicVars.end())
                cout << "x" << i+1 << " ";
        }
        cout << endl;
        return;
    }

    void printTableau() {
        if (!dual_simplex_possible)
            return;
        cout << "\nDual Simplex Tableau:\n";
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------\n";

        // Print the objective function coefficients
        printMatrix(C);

        cout << "-----------------------------------------------------------------------------------------------------------------------------------------\n";

        cout << setw(10) << "CB" << setw(10) << "xB";
        for (int j = 0; j < A[0].size(); j++) {
            cout << setw(10) << "x" + to_string(j + 1);
        }
        cout << setw(10) << "b\n";
        
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < sizeOf(A); i++) {
            cout << setw(10) << C[basicVars[i]]<< setw(10) << "x" + to_string(basicVars[i] + 1) ;
            for (int j = 0; j < cols; j++) {
                cout << setw(10) << A[i][j];
            }
            if(abs(B[i])<10e-6)
                B[i] = 0;
            cout << setw(10) << B[i] << endl;
        }
        
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------\n";
        calculateDelta();
        cout << setw(10) << " " << setw(10) << " ";
        for (int j = 0; j < cols; j++) {
            cout << setw(10) << D[j];
        }
        cout << "\n-----------------------------------------------------------------------------------------------------------------------------------------\n";
    }

    void iterate() {
        int iterations = 0;
        while (dual_simplex_possible) {
            iterations++;
            cout << "\n\nIteration = " << iterations << endl;
            //printbasicvars();
            //printnonbasicvars();
            printSolution(optimality);
            calculateDelta();
            //printDelta();
            
            int leavingVar = findLeavingVariable();
            if (leavingVar == -1) {
                cout << "\nOptimal solution found!\n\n";
                break;
            }

            int enteringVar = findEnteringVariable(leavingVar);
            if (enteringVar == -1) {
                cout << "No feasible solution.\n";
                break;
            }

            cout << "Entering variable : x" << enteringVar + 1 << endl;
            cout << "Leaving variable : x" << basicVars[leavingVar] + 1 << endl;
            pivot(enteringVar, leavingVar);
            printTableau();
        }
        if (dual_simplex_possible) {
            printSolution(1);
        }
    }

    int findEnteringVariable(int leavingVar) {
        float maxRatio = -FLT_MAX;
        int enteringVar = -1;

        for (int j = 0; j < cols ; j++) {
            if (A[leavingVar][j] < 0) { // Only consider negative coefficients
                float ratio = D[j] / A[leavingVar][j];
                if (ratio >= maxRatio) {
                    maxRatio = ratio;
                    enteringVar = j;
                }
            }
        }
        
        if (enteringVar == -1) {
            dual_simplex_possible = false;
            cout << "No feasible solution exists.\n";
        }
        return enteringVar;
    }

    int findLeavingVariable() {
        float minB = *min_element(B.begin(), B.end());
        if (minB >= 0) {
            // All variables are non-negative, optimal solution found
            dual_simplex_possible = false;
            return -1;
        }
        return distance(B.begin(), min_element(B.begin(), B.end())); // Index of most negative basic variable
    }

    void pivot(int enteringVar, int leavingVar) {
        // Get the pivot element from the matrix A
        float pivotElement = A[leavingVar][enteringVar];
        // cout << basicVars[leavingVar] << endl;
        // Normalize the leaving row (leavingVar) by dividing by the pivot element
        for (int j = 0; j < sizeOf(A[0]); j++) {
            A[leavingVar][j] /= pivotElement;
        }
        B[leavingVar] /= pivotElement;

        // Perform row operations to zero out the entering variable in other sizeOf(A)
        for (int i = 0; i < sizeOf(A); i++) {
            if (i != leavingVar) {
                float factor = A[i][enteringVar];
                for (int j = 0; j < sizeOf(A[0]); j++) {
                    A[i][j] -= factor * A[leavingVar][j];
                    // cout << A[i][j] << " ";
                }
                // cout << endl;
                B[i] -= factor * B[leavingVar];
            }
        }

        // Update the basic variables to reflect the pivot operation
        basicVars[leavingVar] = enteringVar;
    }

    void printSolution(bool optimality) {
        if (optimality)
            cout << "\nOptimal Solution : \n";
        else
            cout << "\nBasic Feasible Solution:\n";
        for (int i = 0; i < cols - sizeOf(A); i++) { // Only iterate over the original variables
            float value = 0;
            for (int j = 0; j < sizeOf(A); j++) {
                if (basicVars[j] == i) {
                    value = B[j];
                    break;
                }
            }
            cout << "x" << i + 1 << " = " << value << endl; // Print only the original variable values
        }

        float optimalValue = 0;
        for (int i = 0; i < sizeOf(A); i++) {
            optimalValue += C[basicVars[i]] * B[i];
        }
        if (optimality){
            cout << "\nOptimal Objective Value: " << ((isMaximize ? optimalValue : -optimalValue)) << "\n\n";
        }   
        else    
            cout << "\nObjective Value: " << ((isMaximize ? optimalValue : -optimalValue)) << endl;
    }

    bool is_integer_solution(){
        for (int i = 0; i < (int) B.size(); i++){
            if (abs(B[i] - round(B[i])) >= 10e-6){
                // cout <<  endl << B[i] << " " << floor(B[i]) << endl;
                return 0;
            }

        }
        cout << "Dual simplex here has integer solution\n";
        cout << "Integer solution found!\n";
        return 1;
    }
};

class big_m {
public:
    int rows, cols;
    int total_slack{0}, total_surplus{0}, total_artificial{0};
    vector<vector<float>> A; // Coefficient matrix
    vector<float> B; // RHS of constraints
    vector<float> C; // Coefficient of the objective function
    vector<int> basicVars; // Indices of basic variables
    vector<float> D; // Delta values
    float minRatio = FLT_MAX;
    float pivotElement = FLT_MAX;
    float M;
    bool big_m_possible = 1;
    bool isMaximize = 1;
    bool isOptimal = 0;
    big_m(vector<vector<float>> matrix, vector<float> b, vector<float> c, vector<int> unrestrictedVars, vector<string> &comp, bool &flag) {
        A = matrix;
        B = b;
        C = c;
        isMaximize = flag;
        M = isMaximize ? -10e7 : -10e7; 

        rows = matrix.size();
        cols = matrix[0].size();

        make_rhs_positive(comp);
        handleUnrestrictedVariables(unrestrictedVars); // Handle unrestricted variables
        count_variables(comp);
        expandMatrix(comp); // Expand the matrix to include slack variables, artificial vrariables and surplus variables

        rows = A.size();
        cols = A[0].size();
        basicVars = identify_basis();
        printTableau();
        iterate();
    }

    void count_variables(const vector<string> &comp) {
        for (const auto &c : comp) {
            if (c == "<=") {
                total_slack++;
            } else if (c == ">=") {
                total_surplus++;
                total_artificial++;
            } else if (c == "=") {
                total_artificial++;
            }
        }
    }

    void make_rhs_positive(vector<string> &comp) {
        for (int j = 0; j < (int) B.size(); j++){
            if (B[j] < 0){
                B[j]*=-1;
                for (int i = 0; i < (int) A[0].size(); i++)
                    A[j][i]*=-1;
                if(comp[j] == "<=")
                    comp[j] = ">=";
                else if (comp[j] == ">=")
                    comp[j] = "<=";
            }
        }
        return;
    }

    void handleUnrestrictedVariables(const vector<int>& unrestrictedVars) {
        for (int index : unrestrictedVars) {
            // Add yj and zj
            for (int i = 0; i < sizeOf(A); i++) {
                //A[i].push_back(-A[i][index]); // Adding -zj
                A[i].insert(A[i].begin() + index + 1, -A[i][index]);
            }
            // C.push_back(-C[index]); // Adding -czj
            C.insert(C.begin() + index + 1, -C[index]);
            cols++; // Increase the number of columns
        }
    } 

    void expandMatrix(vector<string> &comp) {
        int m = A.size();
        int n = A[0].size();

        // Resize each row to accommodate new variables
        for (auto &row : A) {
            row.resize(n + total_slack + total_surplus + total_artificial, 0);
        }

        int slack_index = n;
        int surplus_index = n + total_slack;
        int artificial_index = n + total_slack + total_surplus;

        for (int i = 0; i < m; i++) {
            if (comp[i] == "<=") {
                A[i][slack_index++] = 1;  // Add slack variable
            } else if (comp[i] == ">=") {
                A[i][surplus_index++] = -1;  // Add surplus variable
                A[i][artificial_index++] = 1;  // Add artificial variable
            } else if (comp[i] == "=") {
                A[i][artificial_index++] = 1;  // Add artificial variable
            }
        }
        C.resize(n+total_slack+total_artificial+total_surplus);
        for (int i = n+total_slack + total_surplus; i < (int) C.size(); i++)
            C[i] = M;
    }

    vector<int> identify_basis() {  // change the logic so that you traverse the cols first to check if its 1 and then move further down till you like get the element
        vector<int> basis_indices;
        int m = A.size();
        for (int i = 0; i < m; i++){
            for (int j = cols-total_slack-total_surplus-total_artificial; j < cols; j++){
                int one_count = 0;
                if (A[i][j] == 1){
                    for (int k = 0; k < m; k++){
                        if(A[k][j] == 0)
                            continue;
                        else if (A[k][j] == 1)
                            one_count++;
                        else
                            one_count--;
                    }
                    if(one_count == 1)
                        basis_indices.push_back(j);
                }
            }
        }
        return basis_indices;
    }

    void calculateDelta() {
        D.clear();
        for (int j = 0; j < cols; j++) {
            float delta = 0;
            for (int i = 0; i < sizeOf(A); i++) {
                delta += C[basicVars[i]] * A[i][j];
            }
            delta -= C[j];
            if (abs(delta) < 10e-6)
                delta = 0;
            D.push_back(delta);
        }
    }

    void printMatrix(const vector<float> &matrix) {
        cout << setw(10) << " " << setw(10) << "cj";
        for (float val : matrix) {
            cout << setw(10) << val; // Adjust width to align properly
        }
        cout << endl;
    }

    void printbasicvars(){
        cout << "Basic Variables : \t";
        for (auto var : basicVars){
            if (var+1 <= cols-total_artificial)
                cout << "x" << var+1 << " ";
            else
                cout << "A" << var+1-(cols-total_artificial) << " ";
        }
        cout << endl;
        return;
    }

    void printnonbasicvars(){
        cout << "Non Basic Variables : \t";
        for (int i  = 0; i < cols; i++){
            if (find(basicVars.begin(), basicVars.end(), i)==basicVars.end()){
                if (i+1 <= cols-total_artificial)
                    cout << "x" << i+1 << " ";
                else
                    cout << "A" << i+1-(cols-total_artificial) << " ";
            }
        }
        cout << endl;
        return;
    }

    void printDelta(){
        cout << "\nDelta (∆j) values:\n"; 
        for (int j = 0; j < cols; j++)
            cout << "∆" << j + 1 << " = " << D[j] << " ";
        cout << endl;
        cout << "Most negative Delta (∆j) value : " << "∆" << (distance( D.begin(), min_element(D.begin(), D.end())) + 1) << " = " << *min_element(D.begin(), D.end()) << "\n\n";
    }

    void printTableau() {
        cout << "Big-M Tableau:\n";
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------\n";

        // Print the objective function coefficients
        printMatrix(C);

        cout << "-----------------------------------------------------------------------------------------------------------------------------------------\n";

        cout << setw(10) << "CB" << setw(10) << "xB";
        for (int j = 0; j < cols; j++) {
            if (j+1 <= cols-total_artificial){
                cout << setw(10) << "x" + to_string(j + 1);
            }
            else
                cout << setw(10) << "A" + to_string(j+1 - (cols-total_artificial));
        }
        cout << setw(10) << "b\n";
        
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < sizeOf(A); i++) {
            if (basicVars[i]+1<=cols-total_artificial){
                cout << setw(10) << C[basicVars[i]]<< setw(10) << "x" + to_string(basicVars[i] + 1) ;
            }
            else
                cout << setw(10) << C[basicVars[i]]<< setw(10) << "A" + to_string(basicVars[i] + 1 - (cols-total_artificial));
            for (int j = 0; j < cols; j++) {
                cout << setw(10) << A[i][j];
            }
            cout << setw(10) << B[i] << endl;
        }
        
        cout << "-----------------------------------------------------------------------------------------------------------------------------------------\n";
        calculateDelta();
        cout << setw(10) << " " << setw(10) << " ";
        for (int j = 0; j < cols; j++) {
            cout << setw(10) << D[j];
        }
        cout << "\n-----------------------------------------------------------------------------------------------------------------------------------------\n";
    }

    void iterate() {
        int iterations = 0;
        while (big_m_possible) {
            iterations++;
            cout << "\n\nIteration = " << iterations << endl;
            // printbasicvars();
            // printnonbasicvars();
            // printFinalSolution(0);
            calculateDelta();
            // printDelta();

            int enteringVar = findEnteringVariable();
            if (enteringVar == -1) {
                cout << "Total iterations = " << iterations << endl;
                cout << "\nFinal Solution:\n";
                break;
            }
            

            int leavingVar = findLeavingVariable(enteringVar);
            if (leavingVar == -1) {
                cout << "Unbounded solution detected!\n";
                big_m_possible = false; // Stop further iterations
                break;
            }
            if (enteringVar > cols-total_artificial-1)
                cout << "Entering variable : A" << enteringVar + 1 - (cols-total_artificial) << endl;
            else
                cout << "Entering variable : x" << enteringVar + 1 << endl;
            if (basicVars[leavingVar] > cols-total_artificial-1)
                cout << "Leaving variable : A" << basicVars[leavingVar] + 1 - (cols-total_artificial) << endl;
            else
                cout << "Leaving variable : x" << basicVars[leavingVar] + 1 << endl;
            pivot(enteringVar, leavingVar);

            cout << "\nPivot element is " << pivotElement << " at (" << leavingVar + 1 << ", " << enteringVar + 1 << ")\n";
            cout << "MinRatio is : " << minRatio << "\n\n"; 

            printTableau();

        }
        if (big_m_possible) {
            printFinalSolution(1); // Only print the optimal solution if it's feasible
        }
    }

    int findEnteringVariable() {
        float minDelta = *min_element(D.begin(), D.end());
        return (minDelta < 0) ? distance(D.begin(), find(D.begin(), D.end(), minDelta)) : -1;
    }

    int findLeavingVariable(int enteringVar) {
        minRatio = FLT_MAX;
        int leavingVar = -1;
        vector<int> tieIndices;
        
        // First pass to find the minimum ratio
        for (int i = 0; i < sizeOf(A); i++) {
            if (A[i][enteringVar] > 0) {
                float ratio = B[i] / A[i][enteringVar];
                if (ratio < minRatio) {
                    minRatio = ratio;
                    leavingVar = i;
                    tieIndices.clear(); // Clear previous ties
                    tieIndices.push_back(i);
                } else if (ratio == minRatio) {
                    tieIndices.push_back(i); // Add to tie list
                }
            }
        }

        // If there's a tie, give first preference to artificial variable then proceed further
        if (tieIndices.size() > 1) {
            cout << "\ndegeneracy detected!\n";
            for (int i = 0; i < (int) tieIndices.size(); i++){
                if(basicVars[tieIndices[i]] >= sizeOf(A)+total_slack+total_surplus-1)
                    return tieIndices[i];
            }
            leavingVar = *max_element(tieIndices.begin(), tieIndices.end());
        }
        return leavingVar;
    }

    void pivot(int enteringVar, int leavingVar) {
        pivotElement = A[leavingVar][enteringVar];
        //cout << "pivot element is " << pivotElement  << "at" << "(" << leavingVar+1 << ", " << enteringVar+1 << ")" <<endl;
        for (int j = 0; j < cols; j++) {
            A[leavingVar][j] /= pivotElement;
        }
        B[leavingVar] /= pivotElement;

        for (int i = 0; i < sizeOf(A); i++) {
            if (i != leavingVar) {
                float factor = A[i][enteringVar];
                for (int j = 0; j < cols; j++) {
                    A[i][j] -= factor * A[leavingVar][j];
                }
                B[i] -= factor * B[leavingVar];
            }
        }

        basicVars[leavingVar] = enteringVar;
    }

    void check_alternative(){
        for (int i = 0; i < (int) D.size(); i++){
            if (D[i] == 0){
                if (find(basicVars.begin(), basicVars.end(), i) == basicVars.end()){
                    cout << "alternative solution found!\n";

                    int enteringVar = i;
                    int leavingVar = findLeavingVariable(enteringVar);
                    if (enteringVar > cols-total_artificial-1)
                        cout << "Entering variable : A" << enteringVar + 1 - (cols-total_artificial) << endl;
                    else
                        cout << "Entering variable : x" << enteringVar + 1 << endl;
                    if (basicVars[leavingVar] > cols-total_artificial-1)
                        cout << "Leaving variable : A" << basicVars[leavingVar] + 1 - (cols-total_artificial) << endl;
                    else
                        cout << "Leaving variable : x" << basicVars[leavingVar] + 1 << endl;
                    pivot(enteringVar, leavingVar);

                    cout << "\nPivot element is " << pivotElement << " at (" << leavingVar + 1 << ", " << enteringVar + 1 << ")\n";
                    cout << "MinRatio is : " << minRatio << "\n\n"; 

                    printTableau();

                    cout << "optimal solution : \n";
                    printFinalSolution(0);

                    printbasicvars();

                    printnonbasicvars();

                    printDelta();

                    break;
                }
            }
        }
    }

    void printFinalSolution(bool optimality) {
        if (optimality){
            for (int i = 0; i < (int)basicVars.size(); i++){
                if (basicVars[i] > cols-total_artificial-1){
                    if (B[i] != 0){
                        cout << "not a feasible solution!\n";
                        return;
                    }
                }
            }
            cout << "\nOptimal Solution : \n";
        }
        else
            cout << "\nBasic Feasible Solution:\n";
        for (int i = 0; i < cols - total_surplus - total_slack - total_artificial; i++) { // Only iterate over the original variables
            float value = 0;
            for (int j = 0; j < sizeOf(A); j++) {
                if (basicVars[j] == i) {
                    value = B[j];
                    break;
                }
            }
            cout << "x" << i + 1 << " = " << value << endl; // Print only the original variable values
        }
        float optimalValue = 0;
        for (int i = 0; i < sizeOf(A); i++) {
            optimalValue += C[basicVars[i]] * B[i];
        }
        if (optimality){
            cout << "\nOptimal Objective Value: " << (optimalValue)  << "\n\n";
            check_alternative();
            final_ans(optimalValue);
        }
        else    
            cout << "\nObjective Value: " << (optimalValue) << endl;
    }

    void final_ans(float optimalValue){
        cout << "the final answer for optimal value of the original function is " << (isMaximize ? optimalValue : -optimalValue) << "\n\n";
        isOptimal = 1;
        return;
    }

    bool is_integer_solution(){
        for (int i = 0; i < (int) B.size(); i++){
            if (abs(B[i] - round(B[i])) >= 10e-6)
                return 0;
        }
        cout << "Big-M here has integer solution\n";
        cout << "Integer solution found!\n";
        return 1;
    }
};

class GomoryCut{
private:
    vector<vector<float>> A; // Coefficient matrix
    vector<float> B; // RHS of constraints
    vector<float> C; // Coefficient of the objective function
    vector<int> basicVars;
    int m;
    int n;
public:
    GomoryCut(vector<vector<float>> a, vector<float> b, vector<float> c, vector<int> unrestrictedVars, vector<string> &eq, bool flag){
        big_m solve_I(a, b, c, unrestrictedVars, eq, flag);
        n = solve_I.cols-solve_I.total_artificial;
        m = (int) solve_I.basicVars.size();

        if(solve_I.isOptimal && solve_I.big_m_possible){
            A = solve_I.A;
            B = solve_I.B;
            C = solve_I.C;
            basicVars = solve_I.basicVars;
            reduce_table(solve_I);
        }

        if(!solve_I.is_integer_solution()){
            cout << "\n-----------------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "\n-----------------------------------------------------------------------------------------------------------------------------------------\n";
            cout << "DOING GOMORY_CUT\n";
            int gomory_index = select_fractional_row();
            int iteration = 1;
            generate_gomory_cut(gomory_index);
            dual_simplex solve_II(A, B, C, basicVars);
            A = solve_II.A;
            B = solve_II.B;
            C = solve_II.C;
            basicVars = solve_II.basicVars;
            n = A[0].size();
            bool is_integer_solution = solve_II.is_integer_solution();
            while(!is_integer_solution && iteration <= 10){
                cout << "GOMORY ITERATION: "<< iteration << "\n\n";
                int gomory_index = select_fractional_row();
                generate_gomory_cut(gomory_index);
                dual_simplex solve_II(A, B, C, basicVars);
                A = solve_II.A;
                B = solve_II.B;
                C = solve_II.C;
                basicVars = solve_II.basicVars;
                is_integer_solution = solve_II.is_integer_solution();
                n = A[0].size();
                iteration++;
            }
        }        
    }

    void reduce_table(big_m solve_I){
        for(auto &row : A)
            row.resize(solve_I.cols-solve_I.total_artificial);
        C.resize(solve_I.cols-solve_I.total_artificial);
        return;
    }

    int select_fractional_row(){
        vector<pair<float, int>> frac_rows;    // value of frac part and the index of the row
        float max_frac = 0;
        int index = -1;
        for(int i = 0; i < (int) B.size(); i++){
            if(frac(B[i]) > max_frac){
                max_frac = frac(B[i]);
                index = i;
            }
        }        
        return index;
    }

    float frac(float x){
        double frac = x - trunc(x);
        if (abs(frac) > 10e-6)
            return (frac > 0 ? frac : 1+frac);
        else
            return 0;
    }
    
    void generate_gomory_cut(int gomory_index){
        A.push_back(vector<float> ((int) A[0].size()));
        B.push_back(-1*frac(B[gomory_index]));
        for(int i = 0; i < n; i++){
            A[sizeOf(A)-1][i] = -1*frac(A[gomory_index][i]);
        }
        for( auto &row : A)
            row.push_back(0);
        *(A[sizeOf(A)-1].end()-1) = 1;
        basicVars.push_back(n);
        C.push_back(0);
    }
};

int main() {
    int n, m;
    cout << "Enter number of variables (n), number of equations (m): ";
    cin >> n >> m;

    vector<vector<float>> A(m, vector<float>(n));
    vector<string> comp(m);
    vector<float> B(m);

    cout << "Enter the equations row-wise (for eg: 2 3 <= 5):\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) 
            cin >> A[i][j];
        cin >> comp[i];
        cin >> B[i];
    }

    string maxORmin;
    cout << "Do you want to maximize or minimize the objective function? (max/min): ";
    cin >> maxORmin;
    bool isMaximize = (maxORmin == "max");

    vector<float> C(n);
    cout << "Enter the coefficients of the objective function: ";
    for (int i = 0; i < n; i++) 
        cin >> C[i];
    
    if(!isMaximize){
        for(auto &it : C)
            it*= -1;
        isMaximize = 1;
    }

    vector<int> unrestrictedVars;
    int countUnrestricted;
    cout << "Enter the number of unrestricted variables: ";
    cin >> countUnrestricted;
    cout << "Enter the indices of unrestricted variables (1-based): ";
    for (int i = 0; i < countUnrestricted; i++) {
        int index;
        cin >> index;
        unrestrictedVars.push_back(index-1+i); // Convert to 0-based index
    }
    cout << setprecision(3);

    GomoryCut GomoryCut(A, B, C, unrestrictedVars, comp, isMaximize);

    return 0;
}