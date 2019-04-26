#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const int oo = std::numeric_limits<int>::max();
    const int S = -1;

};

using namespace Constants;

namespace Graphs{

    template<class T>
    struct Tree{

        struct Edge{
            
            int v;
            T w;

            Edge(int V, T W): v(V), w(W) {}
    
        };

        typedef std::forward_list<Edge> List;
        
        vector<List> edges; 
        vector<bool> f;
        const int n;
        T total;

        Tree(int N): edges(N), f(N), n(N), total() {}

        void AddEdge(int u, int v, T w){
            edges[u].emplace_front(v, w),
            edges[v].emplace_front(u, w);
        } 

        T Heaviest(int u, int p = S){
            T max = T(-oo);
            for(auto& e : edges[u]){
                const auto& v = e.v;
                if(v == p) continue;
                T q = Heaviest(v, u); 
                if(q < T()) continue;
                total = total + e.w;
                if(max < q + e.w){
                    max = q + e.w;
                }
            }
            if(max < T()){
                if(f[u] == false){
                    return T(-oo);
                }else{
                    return T(); 
                }
            }
            return max; 
        }

        T MinEnergy(int r = int()){
            T h = Heaviest(r);
            return total - h; 
        }

        void SetFriend(int u){
            f[u] = true;
        }

    };

};

using Graphs::Tree;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int f,n;
    while(cin >> n >> f){
        std::unique_ptr<Tree<int>> t;
        t.reset(new Tree<int>(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            int w; cin >> w;
            t->AddEdge(u, v, w);
        }
        for(int k = 0; k < f; k++){
            int u; cin >> u, --u;
            t->SetFriend(u);
        }
        cout << t->MinEnergy();
        cout << '\n';
    }
    return 0;
}
