#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using std::numeric_limits;
using std::forward_list;
using std::unique_ptr;
using std::ios_base;
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
    vector<int> s;
    vector<int> t;
    int k,n,p,z;

    Graph(int N): edges(N), n(N) {}

    int FlipCount(){
        m.resize(n), t.resize(n),
        stack.resize(n), p = 0,
        s.resize(n), z = 0;
        for(int u = 0; u < n; u++){
            if(t[u]) continue;
            k = 0, Tarjan(u);
        }
        vector<int> in(z);
        for(int u = 0; u < n; u++)
            for(auto& v : edges[u])
                if(s[u] != s[v])
                    ++in[s[v]];
        auto flips = 0;
        for(auto& d : in)
            if(!d) ++flips;
        return flips;
    }

    void Tarjan(int u){
        m[u] = t[u] = ++k,
        stack[++p] = u;
        for(auto& v : edges[u]){
            if(!t[v]) Tarjan(v);
            if(m[v] < m[u])
                m[u] = m[v];
        }
        if(m[u] == t[u]){
            while(stack[p] != u){
                int v = stack[p];
                s[v] = z, --p,
                m[v] = oo;
            }
            s[u] = z++, --p,
            m[u] = oo;
        }
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 1; y <= z; y++){
        int m,n; cin >> n >> m;
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            g->AddEdge(u, v);
        }
        auto f = g->FlipCount();
        cout << "Case " << y,
        cout << ": " << f,
        cout << '\n';
    }
    return 0;
}