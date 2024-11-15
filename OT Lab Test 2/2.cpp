#include <bits/stdc++.h>
using namespace std;

#define double float;

int M = 100000;
vector<int> slack, surplus, artificial, equals;
vector<vector<float>> globalintsolutions;
float finalsolutionforbound;
set<int> integervariables;

void alternatesolutionshandler(vector<vector<float>> &simplexgrid, vector<float> &solution, vector<float> &del, map<int, string> &mp, int m, int n, int extra, vector<int> &xb) {
    int cnt = 1;
    vector<int> alternateind;
    cout << "Solution " << cnt << ":";
    for (int i = 0; i < m; i++) cout << mp[xb[i]] << "=" << solution[i] << " ";
    cout << endl;
    cnt++;
    for (int i = 0; i < (n + extra); i++) {
        if (del[i] != 0)
            continue;
        int checkflag = 0;
        for (int j = 0; j < m; j++) {
            if (i == xb[j]) {
                checkflag = 1;
                break;
            }
        }
        if (checkflag == 0)
            alternateind.push_back(i);
    }
    int ss = alternateind.size();
    for (int i = 0; i < ss; i++) {
        int minratio = INT_MAX;
        int pivotrow = -1;
        for (int j = 0; j < m; j++) {
            if (simplexgrid[j][alternateind[i]] <= 0)
                continue;
            float tempratio = (solution[j] / simplexgrid[j][alternateind[i]]);
            if (tempratio < minratio) {
                minratio = tempratio;
                pivotrow = j;
            }
        }
        if (pivotrow != -1) {
            cout << "Solution " << cnt << ":";
            for (int j = 0; j < m; j++) {
                if (j == pivotrow)
                    cout << mp[alternateind[i]] << "=" << solution[j] / simplexgrid[pivotrow][alternateind[i]] << " ";
                else
                    cout << mp[xb[j]] << "=" << solution[j] - ((solution[pivotrow] * simplexgrid[j][alternateind[i]]) / simplexgrid[pivotrow][alternateind[i]]) << " ";
            }
            cout << endl;
            cnt++;
        }
    }
}

