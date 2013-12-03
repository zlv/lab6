#include "PolStr.h"
#include <iostream>
#include <string>
#include <string.h>
#include <cmath>
#include <stdexcept>
#include <vector>
using namespace std;
void solve(double*, double *y, int n, int, double**);
double eval(double *result, int n, double xres);
double a,b; //– границы отрезка (при равномерной сетке);
double h;
int main(int argc, char **argv) {
    try {
        int k; // порядок сплайна (1 – линейный, 2 – параболический, 3 – кубический)
        int n; //количество сплайнов
        
        cin >> k >> n;
        double *x = new double[n+1];
        double *y = new double[n+1];
        for (int i=0; i<n+1; i++) {
            cin >> x[i];
        }
        for (int i=0; i<n+1; i++) {
            cin >> y[i];
        }
        int m; //количество интервалов в результирующей сетке (т.е. количество узлов – m + 1, что сделано для унификации с узлами исходной сетки);
        cin >> m;
        double *xres = new double[m+1];
        for (int i=0; i<m+1; i++) {
            cin >> xres[i];
        }
        string t; //'y' -- expression known, 'n' -- otherwise
        getline(cin,t);
        getline(cin,t);
        string sexpr;
        char* sexpr_c = new char[1024+1];
        if (t=="y") {
            getline(cin,sexpr);
            strcpy(sexpr_c,sexpr.c_str());
            sexpr_c=CreatePolStr(sexpr_c,0);
            
        }
        double **result = new double*[n];
        for (int i=0; i<n; i++) {
            result[i] = new double[k+1];
        }
        solve(x,y,n,k,result);
        double *resnum = new double[m+1];
        for (int i=0; i<m+1; i++) {
        
            double resT=eval(result,n,xres[i]);
            resnum[i] = resT;
            
            cout << "x" << i << " P" << "(" << xres[i] << ") = " << resT << endl;
        }
        if (t=="y") {
                double *resfuncnum = new double[m+1];
                double sum = 0;
                for (int i=0; i<m+1; i++) {
                        resfuncnum[i]=EvalPolStr(sexpr_c,xres[i],k);
                        sum += sqrt(pow(resnum[i]-resfuncnum[i],2));
                }
                sum /= m+1;
                cout << "Deviation: " << sum << endl;
                delete[] resfuncnum;
        }
        delete[] result;
        delete[] resnum;
        delete[] x;
        delete[] y;
        delete[] xres;
    }
    catch (invalid_argument &e) {
        cerr << e.what() << endl;
    }
            
    return 0;
}

double findMultValue(int n, vector<int> &indexes, int k) {
    if (k==0) {
        return 1;
    }
    bool *aUsing = new bool[n];
    for (int i=0; i<n; i++) {
        aUsing[i] = 1;
    }
    for (vector<int>::iterator iter=indexes.begin(); iter!=indexes.end(); ++iter) {
        aUsing[*iter] = 0;
    }
    if (k==1) {
        double sum = 0;
        for (int i=0; i<n; i++) {
            if (aUsing[i]) {
                sum -= x(i);
            }
        }
        delete[] aUsing;
        return sum;
    }
    else {
        int iusing = 0;
        double sum = 0;
        int count = 0;
        int inum = n-indexes.size();
        for (int i=0; i<inum; i++) {
            while (!aUsing[iusing]) ++iusing;
            indexes.push_back(iusing);
            sum -= x(iusing)*findMultValue(n,indexes,k-1);
            ++iusing;
            ++count;
        }
        indexes.erase(indexes.end()-count,indexes.end());
        return sum;
    }
}

void solve(double *x, double *y, int n, int k, double *l) {
    if (k==1) {
        for (int i=0; i<n; i++) {
            l[i][1] = (y[i+1]-y[i])/(x[i+1]-x[i]);
            l[i][0] = y[i]-l[i][1]*x[i];
        }
    }
    return;
    /*for (int k=0; k<n; k++) {
        l[k] = 0;
    }
    for (int i=0; i<n; i++) {
        double denom = 1;
        
        if(s!="e")
        {
                for (int j=0; j<n; j++) {
                    if (i!=j) {
                        denom *= x(i)-x(j);
                    }
                }
        }
        else
        {
                denom =fact(i)*fact(n-i-1)/pow(-1,n-i-1);
        }
        double mult = y[i]/denom;
        vector<int> indexes; //исключаемые индексы
        indexes.push_back(i);
        if (iDer==0) {
            for (int k=0; k<n; k++) {
                l[k] += mult*findMultValue(n,indexes,k);
            }
        }
        else if (iDer==1) {
            for (int j=0; j<n; j++) {
                if (j==i) continue;
                int il = 0;
                indexes.push_back(j);
                for (int k=0; k<n; k++) {
                    if (k!=j) {
                        l[il+iDer] += mult*findMultValue(n,indexes,il);
                        ++il;
                    }
                }
                indexes.pop_back();
            }
        }
        else if (iDer==2) {
            for (int m=0; m<n; m++) {
                if (m==i) continue;
                indexes.push_back(m);
                for (int j=0; j<n; j++) {
                    if (j==i || j==m) continue;
                    int il = 0;
                    indexes.push_back(j);
                    for (int k=0; k<n; k++) {
                        if (k!=j && k!=m) {
                            l[il+iDer] += mult*findMultValue(n,indexes,il);
                            ++il;
                        }
                    }
                    indexes.pop_back();
                }
                indexes.pop_back();
            }
        }
    }
    for (int i=0; i<n; i++) {
        cout << l[i] <<' ';
    }
    cout << endl;*/
}

double eval(double *result, int n, double xres) {
    double sum = 0;
    for (int i=0; i<n; i++) {
        sum += result[i]*pow(xres,n-1-i);
    }
    return sum;
}
