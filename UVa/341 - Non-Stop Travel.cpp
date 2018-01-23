#include <iostream>
#include <limits>
#include <vector>

using std::numeric_limits;
using std::ios_base;
using std::vector;
using std::cout; 
using std::cin;


typedef long long int Long;
typedef vector<Long> vLong;
typedef vector<int> vInt;

const Long oo = numeric_limits<int>::max();

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int k,n; k = 0;
    while(cin >> n and n){
        vector<vLong> g(n, vLong(n, oo));
        for(int u = 0; u < n; u++){
            int m; cin >> m;
            for(int i = 0; i < m; i++){
                int v,w; cin >> v,
                cin >> g[u][--v];
            }
        }
        vector<vInt> p(n, vInt(n));
        for(int u = 0; u < n; u++)
            for(int v = 0; v < n; v++) p[u][v] = u;
        for(int w = 0; w < n; w++)
            for(int u = 0; u < n; u++)
                for(int v = 0; v < n; v++)
                    if(g[u][w] + g[w][v] < g[u][v])
                        g[u][v] = g[u][w] + g[w][v],
                        p[u][v] = p[w][v];
        int s,t,tp,u; cin >> s >> t, --s;
        vInt path(n); tp = 0, u = --t;
        while(u != s) path[tp++] = u,
            u = p[s][u];
        path[tp++] = s;
        cout << "Case " << ++k,
        cout << ": Path =";
        for(--tp; tp >= 0; tp--)
            cout << ' ' << ++path[tp];
        cout << "; " << g[s][t],
        cout << " second delay",
        cout << '\n';
    }
    return 0;
}