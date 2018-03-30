#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::upper_bound;
using std::ios_base;
using std::string;
using std::vector;
using std::cout;
using std::cin;

typedef vector<int> vInt;

namespace Constants{

    const int M = 52;
    const int S = -1;
    const int Z = 26;

};

using namespace Constants;

namespace Util{

    int Index(char alpha){
        if(alpha < 'a')
            return alpha - 'A';
        return Z + alpha - 'a';
    }

};

using namespace Util;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    vector<vInt> groups(M);
    string row; cin >> row;
    int n = row.size();
    for(int k = 0; k < n; k++){
        auto a = Index(row[k]);
        groups[a].push_back(k);
    }
    int q; cin >> q;
    while(q--){
        string s; cin >> s;
        bool match = false;
        int m = s.size(), z = S;
        int k = 0, x = n, y = S;
        for(k = 0; k < m; k++){
            auto a = Index(s[k]);
            auto& g = groups[a];
            auto b = g.begin();
            auto e = g.end();
            auto f = upper_bound(b, e, z);
            if(f == g.end()) 
                break; 
            z = *f;
            if(z < x) x = z;
            if(y < z) y = z;
        }
        if(k != m)
            cout << "Not matched";
        else
            cout << "Matched",
            cout << ' ' << x,
            cout << ' ' << y;
        cout << '\n';
    }
    return 0;
}