#include <forward_list>
#include <iostream>
#include <iomanip>
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

namespace Constants{

    const double UB = 24 * 60;
    const double E = 1e-9;
    const int P = 5;

};

using namespace Constants;

namespace Math{

    template<class T>
    struct Line{
    
        T b,m;

        Line(T M, T B): m(M), b(B) {}
        
        T operator()(T t) const{
            return m * t + b;
        }

    };

};

namespace Graphs{

    template<class T>
    struct Graph{

        struct Path{

            int u;
            T w;

            Path(int U, T W): u(U), w(W) {}

            bool operator<(const Path& other) const{
                return w > other.w;
            }

        };

        struct Edge{
            
            Math::Line<T> f;
            int v;

            Edge(int V, T M, T B): f(M, B), v(V) {}

        };

        typedef forward_list<Edge> List;
        
        priority_queue<Path> q;
        vector<List> edges;
        vector<bool> seen;
        vector<T> d;
        int n;

        Graph(int N): edges(N), n(N) {} 

        T ShortestPaths(int s, T t){
            d.assign(n, numeric_limits<T>::infinity());
            q.emplace(s, d[s] = T());
            seen.assign(n, false);
            while(!q.empty()){
                int u = q.top().u; q.pop();
                if(seen[u]) continue;
                seen[u] = true;
                for(auto& e : edges[u]){
                    if(seen[e.v]) continue;
                    const auto v = e.v;
                    const T w = e.f(t); 
                    if(d[u] + w < d[v]){
                        d[v] = d[u] + w;
                        q.emplace(v, d[v]);
                    }
                }
            }
            return d.back();
        }

        T MaxTax(T lb, T ub, T epsilon, int s = int()){
            while(ub - lb > epsilon){
                T l = lb + (ub - lb) / T(3);
                T r = ub - (ub - lb) / T(3);
                T vl = ShortestPaths(s, l);
                T vr = ShortestPaths(s, r);
                if(vl < vr){
                    lb = l; 
                }else{
                    ub = r;
                }
            }
            return ShortestPaths(s, ub);
        }

        void AddEdge(int u, int v, T m, T b){
            edges[u].emplace_front(v, m, b),
            edges[v].emplace_front(u, m, b);
        }
        
    };

};

using Graphs::Graph;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    cout << std::setprecision(P),
    cout << std::fixed;
    while(cin >> n >> m){
        unique_ptr<Graph<double>> g;
        g.reset(new Graph<double>(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            double m; cin >> m;
            double b; cin >> b;
            g->AddEdge(u, v, m, b);
        }
        cout << g->MaxTax(0, UB, E),
        cout << '\n';
    }
    return 0;
}
