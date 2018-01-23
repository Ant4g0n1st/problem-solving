#include <iostream>
#include <limits>
#include <vector>

using std::numeric_limits;
using std::ios_base;
using std::vector;
using std::cout;
using std::cin;
using std::max;
using std::min;
using std::ws;

typedef long long int Long; 
typedef vector<Long> vLong;

const Long oo = numeric_limits<int>::max();

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int n; cin >> n;
    vector<vLong> g(n, vLong(n));
    for(int u = 1; u < n; u++)
        for(int v = 0; v < u; v++){
            char c; cin >> ws;
            c = cin.get();
            if(c == 'x'){
                g[u][v] = oo;
            }else{
                cin.unget();
                cin >> g[u][v];
            }
            g[v][u] = g[u][v];
        }
    for(int w = 0; w < n; w++)
        for(int u = 0; u < n; u++)
            for(int v = 0; v < n; v++)
                g[u][v] = min(g[u][v],
                    g[u][w] + g[w][v]);
    Long m = 0;
    for(int u = 0; u < n; u++)
        m = max(m, g[0][u]);
    cout << m << '\n';
    return 0;
}