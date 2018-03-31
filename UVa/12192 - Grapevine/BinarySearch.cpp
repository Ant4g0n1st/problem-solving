#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::cout;
using std::cin;
using std::map;

typedef map<int,int> Map;
typedef vector<int> vInt;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        if(!n and !m) break;
        vector<vector<int>> q;
        q.assign(n, vInt(m));
        vector<Map> first(n);
        for(int r = 0; r < n; r++){
            auto& mp = first[r];
            for(int c = 0; c < m; c++){
                cin >> q[r][c];
                if(!mp.count(q[r][c]))
                    mp[q[r][c]] = c;
            }
        }
        int z; cin >> z, --n, --m;
        while(z--){
            int min; cin >> min;
            int max; cin >> max;
            int side = S;
            for(int r = 0; r <= n; r++){
                auto& mp = first[r];
                Map::iterator f;
                f = mp.lower_bound(min);
                if(f == mp.end()) 
                    continue;
                int c = f->second;
                int e = n - r;
                if(m - c < e)
                    e = m - c;
                auto b = 0, l = S;
                while(b <= e){
                    int k = (b + e) / 2;
                    int x = r + k;
                    int y = c + k;
                    if(q[x][y] > max)
                        e = k - 1;
                    else
                        b = k + 1,
                        l  = k;
                }
                if(side < l) 
                    side = l;
            }
            cout << ++side,
            cout << '\n';
        }
        cout << "-\n";
    }
    return 0;
}