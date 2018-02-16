#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::sort;
using std::cin;

typedef long long int Long;

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
    Long w;

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
        vector<Edge> roads(m);
        for(auto& e : roads)
            cin >> e.u >> e.v >> e.w;
        ds.reset(new DisjointSets(n));
        sort(roads.begin(), roads.end());
        Long savings = 0;
        for(auto& e : roads){
            if(ds->Joined(e.u, e.v)){
                savings += e.w;
                continue;
            }
            ds->Join(e.u, e.v);
        }
        cout << savings,
        cout << '\n';
    }
    return 0;
}