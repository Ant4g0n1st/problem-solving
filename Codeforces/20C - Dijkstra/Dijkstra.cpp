#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::priority_queue;
using std::forward_list;
using std::vector;
using std::cout;
using std::cin;

namespace Types{

    typedef long long int Long;

};

using namespace Types;

namespace Constants{

    const Long oo = std::numeric_limits<Long>::max();
    const int S = -1;

};

using namespace Constants;

namespace Graphs{

    struct Graph{

        struct Edge{

            Long w;
            int v;

            Edge(int V, Long W): v(V), w(W) {}

        };

        typedef forward_list<Edge> List;

        struct Path : Edge{

            Path(int V, Long W): Edge(V, W) {}

            bool operator<(const Path& other) const{
                if(w == other.w) return v > other.v;
                return w > other.w;
            }

        };
        
        vector<List> edges;
        int n;

        Graph(int N): edges(N), n(N) {}

        forward_list<int> ShortestPath(int s, int t){
            priority_queue<Path> q;
            vector<Long> d(n, oo);
            vector<bool> seen(n);
            vector<int> p(n, S);
            q.emplace(s, d[s] = 0);
            while(!q.empty()){
                const auto u = q.top().v; q.pop();
                if(seen[u]) continue;
                if(u == t) break;
                seen[u] = true;
                for(const auto& e : edges[u]){
                    if(seen[e.v]) continue;
                    const auto& v = e.v;
                    if(d[u] + e.w < d[v]){
                        d[v] = d[u] + e.w;
                        p[v] = u;
                        q.emplace(v, d[v]);
                    }
                }
            }
            forward_list<int> shortest;
            if(d[t] < oo){
                for(int u = t; u != S; u = p[u]){
                    shortest.emplace_front(u);
                }
            }
            return std::move(shortest);
        }

        void AddEdge(int u, int v, Long w){
            edges[u].emplace_front(v, w);
            edges[v].emplace_front(u, w);
        }
        
    };
    
};

namespace Solution{

    using Graphs::Graph;

    void SolveProblem(){
        std::ios_base::sync_with_stdio(0);
        cout.tie(0), cin.tie(0); int n, m;
        while(cin >> n >> m){
            std::unique_ptr<Graph> g;
            g.reset(new Graph(n--));
            for(int k = 0; k < m; k++){
                int u; cin >> u, --u;
                int v; cin >> v, --v;
                Long w; cin >> w;
                g->AddEdge(u, v, w);
            }
            auto p = g->ShortestPath(0, n);
            if(p.empty()){
                cout << S << '\n';
                continue;
            }
            cout << ++p.front();
            p.pop_front();
            for(auto& v : p){
                cout << ' ' << ++v;
            }
            cout << '\n';
        }
    }

};

int main(){
    Solution::SolveProblem();
    return 0;
}