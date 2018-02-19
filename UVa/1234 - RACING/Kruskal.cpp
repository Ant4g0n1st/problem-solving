#include <iostream>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::cout;
using std::cin;
using std::max;

typedef vector<int> vInt;

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

    Edge(): u(), v(), w() {}

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        int c,m,n; cin >> n >> m, c = 0;
        vector<Edge> edges(m);
        for(auto& e : edges)
            cin >> e.u, --e.u,
            cin >> e.v, --e.v,
            cin >> e.w, c = max(c, e.w);
        vector<bool> used(++c);
        vector<vInt> bucket(c);
        for(int k = 0; k < m; k++){
            auto w = edges[k].w;
            auto& b = bucket[w];
            b.push_back(k),
            used[w] = true;
        }
        unique_ptr<DisjointSets> ds;
        ds.reset(new DisjointSets(n));
        int cost = 0;
        for(int k = --c; k >= 0; k--){
            if(!used[k]) continue;
            for(auto& i : bucket[k]){
                auto u = edges[i].u;
                auto v = edges[i].v;
                if(ds->Joined(u, v)){
                    cost += k;
                    continue;
                }
                ds->Join(u, v);
            }
        }
        cout << cost << '\n';
    }
    return 0;
}