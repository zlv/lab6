#include "PolStr.h"
#include <iostream>
#include <string>
#include <string.h>
#include <cmath>
#include <stdexcept>
#include <vector>
using namespace std;
void solve(double *y, int n, int, double*);
double eval(double *result, int n, double xres);
double x(int i);
int fact(int in);
double *xx;
string s; //e -- even, u -- uneven
double a,b; //– границы отрезка (при равномерной сетке);
double h;
int main(int argc, char **argv) {
    try {
        int k; // 0 – вычисляется сам полином, 1 – его первая производная, 2 – вторая производная
        int n; //порядок полинома
        
        cin >> k >> n;
        n++;
        getline(cin,s);
        getline(cin,s);
        xx = new double[n];
        double *y = new double[n];
        if (s=="e") {
            cin >> a >> b;
            h=abs(a-b)/(n-1);
        }
        else {
            for (int i=0; i<n; i++) {
                cin >> xx[i];
            }
        }
        for (int i=0; i<n; i++) {
            cin >> y[i];
        }
        int m; //количество интервалов в результирующей сетке (т.е. количество узлов – m + 1, что сделано для унификации с узлами исходной сетки);
        cin >> m;
        double *xres = new double[m];
        for (int i=0; i<m; i++) {
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
        double *result = new double[n];
        solve(y,n,k,result);
        double *resnum = new double[m];
        for (int i=0; i<m; i++) {
        
            double resT=eval(result,n,xres[i]);
            if(s=="e")
            {
                resT=eval(result,n,(xres[i]-a)/h);
            }
            resnum[i] = resT;
            
            cout << "x" << i << " P'(" << k << ")(" << xres[i] << ") = " << resT << endl;
        }
        if (t=="y") {
                double *resfuncnum = new double[m];
                double sum = 0;
                for (int i=0; i<m; i++) {
                        resfuncnum[i]=EvalPolStr(sexpr_c,xres[i],k);
                        sum += sqrt(pow(resnum[i]-resfuncnum[i],2));
                }
                sum /= m;
                cout << "Deviation: " << sum << endl;
                delete[] resfuncnum;
        }
        delete[] result;
        delete[] resnum;
        delete[] xx;
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

void solve(double *y, int n, int iDer, double *l) {
    for (int k=0; k<n; k++) {
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
    cout << endl;
}

double eval(double *result, int n, double xres) {
    double sum = 0;
    for (int i=0; i<n; i++) {
        sum += result[i]*pow(xres,n-1-i);
    }
    return sum;
}
double x(int i) {
    if(s=="e")
        return i;
    else
        return xx[i];
}
int fact(int in)
{
  int fact=1;
  for (int c = 1; c <= in; c++)
    fact = fact * c;
  return fact;
}