bool degeneracyhandler(vector<vector<float>> &simplexgrid, vector<float> &solution, int &pivotrow, int pivotcol, int m, int n, vector<int> &xb, float minratio) {
    vector<int> indofsameminratio;
    for (int j = 0; j < m; j++) {
        if (simplexgrid[j][pivotcol] <= 0)
            continue;
        float ratiocol = solution[j] / simplexgrid[j][pivotcol];
        if (ratiocol == minratio)
            indofsameminratio.push_back(j);
    }
    int ss = indofsameminratio.size();
    for (int i = 0; i < ss; i++) {
        cout << indofsameminratio[i] << " ";
        for (int j = 0; j < artificial.size(); j++) {
            if (xb[indofsameminratio[i]] == artificial[j]) {
                pivotrow = indofsameminratio[i];
                return true;
            }
        }
    }
    cout << endl;
    vector<vector<float>> tempcheck(ss);
    for (int i = 0; i < ss; i++) {
        for (int j = 0; j < ss; j++) {
            tempcheck[i].push_back(simplexgrid[indofsameminratio[i]][xb[indofsameminratio[j]]]);
            cout << tempcheck[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < ss; i++) {
        float tempminratio = INT_MAX;
        int temppivotrow = -1;
        int tempcnt = 0;
        for (int j = 0; j < ss; j++) {
            float kkk = tempcheck[j][i] / simplexgrid[indofsameminratio[j]][pivotcol];
            if (kkk < tempminratio) {
                tempminratio = kkk;
                temppivotrow = indofsameminratio[j];
            }
        }
        for (int j = 0; j < ss; j++) {
            float kkk = tempcheck[j][i] / simplexgrid[indofsameminratio[j]][pivotcol];
            if (kkk == tempminratio) {
                tempcnt++;
            }
        }
        if (tempcnt == 1) {
            pivotrow = temppivotrow;
            return true;
        }
    }
    return false;
}

void printtablesimplex(vector<float> &del, vector<float> &fn, map<int, string> &mp, vector<int> &xb, vector<vector<float>> &simplexgrid, vector<float> &solution, int n, int extra, int m) {
    cout << " ";
    cout << setw(11) << " " << "";
    cout << setw(11) << " ";
    for (int i = 0; i < n + extra; i++) cout << setw(11) << setprecision(2) << (fn[i]);
    cout << endl;
    cout << " ";
    cout << setw(11) << "xB" << setw(11) << "cB";
    for (int i = 0; i < n + extra; i++) cout << setw(11) << setprecision(2) << mp[i];
    cout << setw(11) << "b";
    cout << endl;
    for (int i = 0; i < m; i++) {
        cout << " ";
        cout << setw(11) << mp[xb[i]];
        cout << setw(11) << (fn[xb[i]]);
        for (int j = 0; j < n + extra; j++) {
            cout << setw(11) << setprecision(2) << (simplexgrid[i][j]);
        }
        cout << setw(11) << fixed << setprecision(2) << (solution[i]);
        cout << endl;
    }
    cout << " ";
    cout << setw(11) << " ";
    cout << setw(11) << " ";
    for (int i = 0; i < n + extra; i++) cout << setw(11) << setprecision(2) << (del[i]);
    cout << setw(11) << " ";
    cout << endl;
}

void printtablebigm(vector<float> &del, vector<float> &fn, map<int, string> &mp, vector<int> &xb, vector<vector<float>> &simplexgrid, vector<float> &solution, int n, int extra, int m) {
    cout << " ";
    cout << setw(11) << " " << "";
    cout << setw(11) << " ";
    for (int i = 0; i < n + extra; i++) {
        if (fn[i] == -100000.00)
            cout << setw(11) << "-M";
        else
            cout << setw(11) << (fn[i]);
    }
    cout << endl;
    cout << " ";
    cout << setw(11) << "xB" << setw(11) << "cB";
    for (int i = 0; i < n + extra; i++) cout << setw(11) << setprecision(2) << mp[i];
    cout << setw(11) << "b";
    cout << endl;
    for (int i = 0; i < m; i++) {
        cout << " ";
        cout << setw(11) << mp[xb[i]];
        if (fn[xb[i]] == -100000.00)
            cout << setw(11) << "-M";
        else
            cout << setw(11) << (fn[xb[i]]);
        for (int j = 0; j < n + extra; j++) {
            cout << setw(11) << setprecision(2) << (simplexgrid[i][j]);
        }
        cout << setw(11) << fixed << setprecision(2) << (solution[i]);
        cout << endl;
    }
    cout << " ";
    cout << setw(11) << " ";
    cout << setw(11) << " ";
    vector<pair<float, float>> values;
    for (int i = 0; i < (n + extra); i++) {
        float quo = 0;
        float bb = 0;
        for (int j = 0; j < m; j++) {
            if (abs(fn[xb[j]]) == 100000.0)
                quo += (((fn[xb[j]]) / abs(fn[xb[j]])) * simplexgrid[j][i]);
            else
                bb += fn[xb[j]] * simplexgrid[j][i];
        }
        if (abs(fn[i]) == 100000.0)
            quo += ((fn[i]) / abs(fn[i])) * -1;
        else
            bb += (-fn[i]);
        values.push_back({quo, bb});
    }
    for (int i = 0; i < n + extra; i++) {
        float quo = values[i].first;
        float bb = values[i].second;
        ostringstream pri;
        if (quo >= 0)
            pri << fixed << setprecision(1) << quo << "M";
        else if (quo < 0)
            pri << fixed << setprecision(1) << quo << "M";
        if (bb >= 0)
            pri << "+" << fixed << setprecision(1) << bb;
        else if (bb < 0)
            pri << fixed << setprecision(1) << bb;
        cout << setw(11) << pri.str();
    }
    cout << endl;
}

vector<float> simplexmethod(vector<vector<float>> &grid, vector<int> &basis, vector<float> &y, map<int, string> &mp, vector<float> &fn, int n, int m, int extra, int method) {
    vector<float> xici(1000, 0);
    vector<float> del(1000, 0);
    vector<float> solution = y;
    vector<vector<float>> simplexgrid = grid;
    vector<int> xb = basis;
    vector<float> finalans(1000, 0);
    float mindel = INT_MAX;
    float minratio = INT_MAX;
    int pivotcol = -1;
    int breakflag = 0;
    int pivotrow;
    for (int i = 0; i < (n + extra); i++) {
        float sum = 0;
        for (int j = 0; j < m; j++) {
            sum += fn[xb[j]] * simplexgrid[j][i];
        }
        xici[i] = sum;
    }
    for (int i = 0; i < (n + extra); i++) {
        del[i] = xici[i] - fn[i];
        if (del[i] < mindel) {
            mindel = del[i];
            pivotcol = i;
        }
        if (del[i] < 0)
            breakflag = 1;
    }

    cout << "Iteration 0:" << endl;
    if (method == 0)
        printtablesimplex(del, fn, mp, xb, simplexgrid, solution, n, extra, m);
    else
        printtablebigm(del, fn, mp, xb, simplexgrid, solution, n, extra, m);
    int itcnt = 1;
    if (breakflag == 0)
        return finalans;
    while (1) {
        vector<vector<float>> tempsimplexgrid = simplexgrid;
        vector<float> tempsoln = solution;
        minratio = INT_MAX;
        int unboundedflag = 0;
        for (int j = 0; j < m; j++) {
            if (simplexgrid[j][pivotcol] <= 0)
                continue;
            float ratiocol = solution[j] / simplexgrid[j][pivotcol];
            if (ratiocol < minratio) {
                unboundedflag = 1;
                minratio = ratiocol;
                pivotrow = j;
            }
        }
        if (unboundedflag == 0) {
            cout << "This equation comes under  unbounded condition" << endl;
            return finalans;
        }
        int degeneratecnt = 0;
        for (int j = 0; j < m; j++) {
            if (simplexgrid[j][pivotcol] <= 0)
                continue;
            float ratiocol = solution[j] / simplexgrid[j][pivotcol];
            if (ratiocol == minratio)
                degeneratecnt++;
        }
        cout << "Min Ratio: " << minratio << endl;
        if (degeneratecnt > 1) {
            cout << "This equation comes under degenerate condition" << endl;
            if (degeneracyhandler(tempsimplexgrid, tempsoln, pivotrow, pivotcol, m, n, xb, minratio) == 0) {
                cout << "Degeneracy cannot be breaked" << endl;
                return finalans;
            }
        }
        cout << "Pivot Row: " << pivotrow + 1 << endl;
        cout << "Pivot Col: " << pivotcol + 1 << endl;
        cout << "Pivot Element: " << simplexgrid[pivotrow][pivotcol] << endl;
        float optimalval = 0;
        for (int i = 0; i < m; i++) optimalval += fn[xb[i]] * solution[i];
        cout << "Optimal Value: " << optimalval << endl;
        cout << endl;
        cout << "Iteration" << itcnt << ":" << endl;
        xb[pivotrow] = pivotcol;
        for (int i = 0; i < (n + extra); i++) {
            tempsimplexgrid[pivotrow][i] = simplexgrid[pivotrow][i] / simplexgrid[pivotrow][pivotcol];
        }
        for (int i = 0; i < m; i++) {
            if (i == pivotrow)
                continue;
            for (int j = 0; j < (n + extra); j++) {
                tempsimplexgrid[i][j] = simplexgrid[i][j] - ((simplexgrid[pivotrow][j] * simplexgrid[i][pivotcol]) / simplexgrid[pivotrow][pivotcol]);
            }
        }
        for (int i = 0; i < m; i++) {
            if (i == pivotrow) {
                tempsoln[i] = solution[i] / simplexgrid[pivotrow][pivotcol];
                continue;
            }
            tempsoln[i] = solution[i] - ((solution[pivotrow] * simplexgrid[i][pivotcol]) / simplexgrid[pivotrow][pivotcol]);
        }
        solution = tempsoln;
        simplexgrid = tempsimplexgrid;
        mindel = INT_MAX;
        minratio = INT_MAX;
        pivotcol = -1;
        breakflag = 0;
        for (int i = 0; i < (n + extra); i++) {
            float sum = 0;
            for (int j = 0; j < m; j++) {
                sum += fn[xb[j]] * simplexgrid[j][i];
            }
            xici[i] = sum;
        }
        for (int i = 0; i < (n + extra); i++) {
            del[i] = xici[i] - fn[i];
            if (del[i] < mindel) {
                mindel = del[i];
                pivotcol = i;
            }
            if (del[i] < 0)
                breakflag = 1;
        }
        if (method == 0)
            printtablesimplex(del, fn, mp, xb, simplexgrid, solution, n, extra, m);
        else
            printtablebigm(del, fn, mp, xb, simplexgrid, solution, n, extra, m);
        cout << "Values are: ";
        for (int i = 0; i < m; i++) cout << mp[xb[i]] << "=" << solution[i] << " ";
        cout << endl;
        if (breakflag == 0) {
            int ss = artificial.size();
            for (int i = 0; i < ss; i++) {
                for (int j = 0; j < m; j++) {
                    if (artificial[i] == xb[j]) {
                        cout << "The solution is infeasible" << endl;
                        return finalans;
                    }
                }
            }
            cout << "We get solution at iteration " << itcnt << endl;
            float optimalval = 0;
            for (int i = 0; i < m; i++) optimalval += fn[xb[i]] * solution[i];
            cout << "Optimal Value: " << optimalval << endl;
            break;
        }
        itcnt++;
    }
    for (int i = 0; i < m; i++)
        if (xb[i] < n)
            finalans[xb[i]] = solution[i];
    return finalans;
    int delzerocnt = 0;
    for (int i = 0; i < n + extra; i++)
        if (del[i] == 0)
            delzerocnt++;
    if (delzerocnt > m) {
        cout << "We get alternate solutions too" << endl;
        cout << "Solutions are:" << endl;
        alternatesolutionshandler(simplexgrid, solution, del, mp, m, n, extra, xb);
    }
}

void solver(vector<vector<float>> &grid, vector<int> &basis, vector<float> &y, map<int, string> &mp, vector<float> &fn, int n, int m, int extra) {
    vector<float> finalans;
    if (artificial.size() > 0) {
        finalans = simplexmethod(grid, basis, y, mp, fn, n, m, extra, 1);
    } else {
        finalans = simplexmethod(grid, basis, y, mp, fn, n, m, extra, 0);
    }
    // return;
    cout << "SOLUTION:";
    for (int i = 0; i < n; i++) cout << finalans[i] << " ";
    cout << endl;
    int gotsolutionflag = 0;
    int notgotintegerflag = 0;
    float maxiofall = INT_MIN;
    int maxiind = -1;
    for (int i = 0; i < n; i++)
        if (finalans[i] != 0)
            gotsolutionflag = 1;
    if (gotsolutionflag == 1) {
        float tempsolutiontocheck = 0;
        for (int i = 0; i < n; i++) {
            tempsolutiontocheck += finalans[i] * fn[i];
            int tempp = floor(finalans[i]);
            if (integervariables.find(i) != integervariables.end() && tempp * 1.0 != finalans[i]) {
                notgotintegerflag = 1;
            }
            if (integervariables.find(i) != integervariables.end() && tempp * 1.0 != finalans[i] && maxiofall < finalans[i]) {
                maxiofall = finalans[i];
                maxiind = i;
            }
        }
        if (tempsolutiontocheck < finalsolutionforbound) {
            return;
        }
        cout << tempsolutiontocheck << " ";
    }

    if (notgotintegerflag == 0 && gotsolutionflag) {
        vector<float> tempfinalans;
        float anstoupdtate = 0;
        cout << "................................................................................................................................" << endl;
        for (int i = 0; i < n; i++) tempfinalans.push_back((finalans[i]));
        for (int i = 0; i < n; i++) {
            anstoupdtate += finalans[i] * fn[i];
            cout << tempfinalans[i] << " ";
            cout << endl;
        }
        if (anstoupdtate > finalsolutionforbound)
            finalsolutionforbound = anstoupdtate;
        globalintsolutions.push_back(tempfinalans);
        return;
    }
    // cout<<"GRID 1:"<<endl;
    // for(int i=0;i<m;i++){
    //     for(int j=0;j<n;j++){
    //         cout<<grid[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }

    if (gotsolutionflag == 0)
        return;
    int lb = floor(maxiofall);
    int ub = ceil(maxiofall);
    // case1
    if (lb >= -0.005) {
        m++;
        grid[m - 1][maxiind] = 1;
        grid[m - 1][n + extra] = 1;
        y[m - 1] = lb;
        slack.push_back(n + extra);
        basis.push_back(n + extra);
        mp[n + extra] = "x" + to_string(n + extra + 1);
        cout << "HI" << endl;
        extra++;
        solver(grid, basis, y, mp, fn, n, m, extra);
        grid[m - 1][maxiind] = 0;
        grid[m - 1][n + extra - 1] = 0;
        m--;
        slack.pop_back();
        basis.pop_back();
        extra--;
    } else {
        m++;
        grid[m - 1][maxiind] = -1;
        grid[m - 1][n + extra] = -1;
        y[m - 1] = abs(lb);
        surplus.push_back(n + extra);
        mp[n + extra] = "x" + to_string(n + extra + 1);
        extra++;
        artificial.push_back(n + extra);
        basis.push_back(n + extra);
        fn[n + extra] = -100000.0;
        mp[n + extra] = "x" + to_string(n + extra + 1);
        grid[m - 1][n + extra] = 1;
        extra++;
        solver(grid, basis, y, mp, fn, n, m, extra);
        grid[m - 1][maxiind] = 0;
        grid[m - 1][n + extra - 2] = 0;
        fn[n + extra - 1] = 0;
        m--;
        surplus.pop_back();
        artificial.pop_back();
        basis.pop_back();
        extra -= 2;
        // grid[m-1][maxiind]=0;
    }
    // cout<<"GRID 2:"<<endl;
    // for(int i=0;i<m;i++){
    //     for(int j=0;j<n;j++){
    //         cout<<grid[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    if (ub >= -0.005) {
        m++;
        grid[m - 1][maxiind] = 1;
        grid[m - 1][n + extra] = -1;
        y[m - 1] = abs(ub);
        surplus.push_back(n + extra);
        mp[n + extra] = "x" + to_string(n + extra + 1);
        extra++;
        artificial.push_back(n + extra);
        basis.push_back(n + extra);
        fn[n + extra] = -100000.0;
        mp[n + extra] = "x" + to_string(n + extra + 1);
        grid[m - 1][n + extra] = 1;
        extra++;
        solver(grid, basis, y, mp, fn, n, m, extra);
        grid[m - 1][maxiind] = 0;
        grid[m - 1][n + extra - 2] = 0;
        fn[n + extra - 1] = 0;
        m--;
        surplus.pop_back();
        artificial.pop_back();
        basis.pop_back();
        extra -= 2;

    } else {
        m++;
        grid[m - 1][maxiind] = -1;
        grid[m - 1][n + extra] = 1;
        y[m - 1] = abs(ub);
        slack.push_back(n + extra);
        basis.push_back(n + extra);
        mp[n + extra] = "x" + to_string(n + extra + 1);
        cout << "HI" << endl;
        extra++;
        solver(grid, basis, y, mp, fn, n, m, extra);
        grid[m - 1][maxiind] = 0;
        grid[m - 1][n + extra - 1] = 0;
        m--;
        slack.pop_back();
        basis.pop_back();
        extra--;
    }
}

int main() {
    int n, m, ss;
    cout << "input the number of variables present at first" << endl;
    cin >> n;
    cout << "input the number of equations given" << endl;
    cin >> m;
    set<int> restrictedvars;
    int restrictedvarsize;
    cout << "how many are unrestricted variable" << endl;
    cin >> restrictedvarsize;
    cout << "Now given me which are unrestricted variable if if there are x2 x3 are unrestricted then give as 2 3" << endl;
    for (int i = 0; i < restrictedvarsize; i++) {
        int x;
        cin >> x;
        x--;
        restrictedvars.insert(x);
    }
    cout << "now give the value in this way" << endl;
    cout << "a11 a12 a13 ....a1n b1 1(if greater than equal to) 0(if equals to) -1(if less than equal to)" << endl;
    cout << "a21 a22 a23 ....a2n b2 1(if greater than equal to) 0(if equals to) -1(if less than equal to)" << endl;
    cout << "....upto m equations" << endl;
    cout << "example 2 3 1 4 1 means 2x1+3x2+1x3>=4 like this you have to give input to m equatiosn" << endl;
    vector<vector<float>> grid(1000, vector<float>(1000, 0));
    map<int, string> mp;
    vector<float> y(1000, 0);
    slack.clear();
    artificial.clear();
    surplus.clear();
    int simplexflag = 0;
    int extra = 0;
    int prevn = n;
    int cntcnt = 0;
    for (int j = 0; j < n; j++) {
        if (restrictedvars.find(j) != restrictedvars.end()) {
            mp[j] = "x" + to_string(cntcnt + 1) + "'";
            mp[j + 1] = "x" + to_string(cntcnt + 1) + "''";
            cntcnt++;
            j++;
            n++;
        } else {
            mp[j] = "x" + to_string(cntcnt + 1);
            cntcnt++;
        }
    }
    for (int i = 0; i < m; i++) {
        int type;
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
            if (restrictedvars.find(j) != restrictedvars.end()) {
                grid[i][j + 1] = -1 * grid[i][j];
                j++;
            }
        }
        cin >> y[i];
        cin >> type;
        if (type == 1) {
            if (y[i] >= 0) {
                simplexflag = 1;
                surplus.push_back(n + extra);
                mp[n + extra] = "x" + to_string(prevn + 1);
                prevn++;
                extra++;
                artificial.push_back(n + extra);
                mp[n + extra] = "x" + to_string(prevn + 1);
                grid[i][n + extra] = 1;
                grid[i][n + extra - 1] = -1;
                prevn++;
                extra++;
            } else {
                slack.push_back(n + extra);
                mp[n + extra] = "x" + to_string(prevn + 1);
                prevn++;
                extra++;
                for (int j = 0; j < n; j++) grid[i][j] *= -1;
                grid[i][n + extra - 1] = 1;
            }
        }
        if (type == -1) {
            if (y[i] >= 0) {
                slack.push_back(n + extra);
                mp[n + extra] = "x" + to_string(prevn + 1);
                prevn++;
                extra++;
                grid[i][n + extra - 1] = 1;
            } else {
                simplexflag = 1;
                surplus.push_back(n + extra);
                mp[n + extra] = "x" + to_string(prevn + 1);
                prevn++;
                extra++;
                for (int j = 0; j < n; j++) grid[i][j] *= -1;
                artificial.push_back(n + extra);
                mp[n + extra] = "x" + to_string(prevn + 1);
                prevn++;
                grid[i][n + extra] = 1;
                grid[i][n + extra - 1] = -1;
                extra++;
            }
        }
        if (type == 0) {
            simplexflag = 1;
            equals.push_back(i);
            if (y[i] < 0) {
                for (int j = 0; j < n; j++) grid[i][j] *= -1;
            }
        }
    }
    int es = equals.size();
    for (int i = 0; i < es; i++) {
        int gotflag = 0;
        for (int j = 0; j < (n); j++) {
            int tempgotflag = 0;
            if (grid[equals[i]][j] == 1) {
                for (int kk = 0; kk < m; kk++) {
                    if (kk != equals[i] && grid[kk][j] != 0) {
                        tempgotflag = 1;
                        break;
                    }
                }
                if (tempgotflag == 0) {
                    gotflag = 1;
                    break;
                }
            }
        }
        if (gotflag == 0) {
            artificial.push_back(n + extra);
            mp[n + extra] = "x" + to_string(prevn + 1);
            prevn++;
            extra++;
            grid[i][n + extra - 1] = 1;
        }
    }
    int maxtype;
    cout << "what is given maximize or minimize enter 1 if maximize or -1 if minimize" << endl;
    cin >> maxtype;
    vector<float> fn(1000, 0);
    cout << "give the coefficients of equations in order of x1 x2 x3 x4......xn" << endl;
    for (int i = 0; i < n; i++) {
        cin >> fn[i];
        if (restrictedvars.find(i) != restrictedvars.end()) {
            fn[i + 1] = -1 * fn[i];
            i++;
        }
    }
    if (maxtype == -1) {
        for (int i = 0; i < n; i++) fn[i] *= -1;
    }
    for (int i = 0; i < m; i++) {
        y[i] = abs(y[i]);
    }
    int intevar;
    cout << "Enter the number of integervariables you want(i.e numbers of variables which you want as integer)" << endl;
    cin >> intevar;
    integervariables.clear();
    cout << "Now enter the variables which you want as integer variables(all 1 indexing in this question)" << endl;
    for (int i = 0; i < intevar; i++) {
        int x;
        cin >> x;
        x--;
        integervariables.insert(x);
    }
    cout << "solution is:" << endl;
    cout << "Maximize ";
    for (int i = 0; i < n; i++) {
        if (fn[i] >= 0)
            cout << "+" << fn[i] << mp[i];
        else
            cout << fn[i] << mp[i];
    }
    cout << "subject to" << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n + extra; j++) {
            if (grid[i][j] >= 0)
                cout << "+" << grid[i][j] << mp[j];
            if (grid[i][j] < 0)
                cout << grid[i][j] << mp[j];
        }
        cout << "=";
        cout << abs(y[i]) << endl;
    }
    for (int i = 0; i < n + extra; i++) {
        cout << mp[i] << ",";
    }
    cout << ">=0" << endl;
    cout << "Slack variables:";
    ss = slack.size();
    for (int i = 0; i < ss; i++) cout << mp[slack[i]] << " ";
    cout << endl;
    cout << "Surplus Variables:";
    ss = surplus.size();
    for (int i = 0; i < ss; i++) cout << mp[surplus[i]] << " ";
    cout << endl;
    cout << "Artificial Variables:";
    ss = artificial.size();
    for (int i = 0; i < ss; i++) cout << mp[artificial[i]] << " ";
    cout << endl;
    cout << "Basis:";
    vector<int> basis;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n + extra; j++) {
            int tempgotflag = 0;
            if (grid[i][j] == 1) {
                for (int k = 0; k < m; k++) {
                    if (k != i && grid[k][j] != 0) {
                        tempgotflag = 1;
                        break;
                    }
                }
                if (tempgotflag == 0) {
                    basis.push_back(j);
                    break;
                }
            }
        }
    }
    cout << endl;
    if (simplexflag == 1) {
        cout << "General Simplex cannot be performed in this case so we are using Big M method" << endl;
        ss = artificial.size();
        for (int i = 0; i < ss; i++) fn[artificial[i]] = -100000.0;
        // simplexmethod(grid,basis,y,mp,fn,n,m,extra,1);
    } else {
        cout << "General Simplex Method Can be used" << endl;
        // simplexmethod(grid,basis,y,mp,fn,n,m,extra,0);
    }
    finalsolutionforbound = INT_MIN;
    solver(grid, basis, y, mp, fn, n, m, extra);
    int ssw = globalintsolutions.size();
    cout << "INTEGER SOLUTIONS:" << endl;
    for (int i = 0; i < ssw; i++) {
        for (int j = 0; j < n; j++) {
            cout << globalintsolutions[i][j] << " ";
        }
        cout << endl;
    }
    cout << "FINAL ANSWER: " << finalsolutionforbound << endl;
    return 0;
}