#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::cin;

typedef unsigned int Int;

namespace Constants{

    const int oo = std::numeric_limits<int>::max();

};

using namespace Constants;

struct Tree{

    typedef std::forward_list<int> List;

    vector<bool> client;
    vector<bool> server;
    vector<List> edges;
    vector<int> deg;
    vector<int> dc;
    vector<Int> ds;
    int n;

    Tree(int N): deg(N), edges(N), n(N) {}

    int PlaceReplica(int u, int p, int k){
        if(client[u]) return dc[u] = 0;
        for(const auto& v : edges[u]){
            if(v == p) continue; 
            PlaceReplica(v, u, k);
            if(dc[u] < dc[v] + 1)
                dc[u] = dc[v] + 1;
            if(ds[v] + 1 < ds[u])
                ds[u] = ds[v] + 1;
        } 
        if(dc[u] + ds[u] <= k)
            dc[u] = -oo;
        if(dc[u] == k)
            server[u] = true,
            dc[u] = -oo,
            ds[u] = 0;
        return dc[u];
    }

    int Replicas(int s, int k){
        client.resize(n);
        for(int u = 0; u < n; u++){
            if(--deg[u]) continue;
            client[u] = true;
        }
        ds.assign(n, oo);
        queue<int> q;
        q.push(s), ds[s] = 0;
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(ds[u] + 1 < ds[v])
                    ds[v] = ds[u] + 1,
                    q.push(v);
        }
        dc.assign(n, -oo);
        server.resize(n),
        PlaceReplica(s, s, k);
        auto replicas = 0;
        for(int u = 0; u < n; u++)
            if(server[u]) ++replicas;
        return replicas;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u),
        ++deg[u], ++deg[v];
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        unique_ptr<Tree> t;
        int n; cin >> n;
        int s; cin >> s, --s;
        int r; cin >> r;
        t.reset(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            t->AddEdge(u, v);
        }
        cout << t->Replicas(s, r),
        cout << '\n';
    }
    return 0;
}