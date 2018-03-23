#include <iostream>
#include <limits>
#include <vector>

using std::numeric_limits;
using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const int oo = numeric_limits<int>::max();
    const int S = -1;

};

using namespace Constants;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n,t; 
    while(cin >> t >> n){
        vector<int> year(n);
        for(auto& y : year) cin >> y;
        int e,s; e = n - 1, s = 0;
        int l,r; l = r = 0;
        while(s <= e){
            int h = (s + e) >> 1;
            int a,b; a = b = S;
            for(int k = 0; k < n; k++){
                if(k + h >= n) break;
                auto y = year[k + h];
                auto x = year[k];
                if(y - x + 1 <= t){
                    a = k, b = a + h;
                    break;
                }
            }
            if(a != S and b != S)
                l = a, r = b,
                s = h + 1;
            else 
                e = h - 1;
        }
        cout << r - l + 1 << ' ',
        cout << year[l] << ' ',
        cout << year[r],
        cout << '\n';
    }
    return 0;
}