#include <algorithm>
#include <iostream>
#include <vector>

using std::ios_base;
using std::vector;
using std::cout;
using std::sort;
using std::cin;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int n,q,z; z = 0;
    while(cin >> n >> q){
        if(!n and !q) continue;
        vector<int> marble(n);
        for(auto& m : marble) 
            cin >> m;
        sort(marble.begin(), marble.end());
        cout << "CASE# " << ++z << ":\n";
        for(int k = 0; k < q; k++){
            int e,f,h,s,x; cin >> x,
            e = n - 1, f = S, s = 0;
            while(s <= e){
                h = (s + e) >> 1;
                if(marble[h] < x)
                    s = h + 1;
                else
                    e = h - 1,
                    f = h;
            }
            if(f != S and marble[f] == x)
                cout << x << " found at ",
                cout << ++f;
            else 
                cout << x << " not found";
            cout << '\n';
        }
    }
    return 0;
}