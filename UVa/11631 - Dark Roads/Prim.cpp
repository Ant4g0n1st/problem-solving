#include <forward_list>
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::priority_queue;
using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;

typedef long long int Long;

const Long oo = numeric_limits<unsigned int>::max();

struct Graph{

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

        bool operator<(const Edge& other) const{
            return w > other.w;
        }

    };

    typedef forward_list<Edge> List;

    vector<List> edges;
    int n;

    Graph(int N): edges(N), n(N) {}

    Long MST(){ Long mst = 0;
        vector<bool> seen(n);
        vector<Long> w(n, oo);
        for(int z = 0; z < n; z++){
            if(seen[z]) continue;
            priority_queue<Edge> q;
            q.push(Edge(z, 0));
            while(!q.empty()){
                auto r = q.top().w;
                auto u = q.top().v; q.pop();
                if(seen[u]) continue;
                seen[u] = true, mst += r;
                for(auto& e : edges[u]){
                    if(seen[e.v]) continue;
                    if(e.w < w[e.v])
                        w[e.v] = e.w,
                        q.push(e);
                }
            }
        }
        return mst;
    }

    void AddEdge(int u, int v, Long w){
        edges[u].emplace_front(v, w),
        edges[v].emplace_front(u, w);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        if(!n and !m) break;
        unique_ptr<Graph> g;
        Long total = 0;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u,v; cin >> u >> v;
            Long w; cin >> w;
            total += w,
            g->AddEdge(u, v, w);
        }
        cout << total - g->MST(),
        cout << '\n';
    }
    return 0;
}