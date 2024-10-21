#include <bits/stdc++.h>
using namespace std;
#define M 1000
#define float double

int original_var;
int type;

int main() {
    int n, m;

    cout << "Enter the number of variables: ";
    cin >> n;
    cout << "Enter the number of constraints: ";
    cin >> m;
    original_var=n;

    vector<int> unrestricted(n);
    cout << "For each variable, enter 1 if unrestricted else 0:\n";
    for(int i=0; i<n; i++){
    	cin >> unrestricted[i];
    }

    vector<vector<float>> A(m, vector<float>(n));

    cout << "\nEnter the coefficients of the constraint equations:\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    // Declaration of the objective function coefficients
    vector<float> c(n);
    cout << "\nEnter coefficients of the objective function: ";
    for (int i = 0; i < n; i++) {
        cin >> c[i];
    }

    // Objective function classification
    cout << "\nEnter type of problem: \n1. Maximization \n2. Minimization\n";

    cin >> type;
    if (type == 2) {
        for (int i = 0; i < n; i++) {
            c[i] = -c[i]; // Flip the sign for minimization problems
        }
    }

    vector<float> rhs(m);  // Change rhs to vector
    cout << "\nEnter the RHS values:  ";
    for (int i = 0; i < m; i++) {
        cin >> rhs[i];
    }

    cout << "\nEnter the sign of the constraints (for each constraint):\n1. <=\n2. >=\n3. =\n";
    set<int> unres_dual;
    for (int i = 0; i < m; i++) {
        int sign;
        cin >> sign;

        if (sign == 2) { // Greater than or equal (>=) - Add surplus and artificial variable
            rhs[i]=-rhs[i];
            for(int j=0; j<n; j++){
                A[i][j]=-A[i][j];
            }
        }

        else if(sign == 3) {
        	unres_dual.insert(i);
        }
    }

    // cout << "\n\nStandard Form:  ";
    // cout << "\nMax z = ";
    // for(int i=0; i<n; i++){
    //     if(i>0 && c[i]>0){cout<<" +";}
    //     if(i>0 && c[i]<0){cout<<" -";}
    //     if(c[i]!=0)
    //     	cout<<c[i]<<"x"<<i+1;

    // }

    // for(int i=0; i<m; i++){
    //    cout<<"\n";
    //    for(int j=0; j<n; j++){
    //     if(j>0 && A[i][j]>0){cout<<" +";}
    //     if(j>0 && A[i][j]<0){cout<<" -";}
    //     if(A[i][j]!=0)
    //     	cout<<A[i][j]<<"x"<<j+1;
    //    }
    //    cout<<"  <= "<<rhs[i];
    // }

    vector<float>c_dual(m);
    for(int i=0; i<m; i++){c_dual[i]=rhs[i];}

    vector<vector<float>> A_dual(n, vector<float>(m));
    for(int i=0; i<n; i++){
       for(int j=0; j<m; j++){
           A_dual[i][j]=A[j][i];
       }
    }

    vector<float>rhs_dual(n);
    for(int i=0; i<n; i++){rhs_dual[i]=c[i];}

    cout << "\n\nDual Formation:  ";
    cout << "\nMin z = ";
    for(int i=0; i<m; i++){
        if(c_dual[i]>0){cout<<"+";}
        if(c_dual[i]!=0)
        	cout<<c_dual[i]<<"w"<<i+1<<' ';

    }

    cout<<"\nsubject to:";
    for(int i=0; i<n; i++){
       cout<<"\n";
       for(int j=0; j<m; j++){
        if(A_dual[i][j]>0){cout<<"+";}
        if(A_dual[i][j]!=0)
        	cout<<A_dual[i][j]<<"w"<<j+1<<' ';
       }
       if(unrestricted[i])
       	cout<<"  = "<<rhs_dual[i];
       else
       	cout<<"  >= "<<rhs_dual[i];
    }
    cout<<'\n';

    for(int i=0; i<m; i++){
    	if(unres_dual.find(i)==unres_dual.end())
    		cout<<'w'<<i+1<<">=0, ";
    }

    if(!unres_dual.empty()){
    	cout << "\nUnrestricted variables: ";
    	for(auto i:unres_dual)
    		cout<<'w'<<i+1<<", ";
	}

    cout << endl;
    return 0;
}
