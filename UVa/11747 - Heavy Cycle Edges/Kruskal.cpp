#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::sort;
using std::cin;

typedef unsigned int Int;

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

    int u,v;
    Int w;

    Edge(): u(), v(), w() {}

    bool operator<(const Edge& other) const{
        return w < other.w;
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        if(!n and !m) break;
        unique_ptr<DisjointSets> ds;
        vector<Edge> edges(m);
        for(auto& e : edges)
            cin >> e.u >> e.v,
            cin >> e.w;
        sort(edges.begin(), edges.end());
        vector<Int> heavy; int taken = 0;
        ds.reset(new DisjointSets(n));
        for(auto& e : edges){
            if(ds->Joined(e.u, e.v)){
                heavy.push_back(e.w);
                continue;
            }
            ds->Join(e.u, e.v),
            ++taken;
        }
        if(taken == m){
            cout << "forest\n";
            continue;
        }
        int h = heavy.size();
        cout << heavy.front();
        for(int k = 1; k < h; k++)
            cout << ' ' << heavy[k];
        cout << '\n';
    }
    return 0;
}