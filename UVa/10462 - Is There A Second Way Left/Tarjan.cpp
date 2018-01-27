#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::sort;
using std::cout;
using std::cin;
using std::max;
using std::min;

typedef long long int Long;
typedef vector<int> vInt;

const Long oo = numeric_limits<int>::max();
const int S = -1;

struct DisjointSets{

    vector<int> p;
    vector<int> r;
    int n;

    DisjointSets(int N): n(N), p(N), r(N) {
        for(int u = 0; u < n; u++) p[u] = u;
    }

    void Join(int u, int v){
        int Ru = Root(u), Rv = Root(v);
        if(r[Ru] < r[Rv]) p[Ru] = Rv;
        if(r[Rv] < r[Ru]) p[Rv] = Ru;
        if(r[Ru] == r[Rv]){
            if(Rv == Ru) return;
            p[Ru] = Rv, ++r[Rv];
        } --n;
    }

    int Root(int u){
        if(u == p[u]) return p[u];
        return p[u] = Root(p[u]);
    }

    bool Joined(int u, int v){
        return Root(u) == Root(v);
    }

    int SetCount(){
        return n;
    }

};

struct WeightedSets : DisjointSets{

    vector<Long> w;

    WeightedSets(int N): DisjointSets(N), w(N){}

    void SetWeight(int u, Long m){ w[u] = m; }

    void SetRank(int u, int d){ r[u] = d; }

    void Join(int u, int v){
        int Ru = Root(u), Rv = Root(v);
        if(r[Ru] < r[Rv]) p[Rv] = Ru;
        if(r[Rv] < r[Ru]) p[Ru] = Rv;
    }

    int Root(int u, Long& m){ 
        if(u == p[u]) return u;
        Long n = -oo; 
        p[u] = Root(p[u], n),
        w[u] = max(w[u], n),
        m = w[u];
        return p[u];
    }

    int Root(int u){ Long m;
        return Root(u, m);
    }

    Long GetWeight(int u){ 
        return w[u];
    }

};

struct Tree{

    struct Edge{

        Long w;
        int v; 

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef vector<Edge> vEdge;

    struct Query{

        int u,v;

        Query(int U, int V): u(U), v(V) {}

    };

    unique_ptr<WeightedSets> ws;
    vector<Query> queries;
    vector<vEdge> edges;
    vector<vInt> queued;
    vector<Long> result;
    vector<bool> seen;
    vector<vInt> idx;
    vector<int> d;
    vector<int> a;
    int q,n;

    Tree(int N): a(N), n(N),
        d(N), seen(N), edges(N, vEdge()),
        queued(N, vInt()), idx(N, vInt()), q(S) {}

    void Tarjan(int u, int p = S){
        for(auto& e : edges[u]){
            if(e.v == p) continue;
            auto& v = e.v;
            d[v] = d[u] + 1,
            ws->SetRank(v, d[v]),
            ws->SetWeight(v, e.w),
            Tarjan(v, u),
            ws->Join(v, u),
            a[ws->Root(u)] = u;
        } seen[u] = true;
        for(auto& k : idx[u]){
            auto& e = queries[k];
            int v = e.u == u ? e.v : e.u;
            if(!seen[v]) continue;
            auto& lca = a[ws->Root(v)];
            queued[lca].push_back(k);
        }
        for(auto& k : queued[u]){
            auto& e = queries[k];
            ws->Root(e.u), ws->Root(e.v);
            auto& w = result[k]; w = -oo;
            if(e.u != u)
                w = max(w, ws->GetWeight(e.u));
            if(e.v != u)
                w = max(w, ws->GetWeight(e.v));
        }
    }

    void AddEdge(int u, int v, Long w){
        edges[u].emplace_back(v, w),
        edges[v].emplace_back(u, w);
    }

    void ProcessQueries(int r = 0){
        ws.reset(new WeightedSets(n));
        result.resize(++q); Tarjan(r);
    }

    void AddQuery(int u, int v){ ++q;
        queries.emplace_back(u, v),
        idx[u].push_back(q),
        idx[v].push_back(q);
    }

    Long GetResult(int qi){
        return result[qi];
    }

};

struct Edge{

    int u,v;
    Long w;

    Edge(): u(), v(), w() {}

    Edge(int U, int V, Long W): u(U), v(V), w(W) {}

    bool operator<(const Edge& other) const{
        return w < other.w;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int t; cin >> t;
    for(int c = 1; c <= t; c++){
        int n,m; cin >> n >> m;
        vector<Edge> edges(m);
        vector<int> queries;
        for(auto& e : edges)
            cin >> e.u, --e.u,
            cin >> e.v, --e.v,
            cin >> e.w;
        unique_ptr<DisjointSets> ds;
        int taken = 0; Long mst = 0; 
        sort(edges.begin(), edges.end());
        unique_ptr<Tree> t(new Tree(n));
        ds.reset(new DisjointSets(n));
        for(int k = 0; k < m; k++){
            auto& u = edges[k].u;
            auto& v = edges[k].v;
            if(ds->Joined(u, v)){
                queries.push_back(k),
                t->AddQuery(u, v);
                continue;
            } 
            auto& w = edges[k].w;
            t->AddEdge(u, v, w),
            mst += w, ++taken;
            ds->Join(u, v);
        }
        cout << "Case #" << c << " : ";
        if(ds->SetCount() > 1){
            cout << "No way\n";
            continue;
        }
        if(taken == m){
            cout << "No second way\n";
            continue;
        }
        int q = queries.size();
        t->ProcessQueries();
        Long best = oo;
        for(int k = 0; k < q; k++){
            auto& e = edges[queries[k]];
            Long w = e.w - t->GetResult(k);
            best = min(best, mst + w);
        }
        cout << best << '\n';
    }
    return 0; 
}