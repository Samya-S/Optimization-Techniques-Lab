#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

class soe{
public:
    vector<double> coe;
    double b;
    string op;

    void make_standard(vector<double> &a, double &bb, string &c){
        if(bb<0){
            if(c==">=") c = "<=";
            else if( c=="<=") c = ">=";
            bb = -bb;
            for(int i=0; i<a.size(); i++){
                a[i] = -a[i];
            }
        }
    }

    soe(vector<double> a, double bb, string c){
        this->coe = a;
        this->b = bb;
        this->op = c;
        make_standard(coe, b, op);
    }
};

double getMat(vector<double> &a, vector<vector<double>> &b, int j){
    double sum = 0;
    int m = a.size();
    for(int i=0; i<m; i++){
        sum += a[i]*b[i][j];
    }
    return sum;
}

int main(){
    int n, m;
    cin >> n >> m;
    bool maximum;
    cin >> maximum;

    vector<double> coeff(n+m, 0);
    for(int i=0; i<n; i++){
        cin >> coeff[i];
        if(!maximum) coeff[i]*=-1;    }

    vector<soe> mat;
    for(int i=0; i<m; i++){
        vector<double> rows(n);
        for(int j=0; j<n; j++)
            cin >> rows[j];
        string c; cin >> c;
        double b; cin >> b;
        soe eq(rows,b,c);
        mat.push_back(eq);
    }

    vector<int> restricted(n);
    for(int i=0; i<n; i++)
        cin >> restricted[i];
    
    cout << "Maximize: ";
    for(int i=1; i<=n; i++){
        cout << (coeff[i-1]<0 ? "- " : "+ ") << abs(coeff[i-1]) << "x" << i << " ";
    }
    cout << endl << endl;

    // char t = 'a';
    vector<vector<double>> stf(m, vector<double> (m+n, 0));
    int cnt = n;
    for(int i=0; i<m; i++){
        soe m1 = mat[i];
        for(int j=1; j<=n+1; j++){
            if(j == n+1){
                stf[i][cnt] = 1;
                cnt++;
            } else {
                stf[i][j-1] = m1.coe[j-1];
            }
        }
    }
    
    vector<double> del(m+n, 0);
    vector<double> b(m), XB(m), CB(m, 0);
    vector<string> B(m);
    for(int i=0; i<m; i++){
        string s = "x";
        s += to_string(n+i+1);
        B[i] = s;
    }

    for(int i=0; i<m; i++){
        b[i] = mat[i].b;
        XB[i] = mat[i].b;
    }

    for(int i=0; i<m+n; i++){
        double sum = getMat(CB, stf, i) - coeff[i];
        del[i] = sum;
    }

    cout << "       ";
    for(int i=0; i<m+n; i++){
        if(restricted[i] || i > m){
            if(coeff[i]>=0) cout << " ";
            cout << coeff[i] << "  ";
        } else {
            if(coeff[i]>=0) cout << " ";
            cout << coeff[i] << "  ";
            if(coeff[i]<0) cout << " ";
            cout << -coeff[i] << "  ";
        }
    }
    cout << endl;

    cout << "XB ";
    cout << "CB  ";
    for(int i=1; i<=n+m; i++){
        if(restricted[i-1] || i >= n)
            cout << "x" << i << "  ";
        else{
            cout << "x" << i << "' x" << i << "* ";
        }
    }   
    cout << "b" << endl;

    for(int i=0; i<m; i++){
        cout << "x" << n+i+1 << " ";
        cout << " " << 0 << "  ";
        for(int j=0; j<n+m; j++){
            if(stf[i][j]>=0) cout << " ";
            if(restricted[j] || j > m){
                cout << stf[i][j] << "  ";
            } else {
                cout << stf[i][j] << "  ";
                if(stf[i][j]<0) cout << " ";
                cout << -stf[i][j] << "  ";
            }
        }
        cout << b[i] << endl;
    }
    cout << "       ";

    for(int i=0; i<m+n; i++){
        if(restricted[i] || i > m){
            if(del[i]>=0) cout << " ";
            cout << del[i] << "  ";
        } else {
            if(del[i]>=0) cout << " ";
            cout << del[i] << "  ";
            if(del[i]<0) cout << " ";
            cout << -del[i] << "  ";
        }
    }
    cout << " " << endl;

    return 0;
}