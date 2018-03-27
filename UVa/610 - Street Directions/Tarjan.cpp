#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>

using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::cout;
using std::swap;
using std::cin;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Graph{

    struct TwoWay{

        TwoWay* r;
        bool b;
        int v;

        TwoWay(int V, bool B = false) : v(V), b(B) {}

        void SetReverse(TwoWay* e){ r = e; }

        bool IsBlocked(){ return b; }

        void Block(){ 
            r->b = true, 
            b = true;
        }

    };

    struct OneWay{

        int u,v;

        OneWay(int U, int V): u(U), v(V) {}

        bool operator<(const OneWay& other) const{
            if(u == other.u) return v < other.v;
            return u < other.u;
        }

    };

    typedef forward_list<TwoWay*> List;

    vector<OneWay> directed;
    vector<List> edges;
    vector<bool> seen;
    vector<int> l;
    vector<int> m;
    vector<int> t;
    int k,n;

    Graph(int N): edges(N), n(N) {}

    ~Graph(){
        for(int u = 0; u < n; u++)
            for(auto& e : edges[u])
                if(e) delete e;
    }

    void MakeCycle(int u, int p = S){
        seen[u] = true, l[u] = ++k;
        for(auto& e : edges[u]){
            if(e->IsBlocked()) continue;
            auto& v = e->v;
            if(seen[v]){
                e->Block();
                auto x = u, y = v;
                if(l[x] < l[y])
                    swap(x, y);
                AddDirected(x, y);
                continue;
            }
            e->Block(), MakeCycle(v, u),
            AddDirected(u, v);
        }
    }

    vector<OneWay>& GetDirections(){
        m.resize(n), t.resize(n), k = 0;
        for(int u = 0; u < n; u++)
            if(!t[u]) Tarjan(u);
        l.resize(n), seen.resize(n);
        for(int u = 0; u < n; u++){
            if(seen[u]) continue;
            k = 0, MakeCycle(u);
        }
        return directed;
    }

    void Tarjan(int u, int p = S){
        m[u] = t[u] = ++k;
        for(auto& e : edges[u]){
            auto& v = e->v;
            if(v == p) continue;
            if(!t[v]){ Tarjan(v, u);
                if(t[u] < m[v])
                    AddDirected(u, v),
                    AddDirected(v, u),
                    e->Block();
                if(m[v] < m[u])
                    m[u] = m[v];
            }
            if(t[v] < m[u])
                m[u] = t[v];
        }
    }

    void AddDirected(int u, int v){
        directed.emplace_back(u, v);
    }

    void AddEdge(int u, int v){
        TwoWay* uv = new TwoWay(v);
        TwoWay* vu = new TwoWay(u);
        edges[u].push_front(uv),
        edges[v].push_front(vu),
        uv->SetReverse(vu),
        vu->SetReverse(uv);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int m,n,z; z = 0;
    while(cin >> n >> m){
        if(!n and !m) break;
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            g->AddEdge(u, v);
        }
        auto& st = g->GetDirections();
        cout << ++z << "\n\n";
        for(auto& street : st)
            cout << ++street.u,
            cout << ' ',
            cout << ++street.v,
            cout << '\n';
        cout << "#\n";
    }
    return 0;
}