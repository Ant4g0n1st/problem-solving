#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using std::numeric_limits;
using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const int oo = numeric_limits<int>::max();
    const int S = -1;

};

using namespace Constants;

struct Graph{

    typedef forward_list<int> List;

    vector<List> edges;
    vector<int> stack;
    vector<int> m;
    vector<int> t;
    int c,k,n,p;

    Graph(int N): edges(N), n(N) {}

    int StronglyConnected(){
        m.resize(n), c = k = 0,
        t.resize(n), p = S,
        stack.resize(n);
        for(int u = 0; u < n; u++)
            if(!t[u]) Tarjan(u);
        return c;
    }

    void Tarjan(int u){
        m[u] = t[u] = ++k,
        stack[++p] = u;
        for(auto& v : edges[u]){
            if(!t[v]) Tarjan(v);
            if(m[v] < m[u])
                m[u] = m[v];
        }
        if(m[u] == t[u]){ ++c;
            while(stack[p] != u){
                int v = stack[p];
                m[v] = oo, --p;
            }
            m[stack[p--]] = oo;
        }
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        if(!n and !m) break;
        unique_ptr<Graph> g; int c;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            int w; cin >> w, --w;
            g->AddEdge(u, v);
            if(!w) continue;
            g->AddEdge(v, u);
        }
        c = g->StronglyConnected();
        cout << (--c ? '0' : '1'),
        cout << '\n';
    }
    return 0;
}