#include <unordered_map>
#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

using std::numeric_limits;
using std::unordered_map;
using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::string;
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
    vector<List> scc;
    vector<int> m;
    vector<int> t;
    int k,n,p;

    Graph(int N): edges(N), n(N) {}

    vector<List>& GetCircles(){
        t.resize(n), k = 0,
        m.resize(n), p = S,
        stack.resize(n);
        for(int u = 0; u < n; u++)
            if(!t[u]) Tarjan(u);
        return scc;
    }

    void Tarjan(int u){
        t[u] = m[u] = ++k,
        stack[++p] = u;
        for(auto& v : edges[u]){
            if(!t[v]) Tarjan(v);
            if(m[v] < m[u])
                m[u] = m[v];
        }
        if(t[u] == m[u]){ List z;
            while(stack[p] != u){
                int v = stack[p];
                z.push_front(v),
                m[v] = oo, --p;
            }
            z.push_front(u), --p,
            scc.push_back(z),
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
    int m,n,z; z = 0;
    while(cin >> n >> m){
        if(!n and !m) break;
        unique_ptr<Graph> g; 
        unordered_map<string,int> mp;
        vector<string> name; int p; 
        g.reset(new Graph(n)), p = 0;
        for(int k = 0; k < m; k++){
            string u; cin >> u;
            if(!mp.count(u))
                name.push_back(u), 
                mp[u] = p++;
            string v; cin >> v;
            if(!mp.count(v))
                name.push_back(v),
                mp[v] = p++;
            g->AddEdge(mp[u], mp[v]);
        }
        auto& s = g->GetCircles();
        if(z) cout << '\n';
        cout << "Calling circles ",
        cout << "for data set ",
        cout << ++z << ":\n";
        for(auto& list : s){
            auto f = list.front();
            list.pop_front(),
            cout << name[f];
            for(auto& u : list) 
                cout << ", ",
                cout << name[u];
            cout << '\n';
        }
    }
    return 0;
}