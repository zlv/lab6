#include "PolStr.h"
#include <iostream>
#include <string>
#include <string.h>
#include <cmath>
#include <stdexcept>
#include <vector>
using namespace std;
void solve(double *x, double *y, int n, int k, double **l);
double eval(double *x, double **result, int n, int k, double xres);
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
        
            double resT=eval(x,result,n,k,xres[i]);
            resnum[i] = resT;
            
            cout << "x" << i << " S" << "(" << xres[i] << ") = " << resT << endl;
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
        for (int i=0; i<n; i++) {
            delete[] result[i];
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

void solve(double *x, double *y, int n, int k, double **l) {
    if (k==1) {
        for (int i=0; i<n; i++) {
            double a = y[i];
            double b = (y[i+1]-y[i])/(x[i+1]-x[i]);
            l[i][0] = b;
            l[i][1] = a-b*x[i];
            cout << a << ' ' << b << endl;
        }
    }
}

double eval(double *x, double **result, int n, int k, double xres) {
    int i = 0;
    for (; i<n; i++) {
        if (xres<x[i+1] || i==n-1)
            break;
    }
    int index_in_spline = i;
    cerr << "index in spline : " << i << endl;
    double sum = 0;
    for (i=0; i<k+1; i++) {
        sum += result[index_in_spline][i]*pow(xres,k-i);
    }
    return sum;
}
