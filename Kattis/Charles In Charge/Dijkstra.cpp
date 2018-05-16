#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>
#include <set>

using std::priority_queue;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;
using std::set;

typedef long long int Long;

namespace Constants{

    const Long oo = std::numeric_limits<Long>::max();
    const Long H = 100;
    const int S = -1;

};

using namespace Constants;

struct Graph{

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef std::forward_list<Edge> List;

    struct Path : Edge{

        Path(int V, Long W): Edge(V, W) {}

        bool operator<(const Path& other) const{
            return w > other.w;
        }

    };

    vector<List> edges;
    vector<bool> seen;
    vector<Long> d;
    set<Long> z;
    int n;

    Graph(int N): edges(N), n(N) {}

    Long ShortestPath(int s, int t, Long max = oo){
        priority_queue<Path> q;
        seen.assign(n, false),
        d.assign(n, oo);
        q.emplace(s, d[s] = 0);
        while(!q.empty()){
            auto u = q.top().v; q.pop();
            if(seen[u]) continue;
            if(u == t) break;
            seen[u] = true;
            for(auto& e : edges[u]){
                if(max < e.w) continue;
                if(seen[e.v]) continue;
                if(d[u] + e.w < d[e.v]){
                    d[e.v] = d[u] + e.w;
                    Path p(e.v, d[e.v]);
                    q.push(p);
                }
            }
        }
        return d[t];
    }

    Long Lightest(int s, int t, Long x){
        vector<Long> w(z.begin(), z.end());
        auto sp = ShortestPath(s, t);
        int a = 0, b = w.size() - 1, k = S;
        auto ub = sp * (x + H);
        while(a <= b){ 
            auto h = (a + b) >> 1;
            auto p = ShortestPath(s, t, w[h]);
            if(p == oo){
                a = h + 1;
                continue;
            }
            if(p * H > ub)
                a = h + 1;
            else 
                b = h - 1,
                k = h;
        }
        return w[k];
    }

    void AddEdge(int u, int v, Long w){
        edges[u].emplace_front(v, w),
        edges[v].emplace_front(u, w),
        z.insert(w);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n,m;
    while(cin >> n >> m){
        unique_ptr<Graph> g;
        Long x; cin >> x;
        g.reset(new Graph(n--));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            Long w; cin >> w;
            g->AddEdge(u, v, w);
        }
        auto h = g->Lightest(0, n, x);
        cout << h << '\n';
    }
    return 0;
}