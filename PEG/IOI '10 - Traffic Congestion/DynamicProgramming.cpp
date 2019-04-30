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

        typedef std::forward_list<int> List;

        vector<List> edges;
        vector<int> p;
        vector<T> dp;
        vector<T> w;
        const int n;

        Tree(int N): edges(N), n(N), w(N) {}

        int FindArena(int r = int()){
            vector<int> order(n); 
            dp.assign(n, T());
            auto f = int();
            auto b = int();
            p.assign(n, S);
            order[b++] = r;
            while(f < b){
                const auto& u = order[f++];
                for(auto& v : edges[u]){
                    if(v != p[u]){
                        order[b++] = v;
                        p[v] = u;
                    }
                }
            }
            while(f > 0){
                const auto& u = order[--f];
                for(auto& v : edges[u]){
                    if(v != p[u]){
                        dp[u] += dp[v];
                    }
                } 
                dp[u] = dp[u] + w[u];
            }
            auto total = dp[r];
            auto arena = S;
            auto min = oo;
            for(int u = 0; u < n; u++){
                auto max = -oo;
                for(auto& v : edges[u]){
                    if(v == p[u]) continue;
                    if(max < dp[v]){
                        max = dp[v];
                    }
                }
                if(max < total - dp[u]){
                    max = total - dp[u];
                }
                if(max < min){
                    arena = u;
                    min = max;
                }
            }
            return arena;
        }        

        void AddEdge(int u, int v){
            edges[u].push_front(v),
            edges[v].push_front(u);
        }

        void SetWeight(int u, T w){
            this->w[u] = w;
        }

    };

};

using Graphs::Tree;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n; 
    while(cin >> n){
        std::unique_ptr<Tree<int>> t;        
        t.reset(new Tree<int>(n));
        for(int u = 0; u < n; u++){
            int w; cin >> w;
            t->SetWeight(u, w);
        }
        for(int k = 1; k < n; k++){
            int u; cin >> u;
            int v; cin >> v;
            t->AddEdge(u, v);
        }
        cout << t->FindArena();
    }
    return 0;
}
