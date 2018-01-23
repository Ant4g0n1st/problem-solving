#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::swap;
using std::cin;

typedef long long int Long;
typedef vector<Long> vLong;
typedef vector<int> vInt;

const int S = -1;

struct Tree{

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef vector<Edge> vEdge;

    vector<vEdge> edges;
    vector<vInt> p;
    vLong s;
    int l,n;
    vInt d; 

    Tree(int N): l(), n(N), s(N),
        edges(N, vEdge()), d(N) {
        for(; N; N >>= 1, l++);
    }

    void AddEdge(int u, int v, Long w){
        edges[u].push_back({v, w}),
        edges[v].push_back({u, w});
    }

    void BinaryLifting(int r = 0){
        p.assign(n, vInt(l, S));
        queue<int> q; q.push(r);
        vector<bool> seen(n);
        seen[r] = true; 
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& e : edges[u]){
                if(seen[e.v]) continue;
                s[e.v] = s[u] + e.w,
                d[e.v] = d[u] + 1,
                seen[e.v] = true,
                p[e.v][0] = u,
                q.push(e.v);
            }
        }
        for(int k = 1; k < l; k++)
            for(int u = 0; u < n; u++){
                if(p[u][k - 1] == S) continue;
                p[u][k] = p[p[u][k - 1]][k - 1];
            }
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

    Long Weight(int u, int v){
        return s[u] + s[v] - 2LL * s[LCA(u, v)];
    }

};

int main(){
    ios_base::sync_with_stdio(0), 
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n and n){
        unique_ptr<Tree> t(new Tree(n));
        for(int u = 1; u < n; u++){
            int v; Long w; 
            cin >> v >> w;
            t->AddEdge(u, v, w);
        } 
        t->BinaryLifting();
        int q; cin >> q;
        vLong out(q);
        for(int k = 0; k < q; k++){
            int u,v; cin >> u >> v;
            out[k] = t->Weight(u, v);
        }
        cout << out.front();
        for(int k = 1; k < q; k++)
            cout << ' ' << out[k];
        cout << '\n';
    }
    return 0;
}