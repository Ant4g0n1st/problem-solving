#include <algorithm>
#include <iostream>
#include <numeric>
#include <memory>
#include <vector>

using std::vector;
using std::cout;
using std::sort;
using std::cin;

namespace Types{

    typedef long long int Long;

};

using namespace Types;

namespace DataStructures{

    struct DisjointSets{

        vector<int> p;

        DisjointSets(int N): p(N) {
            std::iota(p.begin(), p.end(), int());
        }

        int Root(const int u){
            if(u == p[u]) return p[u];
            return p[u] = Root(p[u]);
        }

    };

};

namespace Graphs{

    using DataStructures::DisjointSets;

    template<class T>
    struct Tree{

        struct Edge{

            int u;
            int v;
            T w;

            Edge(int U, int V, T W): 
                u(U), v(V), w(W) {}

            bool operator<(const Edge& other) const{
                return w > other.w;
            }
    
        };

        struct WeightedSets : public DisjointSets{
    
            vector<T> s;
            vector<T> w;

            WeightedSets(int N): 
                DisjointSets(N), s(N, 1), w(N) {}

            void Join(int u, int v, T x){
                u = Root(u), v = Root(v);
                auto m = s[v] * x + w[u];
                if(m < s[u] * x + w[v]){
                    m = s[u] * x + w[v];
                }
                w[p[v] = u] = m;
                s[p[v]] += s[v];
            }

            T MaxWeight(int u = int()){
                return w[Root(u)];
            }

        };

        vector<Edge> edges;
        const int n;

        Tree(int N): edges(), n(N) {}

        T MaxTraffic(){
            sort(edges.begin(), edges.end());
            std::unique_ptr<WeightedSets> ws;
            ws.reset(new WeightedSets(n));
            for(const auto& e : edges){
                ws->Join(e.u, e.v, e.w);
            } 
            return ws->MaxWeight();
        }

        void AddEdge(int u, int v, T w){
            edges.emplace_back(u, v, w);
        }

    };

};

using Graphs::Tree;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        std::unique_ptr<Tree<Long>> t;
        t.reset(new Tree<Long>(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            Long w; cin >> w;
            t->AddEdge(u, v, w);
        }
        cout << t->MaxTraffic();
        cout << '\n';
    }
    return 0;
}
