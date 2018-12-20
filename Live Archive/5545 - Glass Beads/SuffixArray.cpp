#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::sort;
using std::cin;

namespace Strings{

    int SmallestSuffix(const string& w){
        string dw = w + w;
        int n = dw.size();
        vector<int> rank(n);
        vector<int> sa(n);
        vector<int> z(n);
        for(int k = 0; k < n; k++){
            z[k] = n - k - 1;
        }
        auto f = [&dw] (int u, int v){
            if(dw[u] == dw[v]){ 
                return u < v;
            }
            return dw[u] < dw[v];
        };
        sort(z.begin(), z.end(), f);
        for(int k = 0; k < n; k++){
            rank[k] = dw[k];
            sa[k] = z[k];
        }
        vector<int> r(n);
        vector<int> s(n);
        for(int l = 1; l < n; l <<= 1){
            r = rank, s = sa;
            int h = l >> 1;
            for(int k = 0; k < n; k++){
                rank[sa[k]] = k;
                if(k == 0) continue;
                int u = sa[k - 1];
                int v = sa[k];
                if(r[u] != r[v])
                    continue;
                if(u + l >= n)
                    continue;
                if(r[u + h] != r[v + h])
                    continue;
                rank[v] = rank[u];
            }
            for(int k = 0; k < n; k++){
                z[k] = k;
            }
            for(int k = 0; k < n; k++){
                int x = s[k] - l;
                if(x < 0) continue;
                sa[z[rank[x]]++] = x;
            }
        }
        return sa.front(); 
    }

};

using Strings::SmallestSuffix;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int t; cin >> t;
    for(int k = 0; k < t; k++){
        string w;
        cin >> std::ws;
        cin >> w;
        int s = SmallestSuffix(w); 
        cout << ++s << '\n';
    }
    return 0;
}
