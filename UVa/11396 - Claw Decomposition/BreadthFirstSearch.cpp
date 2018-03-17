#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;

struct Graph{

    typedef forward_list<int> List;

    vector<List> edges;
    int n;

    Graph(int N): edges(N), n(N) {}

    bool IsBipartite(){
        vector<bool> seen(n);
        vector<int> c(n);
        for(int w = 0; w < n; w++){
            if(seen[w]) continue;
            seen[w] = true;
            queue<int> q; 
            q.push(w);
            while(!q.empty()){
                auto u = q.front();
                for(auto& v : edges[u]){
                    if(seen[v]){
                        if(c[v] != c[u])
                            continue;
                        return false;
                    }
                    c[v] = c[u] ^ 1,
                    seen[v] = true,
                    q.push(v);
                }
                q.pop();
            }
        }
        return true;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n and n){
        unique_ptr<Graph> g; int u,v;
        g.reset(new Graph(n));
        while(cin >> u >> v)
            if(u and v)
                g->AddEdge(--u, --v);
            else break;
        if(g->IsBipartite())
            cout << "YES";
        else cout << "NO";
        cout << '\n';
    }
    return 0;
}