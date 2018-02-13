#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::cin; 

typedef forward_list<int> List;

const int S = -1;

struct Graph{

    vector<List> edges;
    int n;

    Graph(int N): n(N), edges(N) {}

    int MinimumColored(){
        vector<bool> seen(n);
        vector<int> c(n);
        int minimum = 0;
        for(int w = 0; w < n; w++){
            if(seen[w]) continue;
            queue<int> q; q.push(w);
            int a = 0, b = 0; 
            seen[w] = true;
            while(!q.empty()){
                auto u = q.front();
                for(auto& v : edges[u]){
                    if(seen[v]){
                        if(c[v] != c[u])
                            continue;
                        return S;
                    }
                    c[v] = c[u] ^ 1,
                    seen[v] = true,
                    q.push(v);
                }
                c[u] ? ++b : ++a, 
                q.pop();
            }
            if(!b){
                minimum += a;
                continue;   
            }
            if(a < b) 
                minimum += a;
            else 
                minimum += b;
        }
        return minimum;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int z; cin >> z;
    for(int c = 0; c < z; c++){
        int n; cin >> n;
        int m; cin >> m;
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u,v; cin >> u >> v;
            g->AddEdge(u, v);
        }
        cout << g->MinimumColored();
        cout << '\n';
    }
    return 0;
}