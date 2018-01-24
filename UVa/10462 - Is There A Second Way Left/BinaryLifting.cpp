#include <algorithm>
#include <iostream>
#include <memory>
#include <limits>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::sort;
using std::swap;
using std::cin;
using std::max;
using std::min;

typedef long long int Long;
typedef vector<Long> vLong;
typedef vector<int> vInt;

const Long oo = numeric_limits<int>::max();
const int S = -1;

struct DisjointSets{

    vector<int> p;
    vector<int> r;
    int n;

    DisjointSets(int N): n(N), p(N), r(N) {
        for(int u = 0; u < N; u++) p[u] = u;
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

    bool Joined(int u, int v){
        return Root(u) == Root(v);
    }

    int Root(int u){
        if(u == p[u]) return p[u];
        return p[u] = Root(p[u]);
    }

    int SetCount(){
        return n;
    }

};

struct Tree{

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef vector<Edge> vEdge;

    vector<vEdge> edges;
    vector<vLong> m;
    vector<vInt> p;
    vector<int> d;
    int l,n;

    Tree(int N): n(N), d(N), edges(N, vEdge()) {
        for(l = 0; N; N >>= 1, l++);
    }

    void AddEdge(int u, int v, Long w){
        edges[u].push_back({v, w}),
        edges[v].push_back({u, w});
    }

    void BinaryLifting(int root = 0){
        queue<int> q; q.push(root),
        m.assign(n, vLong(l, -oo)),
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

    int Lift(int u, int h, Long& g){ g = -oo; 
        for(int k = 0; h; h >>= 1, k++){
            if(p[u][k] == S) continue;
            if(!(h & 1)) continue;
            g = max(g, m[u][k]),
            u = p[u][k];
        } 
        return u;
    }

    Long Heaviest(int u, int v){ 
        Long mu, mv; int lca = LCA(u, v); 
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
        vector<bool> used(m);
        for(auto& e : edges)
            cin >> e.u, --e.u,
            cin >> e.v, --e.v,
            cin >> e.w;
        unique_ptr<DisjointSets> ds;
        Long mst = 0; int taken = 0; 
        sort(edges.begin(), edges.end());
        unique_ptr<Tree> t(new Tree(n));
        ds.reset(new DisjointSets(n));
        for(int k = 0; k < m; k++){ 
            auto& e = edges[k];
            if(ds->Joined(e.u, e.v)) 
                continue; used[k] = true,
            t->AddEdge(e.u, e.v, e.w),
            mst += e.w, ++taken,
            ds->Join(e.u, e.v);
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
        t->BinaryLifting();
        Long best = oo;
        for(int k = 0; k < m; k++){
            if(used[k]) continue;
            auto& e = edges[k];
            Long d = e.w - t->Heaviest(e.u, e.v);
            best = min(best, mst + d);
        }
        cout << best << '\n';
    }
    return 0;
}