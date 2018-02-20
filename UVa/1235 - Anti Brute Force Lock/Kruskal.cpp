#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

using std::numeric_limits;
using std::unique_ptr;
using std::ios_base;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::max;
using std::min;
using std::ws;

typedef vector<int> vInt;

const int oo = numeric_limits<int>::max();
const int M = 10;
const int D = 4;

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

struct Edge{

    int u,v,w;

    Edge(int U, int V, int W): u(U), v(V), w(W) {}

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    vector<vInt> w(M, vInt(M));
    for(int u = 0; u < M; u++)
        for(int v = u + 1; v < M; v++){
            auto& k = w[u][v];
            k = M - v + u;
            if(v - u < k)
                k = v - u;
            w[v][u] = w[u][v];
        }
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        int x,n; cin >> n, x = oo;
        vector<string> keys(n);
        for(auto& k : keys){ 
            int s = 0; cin >> ws >> k;
            for(auto& d : k) d -= '0',
                s += w[0][d];
            x = min(x, s);
        }
        vector<Edge> edges; int c = 0;
        for(int u = 0; u < n; u++)
            for(int v = u + 1; v < n; v++){
                int s = 0;
                for(int k = 0; k < D; k++){
                    auto& a = keys[u][k];
                    auto& b = keys[v][k];
                    s += w[a][b];
                }
                edges.push_back({u, v, s}),
                c = max(c, s);
            }
        vector<bool> used(++c);
        vector<vInt> bucket(c);
        int m = edges.size(), mst = 0;
        for(int k = 0; k < m; k++){
            auto w = edges[k].w;
            auto& b = bucket[w];
            b.push_back(k),
            used[w] = true;
        }
        unique_ptr<DisjointSets> ds;
        ds.reset(new DisjointSets(n));
        for(int k = 0; k < c; k++){
            if(!used[k]) continue;
            for(auto& i : bucket[k]){
                auto u = edges[i].u;
                auto v = edges[i].v;
                if(ds->Joined(u, v))
                    continue;
                ds->Join(u, v),
                mst += k;
            }
        }
        cout << mst + x << '\n';
    }
    return 0;
}