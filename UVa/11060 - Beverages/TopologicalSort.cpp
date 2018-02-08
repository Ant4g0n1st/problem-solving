#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <map>

using std::unique_ptr;
using std::ios_base;
using std::string;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::map;
using std::ws;

typedef vector<int> vInt;
typedef __int128 Longest;

const Longest one = Longest(1);

struct Graph{

    vector<Longest> closure;
    vector<Longest> pred;
    vector<vInt> edges;
    vector<int> order;
    int m,n;

    Graph(int N): closure(N), 
        edges(N), n(N), m(), order(N), pred(N) {}

    void TopoSort(){
        for(int u = 0; u < n; u++){
            queue<int> q; q.push(u);
            vector<bool> seen(n);
            seen[u] = true;
            while(!q.empty()){
                auto v = q.front(); q.pop();
                for(auto& w : edges[v]){
                    if(seen[w]) continue;
                    closure[u] |= (one << w),
                    pred[w] |= (one << u),
                    seen[w] = true,
                    q.push(w);
                }
            }
        } TopoSort(Longest());
    }

    void TopoSort(Longest mask){
        if(mask == (one << n) - one) return;
        Longest z = one; int u = 0;  
        for(; u < n; u++, z <<= 1){
            if(mask & z) continue;
            if(mask & closure[u]) continue;
            if(Predecesors(u, mask)) break;
        }
        order[m++] = u, TopoSort(mask | z);
    }

    bool Predecesors(int u, Longest mask){
        return (mask & pred[u]) == pred[u];
    }

    void AddEdge(int u, int v){
        edges[u].push_back(v);
    }

    vector<int>& GetOrder(){
        return order;
    }

};

int main(){
    ios_base::sync_with_stdio(0);
    cout.tie(0), cin.tie(0); 
    int c,n; c = 0;
    while(cin >> n){ int p = 0;
        vector<string> name(n);
        unique_ptr<Graph> g;
        map<string,int> dm; 
        for(auto& b : name)
            cin >> ws >> b, 
            dm[b] = p++;
        g.reset(new Graph(n));
        int m; cin >> m;
        for(int k = 0; k < m; k++){
            string u; cin >> ws >> u;
            string v; cin >> ws >> v;
            g->AddEdge(dm[u], dm[v]);
        }
        g->TopoSort();
        cout << "Case #" << ++c << ": ",
        cout << "Dilbert should drink ",
        cout << "beverages in ",
        cout << "this order:";
        for(auto& u : g->GetOrder())
            cout << ' ' << name[u];
        cout << ".\n\n";
    }
    return 0;
}