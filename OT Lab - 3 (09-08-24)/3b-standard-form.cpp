#include <bits/stdc++.h>
using namespace std;

struct Constraint {
    vector<double> coefficients;
    double rhs;
    char type; // '<' for <=, '>' for >=, '=' for =
};

void convertToStandardForm(vector<double> &objective, vector<Constraint> &constraints, bool minimise, vector<bool> restricted){
    if(minimise){
        for(auto &coef:objective){ 
            coef = -coef;
        }
    }

    int varCnt = objective.size();
    int constraintsCnt = constraints.size();

    for(int i=0; i<restricted.size(); i++){
        if(!restricted[i]){
            cout << "x" << i+1 << " is unrestricted" << endl;
        }
    }

    vector<double> slackSurplusArtificial;

    cout << "Standard form: " << endl;
    cout << "Maximize: ";
    for(int i=0; i<varCnt; i++){
        if(restricted[i])
            cout << (objective[i]>=0 ? "+ " : "- ") << abs(objective[i]) << "x" << i+1 << " ";
        else
            cout << (objective[i]>=0 ? "+ " : "- ") << abs(objective[i]) << "x" << i+1 << "a " << (objective[i]<0 ? "+ " : "- ") << abs(objective[i]) << "x" << i+1 << "b ";
    }
    cout << endl;

    cout << "Subject to:" << endl;
    for(int i=0; i<constraintsCnt; i++){
        if(constraints[i].rhs < 0){
            for(auto &coef:constraints[i].coefficients){
                coef = -coef;
            }
            constraints[i].rhs = -constraints[i].rhs;
            constraints[i].type = (constraints[i].type == '<') ? '>' : '<';
        }

        for(int j=0; j<varCnt; j++){
            if(restricted[j])
                cout << (constraints[i].coefficients[j] >= 0 ? "+ " : "- ") << abs(constraints[i].coefficients[j]) << "x" << j+1 << " ";
            else
                cout << (constraints[i].coefficients[j] >= 0 ? "+ " : "- ") << abs(constraints[i].coefficients[j]) << "x" << j+1 << "a " << (constraints[i].coefficients[j] < 0 ? "+ " : "- ") << abs(constraints[i].coefficients[j]) << "x" << j+1 << "b ";
        }

        if(constraints[i].type == '<'){
            cout << "+ s" << i+1;
            slackSurplusArtificial.push_back(1);
        }
        else if(constraints[i].type == '>'){
            cout << "- s" << i+1 << " + a" << i+1;
            slackSurplusArtificial.push_back(-1);
        }
        else { // =
            cout << "+ a" << i+1;
            slackSurplusArtificial.push_back(0);
        }

        cout << " = " << constraints[i].rhs << endl;
    }

    cout << "Basis: ";
    for(int i=0; i<slackSurplusArtificial.size(); i++){
        if(slackSurplusArtificial[i] == 1){
            cout << "s" << i+1 << " ";
        } else if(slackSurplusArtificial[i] == -1){
            cout << "a" << i+1 << " ";
        }
    }
    cout << endl;
}

int main(){
    // vector<double> objective = {3, 2, -4, -1};

    // vector<Constraint> constraints = {
    //     {{3, -1, 2, -5}, -10, '>'},
    //     {{3, 2, -1, 1}, 4, '<'},
    //     {{3, 2, -3, 5}, 5, '='}
    // };

    // bool minimise = false;

    // vector<bool> restricted = {true, true, true, true};

    vector<double> objective = {2, -3, -3, -1};

    vector<Constraint> constraints = {
        {{2, 1, -3, 1}, 5, '>'},
        {{3, -2, 1, 1}, -3, '='},
        {{1, 3, 2, 2}, 2, '<'}
    };

    bool minimise = true;

    vector<bool> restricted = {true, true, true, false};

    convertToStandardForm(objective, constraints, minimise, restricted);

    return 0;
}