#include <iostream>
#include <limits>
#include <vector>

using std::numeric_limits;
using std::ios_base;
using std::vector;
using std::cout;
using std::cin;

typedef unsigned int Int;

namespace Constants{

    const Int oo = numeric_limits<int>::max();

};

using namespace Constants;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        vector<Int> rung;
        int n; cin >> n;
        rung.resize(n);
        for(auto& r : rung) 
            cin >> r;
        for(int k = --n; k > 0; k--)
            rung[k] -= rung[k - 1];
        auto s = rung.front();
        auto e = oo, k = oo;
        while(s <= e){ 
            Int h = (s + e) >> 1;
            bool ok = true;
            Int x = h; 
            for(auto& r : rung){
                if(x < r){
                    ok = false;
                    break;
                }
                if(r == x) --x;
            }
            if(!ok)
                s = h + 1;
            else
                e = h - 1,
                k = h;
        }
        cout << "Case " << ++y,
        cout << ": " << k,
        cout << '\n', --y;
    }
    return 0;
}