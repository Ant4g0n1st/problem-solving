#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using std::numeric_limits;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::sort;
using std::cout;
using std::cin;
using std::min;

typedef long long int Long;

const Long oo = numeric_limits<int>::max();

struct DisjointSets{

    vector<int> p;
    vector<int> r;
    int n,s;

    DisjointSets(int N): n(N), p(N), r(N), s(N) {
        for(int u = 0; u < N; u++) p[u] = u;
    }

    void Join(int u, int v){
        int Ru = Root(u), Rv = Root(v);
        if(r[Ru] < r[Rv]) p[Ru] = Rv;
        if(r[Rv] < r[Ru]) p[Rv] = Ru;
        if(r[Ru] == r[Rv]){
            if(Rv == Ru) return;
            p[Ru] = Rv, ++r[Rv];
        } --s;
    }

    void Reset(){ s = n;
        for(int u = 0; u < n; u++)
            p[u] = u, r[u] = 0;
    }

    int Root(int u){
        if(u == p[u]) return p[u];
        return p[u] = Root(p[u]);
    }

    bool Joined(int u, int v){
        return Root(u) == Root(v);
    }

    int SetCount(){
        return s;
    }

};

struct Edge{

    int u,v;
    Long w;

    Edge(): u(), v(), w() {}

    Edge(int U, int V, Long W): u(U), v(V), w(W) {}

    bool operator<(const Edge& other) const{
        if(w == other.w){
            if(u == other.u)
                return v < other.v;
            else
                return u < other.u;
        }
        return w < other.w;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int t; cin >> t;
    unique_ptr<DisjointSets> ds;
    for(int c = 1; c <= t; c++){
        int n,m; cin >> n >> m;
        vector<Edge> edges(m);
        vector<bool> used(m);
        for(auto& e : edges)
            cin >> e.u, --e.u,
            cin >> e.v, --e.v,
            cin >> e.w;
        sort(edges.begin(), edges.end());
        ds.reset(new DisjointSets(n));
        int taken = 0; Long mst = 0;
        for(int k = 0; k < m; k++){
            auto& e = edges[k];
            if(ds->Joined(e.u, e.v)) 
                continue; ++taken, 
            ds->Join(e.u, e.v),
            used[k] = true,
            mst += e.w;
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
        Long best = oo;
        for(int k = 0; k < m; k++){
            if(!used[k]) continue;
            Long w = 0; ds->Reset();
            for(int i = 0; i < m; i++){
                if(i == k) continue;
                auto& e = edges[i];
                if(ds->Joined(e.u, e.v)) 
                    continue; w += e.w,
                ds->Join(e.u, e.v);
            }
            if(ds->SetCount() > 1)
                continue;
            best = min(best, w);
        }
        cout << best << '\n';
    }
    return 0;
}