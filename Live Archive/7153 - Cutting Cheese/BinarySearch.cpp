#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using std::ios_base;
using std::vector;
using std::cout;
using std::sort;
using std::cin;
using std::pow;

namespace Constants{

    const double PI = std::acos(-1.0);
    const double E = 1e-9;
    const double Z = 1e-3;
    const double S = 1e5;
    const double P = 9;
    const int B = 28;

};

using namespace Constants;

namespace DataStructures{

    struct Sphere{
        
        double r,z;

        Sphere(): r(), z() {}

        double Volume(double a, double b) const{
            if(!(z + r > a)) return double(); 
            if(!(z - r < b)) return double();
            a = (z - r < a ? a - z : -r);
            b = (z + r > b ? b - z : +r);
            double v = r * r * (b - a);
            double u = pow(b, 3);
            u = u - pow(a, 3);
            u = u / double(3);
            return (v - u) * PI; 
        }

    };

};

using namespace DataStructures;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    cout << std::setprecision(P);
    cout << std::fixed; int n,s;
    while(cin >> n >> s){
        if(n == 0){
            const auto w = S / double(s);
            for(int k = 0; k < s; k++){
                cout << w * Z << '\n';
            }
            continue;
        }
        vector<Sphere> holes(n); 
        double v = double(); 
        for(auto& h : holes){
            cin >> h.r >> h.z,
            cin >> h.z >> h.z;
            v += pow(h.r, 3); 
        }
        v = v * double(4) * PI;
        v = v / double(3);
        v = pow(S, 3) - v;
        v = v / double(s);
        vector<double> slices;
        double lb = double();
        for(int a = 0; a < s; a++){
            double x = double();
            double y = S - lb; 
            for(int b = 0; b < B; b++){
                double k = (x + y) / double(2);
                double vs = S * S * k;
                for(const auto& h : holes){
                    vs -= h.Volume(lb, lb + k); 
                }
                if(vs < v){
                    x = k + E; 
                }else{
                    y = k - E;
                }
            }
            slices.push_back(x);
            lb = lb + x;
        }
        for(const auto& k : slices){
            cout << k * Z << '\n';
        }
    }
    return 0;
}
