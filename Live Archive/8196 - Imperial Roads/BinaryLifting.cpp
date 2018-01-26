#include <iostream>
#include <memory>
#include <limits>
#include <vector>
#include <queue>
#include <map>

using std::numeric_limits;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::swap;
using std::cin;
using std::map;
using std::max;

typedef long long int Long;
typedef vector<int> vInt;

const int oo = numeric_limits<int>::max();
const int S = -1;

struct DisjointSets{

    vector<int> p;
    vector<int> r;

    DisjointSets(int N): p(N), r(N) {
        for(int u = 0; u < N; u++) p[u] = u;
    }

    void Join(int u, int v){
        int Ru = Root(u), Rv = Root(v);
        if(r[Ru] < r[Rv]) p[Ru] = Rv;
        if(r[Rv] < r[Ru]) p[Rv] = Ru;
        if(r[Ru] == r[Rv]){
            if(Rv == Ru) return;
            p[Ru] = Rv, ++r[Rv];
        }
    }

    bool Joined(int u, int v){
        return Root(u) == Root(v);
    }

    int Root(int u){
        if(u == p[u]) return p[u];
        return p[u] = Root(p[u]);
    }

};

struct Tree{

    struct Edge{

        int w;
        int v;

        Edge(int V, int W): v(V), w(W) {}

    };

    typedef vector<Edge> vEdge;

    vector<vEdge> edges;
    vector<vInt> m;
    vector<vInt> p;
    vector<int> d;
    int l,n;

    Tree(int N): n(N), d(N), edges(N, vEdge()) {
        for(l = 0; N; N >>= 1, l++);
    }

    void AddEdge(int u, int v, int w){
        edges[u].push_back({v, w}),
        edges[v].push_back({u, w});
    }

    void BinaryLifting(int root = 0){
        queue<int> q; q.push(root),
        m.assign(n, vInt(l, -oo)),
        p.assign(n, vInt(l, S));
        vector<bool> seen(n);
        seen[root] = true;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& e : edges[u]){
                if(seen[e.v]) continue;
                d[e.v] = d[u] + 1,
                seen[e.v] = true,
                m[e.v][0] = e.w,
                p[e.v][0] = u,
                q.push(e.v);
            }
        }
        for(int k = 1; k < l; k++)
            for(int u = 0; u < n; u++){
                if(p[u][k - 1] == S) continue;
                p[u][k] = p[p[u][k - 1]][k - 1];
                m[u][k] = m[p[u][k - 1]][k - 1];
                if(m[u][k] < m[u][k - 1])
                    m[u][k] = m[u][k - 1];
            }
    }

    int Lift(int u, int h, int& g){ g = -oo; 
        for(int k = 0; h; h >>= 1, k++){
            if(p[u][k] == S) continue;
            if(!(h & 1)) continue;
            g = max(g, m[u][k]),
            u = p[u][k];
        } 
        return u;
    }

    int Heaviest(int u, int v){ 
        int mu, mv; int lca = LCA(u, v); 
        Lift(u, d[u] - d[lca], mu),
        Lift(v, d[v] - d[lca], mv);
        return max(mu, mv);
    }

    int Lift(int u, int h){
        for(int k = 0; h; h >>= 1, k++){
            if(!(h & 1)) continue;
            if(p[u][k] != S) u = p[u][k];
        } 
        return u;       
    }

    int LCA(int u, int v){
        if(d[u] < d[v]) swap(u, v);
        u = Lift(u, d[u] - d[v]);
        if(u == v) return v;
        for(int k = l - 1; k >= 0; k--)
            if(p[u][k] != p[v][k])
                u = p[u][k], v = p[v][k];
        return p[u][0];
    }

};

struct Edge{

    int u,v;

    Edge(): u(), v() {}    

    Edge(int U, int V): u(U), v(V) {}

    bool operator<(const Edge& other) const{
        if(u == other.u)
            return v < other.v;
        else 
            return u < other.u;
    }

};

struct WeightedEdge : Edge{

    int w;

    WeightedEdge(): Edge(), w() {}

    WeightedEdge(int U, int V, int W): 
        Edge(U, V), w(W) {}

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n,m;
    while(cin >> n >> m){ int c = 0;
        vector<WeightedEdge> edges(m);
        vector<map<int, int>> w(n);
        for(auto& e : edges)
            cin >> e.u >> e.v >> e.w,
            --e.v, c = max(c, e.w),
            --e.u, w[e.u][e.v] = e.w;
        unique_ptr<DisjointSets> ds;
        Long s = 0; int q; cin >> q;
        vector<vInt> bucket(++c);
        vector<bool> used(c);
        for(int k = 0; k < m; k++){
            auto& a = edges[k].w;
            auto& b = bucket[a];
            b.push_back(k);
            used[a] = true;
        }
        unique_ptr<Tree> t(new Tree(n));
        ds.reset(new DisjointSets(n));
        for(int k = 0; k < c; k++){
            if(!used[k]) continue;
            for(auto& i : bucket[k]){
                auto& e = edges[i];
                if(ds->Joined(e.u, e.v)) 
                    continue; 
                s += e.w, ds->Join(e.u, e.v),
                t->AddEdge(e.u, e.v, e.w);
            }
        }
        t->BinaryLifting();
        for(int k = 0; k < q; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            Long h = t->Heaviest(u, v);
            cout << s - h + w[u][v] << '\n';
        }
    }
    return 0;
}