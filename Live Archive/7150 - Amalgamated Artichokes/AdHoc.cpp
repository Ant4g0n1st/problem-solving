#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

namespace Constants{

    const int P = 6;

};

using namespace Constants;

using std::setprecision;
using std::stringstream;
using std::ios_base;
using std::getline;
using std::string;
using std::vector;
using std::fixed;
using std::cout;
using std::cin;
using std::cos;
using std::sin;
using std::ws;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0),
    cout << fixed,
    cout << setprecision(P);
    while(cin >> ws){ string in;
        if(!getline(cin, in))
            break;
        stringstream st(in);
        double a,b,c,d,p;
        st >> p >> a,
        st >> b >> c,
        st >> d;
        int n; st >> n;
        vector<double> f(++n);
        vector<double> m(n--);
        for(int k = 1; k <= n; k++){
            double z = sin(a * k + b);
            ++z, z += cos(c * k + d),
            ++z, f[k] = m[k] = z * p;
        }
        double max = double();
        bool decline = false;
        for(int k = --n; k > 0; k--){
            if(m[k + 1] < m[k])
                m[k] = m[k + 1],
                decline = true;
            if(max < f[k] - m[k])
                max = f[k] - m[k];
        }
        if(decline)
            cout << max << '\n';
        else 
            cout << double(),
            cout << '\n';
    }
    return 0;
}