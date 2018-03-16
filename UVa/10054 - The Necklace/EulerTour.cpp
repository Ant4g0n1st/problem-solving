#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <list>
#include <map>

using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::list;
using std::cin;
using std::map;

namespace Constants{

    const int T = 2;

};

using namespace Constants;

struct Graph{

    struct Edge{

        Edge* r;
        bool b;
        int v;

        Edge(int V, bool B = false): v(V), b(B) {}

        void SetReverse(Edge* e){ r = e; }

        bool Blocked(){ return b; }

        void Block(){ b = true; }

    };

    typedef list<int>::iterator Iterator;
    typedef vector<Edge*> vEdge;

    vector<vEdge> edges;
    vector<int> deg;
    vector<int> ptr;
    list<int> tour;
    int n;

    Graph(int N): deg(N), edges(N), n(N), ptr(N) {}

    ~Graph(){
        for(int u = 0; u < n; u++)
            for(auto& e : edges[u])
                if(e) delete e;
    }

    bool IsConnected(int s = 0){
        queue<int> q; int m = 0;
        vector<bool> seen(n);
        seen[s] = true, 
        q.push(s);
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& e : edges[u]){
                if(seen[e->v]) continue;
                seen[e->v] = true,
                q.push(e->v), ++m;
            }
        }
        return ++m == n;
    }

    void MakeTour(int u, Iterator it){
        auto& k = ptr[u];
        for(; k < deg[u]; k++){
            auto& e = edges[u][k];
            if(e->Blocked()) continue;
            e->Block(), e->r->Block();
            Iterator t;
            t = tour.insert(it, e->v),
            MakeTour(e->v, t);
        }
    }

    list<int>& EulerTour(int s = 0){
        MakeTour(s, tour.begin());
        return tour;
    }

    void AddEdge(int u, int v){
        auto uv = new Edge(v);
        auto vu = new Edge(u);
        edges[u].push_back(uv),
        edges[v].push_back(vu),
        uv->SetReverse(vu),
        vu->SetReverse(uv),
        ++deg[u], ++deg[v];
    }

    bool IsEulerianCycle(){
        int odd = 0;
        for(auto& k : deg)
            odd += k & 1;
        return !odd;
    }

};

struct Edge{

    int u,v;

    Edge(): u(), v() {}

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        unique_ptr<Graph> g;
        vector<Edge> beads;
        int m; cin >> m;
        map<int,int> mp;
        beads.resize(m);
        vector<int> c;
        int n = 0;
        for(auto& b : beads){
            int u; cin >> u;
            int v; cin >> v;
            if(!mp.count(u))
                c.push_back(u),
                mp[u] = n++;
            if(!mp.count(v))
                c.push_back(v),
                mp[v] = n++;
            b.u = mp[u], 
            b.v = mp[v];
        }
        g.reset(new Graph(n));
        for(auto& b : beads)
            g->AddEdge(b.u, b.v);
        if(y) cout << '\n';
        cout << "Case #" << ++y,
        cout << '\n', --y;
        bool p = g->IsConnected();
        p &= g->IsEulerianCycle();
        if(!p){
            cout << "some beads",
            cout << " may be",
            cout << " lost\n";
            continue;
        }
        auto& t = g->EulerTour();
        auto s = t.front();
        auto u = t.front();
        t.pop_front();
        for(auto& v : t)
            cout << c[u] << ' ',
            cout << c[u = v],
            cout << '\n';
        cout << c[t.back()],
        cout << ' ' << c[s],
        cout << '\n';
    }
    return 0;
}