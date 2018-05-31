#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::max;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Tree{

    typedef std::forward_list<int> List;

    vector<List> edges;
    vector<int> parent;
    vector<int> size;
    vector<bool> cut;
    vector<int> dc;
    vector<int> dp;
    int n;

    Tree(int N): edges(N), n(N) {}

    int Decompose(int k, int w = 0){
        queue<int> q; q.push(w);
        auto min = Subtree(w);
        auto centroid = w;
        while(!q.empty()){
            auto u = q.front(); q.pop();
            auto s = size[w] - size[u];
            for(auto& v : edges[u]){
                if(cut[v]) continue;
                if(v != parent[u])
                    s = max(s, size[v]),
                    q.push(v);
            }
            if(s < min)
                centroid = u,
                min = s;
        }
        ++dp[dc[centroid] = 0],
        cut[centroid] = true;
        int sum = 0;
        for(auto& v : edges[centroid]){
            if(cut[v]) continue;
            dc[v] = dc[centroid] + 1,
            sum += Paths(v, k),
            AddPaths(v);
        }
        ErasePaths(centroid);
        for(auto& v : edges[centroid])
            if(!cut[v])
                sum += Decompose(k, v);
        return sum;
    }

    int Paths(int u, int k, int p = S){
        int s = 0;
        for(auto& v : edges[u]){
            if(cut[v]) continue;
            if(v == p) continue;
            dc[v] = dc[u] + 1,
            s += Paths(v, k, u);
        }
        if(dc[u] <= k) 
            s += dp[k - dc[u]];
        return s;
    }

    void ErasePaths(int u, int p = S){
        for(auto& v : edges[u]){
            if(cut[v]) continue;
            if(v == p) continue;
            ErasePaths(v, u);
        }
        --dp[dc[u]];
    }

    int LengthKPaths(int k){
        size.resize(n), dc.resize(n),
        cut.resize(n), dp.resize(n),
        parent.resize(n);
        if(n <= k) return int();
        return Decompose(k);
    }

    void AddPaths(int u, int p = S){
        for(auto& v : edges[u]){
            if(cut[v]) continue;
            if(v == p) continue;
            AddPaths(v, u);
        }
        ++dp[dc[u]];
    }

    int Subtree(int u, int p = S){
        parent[u] = p; auto s = 0;
        for(auto& v : edges[u]){
            if(cut[v]) continue;
            if(v == p) continue;
            s += Subtree(v, u);
        }
        return size[u] = ++s;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n,k;
    while(cin >> n >> k){
        std::unique_ptr<Tree> t;
        t.reset(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            t->AddEdge(u, v);
        }
        cout << t->LengthKPaths(k),
        cout << '\n';
    }
    return 0;
}