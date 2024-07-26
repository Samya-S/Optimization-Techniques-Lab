#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    // cout << "Enter the number of equations: ";
    cin >> n;
    // cout << endl;

    vector<vector<double>> a(n, vector<double> (n, 0));
    vector<double> b(n, 0);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            // cout << "a[" << i << ", " << j << "] = ";
            cin >> a[i][j];
        }
        // cout << "b[" << i << "] = ";
        cin >> b[i];
        // cout << endl;
    }

    vector<double> x(n, 0), y(n, 0);
    // cout << "Enter initial values of x" << endl;
    for (int i = 0; i < n; i++) {
        // cout << "x:[" << i << "] = ";
        cin >> x[i];
    }

    // cout << endl << "Enter the no. of iteration(s) : ";
    int iter; cin >> iter;
    // cout << endl;

    // cout << endl << "Enter the allowed error: ";
    // int delta; cin >> delta;
    // cout << endl;

    int error = 1e9;
    while (iter--) {
    // do {
        for (int i = 0; i < n; i++) {
            y[i] = (b[i] / a[i][i]);
            for (int j = 0; j < n; j++) {
                if (j == i)
                    continue;
                y[i] = y[i] - ((a[i][j] / a[i][i]) * x[j]);
                x[i] = y[i];
            }
            // error = max(error, )
            cout << "x" << i + 1 << " = " << fixed << setprecision(6) << y[i] << " ";
        }
        cout << endl;
    // } while(error > delta);
    }

    return 0;
}
