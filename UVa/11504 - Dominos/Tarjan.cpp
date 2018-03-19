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
    int c,k,n,p;

    Graph(int N): edges(N), n(N) {}

    int KnockDowns(){
        m.resize(n), c = k = 0,
        t.resize(n), p = S,
        stack.resize(n),
        s.resize(n);
        for(int u = 0; u < n; u++)
            if(!t[u]) Tarjan(u);
        vector<int> in(c);
        for(int u = 0; u < n; u++)
            for(int v : edges[u])
                if(s[u] != s[v])
                    ++in[s[v]];
        int knocks = 0;
        for(auto& d : in)
            if(!d) ++knocks;
        return knocks;
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
                m[v] = oo, --p,
                s[v] = c; 
            }
            m[u] = oo, --p,
            s[u] = c++;
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
    for(int y = 0; y < z; y++){
        unique_ptr<Graph> g;
        int n; cin >> n;
        int m; cin >> m;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v; --v;
            g->AddEdge(u, v);
        }
        cout << g->KnockDowns(),
        cout << '\n';
    }
    return 0;
}