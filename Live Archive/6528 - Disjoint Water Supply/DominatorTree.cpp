#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::swap;
using std::cin;

namespace Constants{

    const int S = -1;
    
};

using namespace Constants;

namespace Graphs{

    typedef std::forward_list<int> List;

    struct Tree{

        vector<List> edges;
        int n;

        Tree(int N): edges(N), n(N) {}

        int Paths(unsigned int s){
            return (s * (s - 1)) >> 1;
        }

        int Disjoint(int r = int()){
            auto paths = int();
            for(auto& v : edges[r]){
                int s = Subtree(v, r);
                paths += Paths(s);
            }
            return Paths(n) - paths;
        }

        int Subtree(int u, int p){
            unsigned int sub = int();
            for(auto& v : edges[u]){
                if(v == p) continue;
                sub += Subtree(v, u);
            } 
            return ++sub;
        }

        void AddEdge(int u, int v){
            edges[u].push_front(v),
            edges[v].push_front(u);
        }

    };

    struct Graph{
    
        vector<vector<int>> p;
        vector<List> edges;
        vector<List> up;
        vector<int> d;
        const int n;
        int l;

        Graph(int N): edges(N), n(N), up(N) {
            for(l = 0; N; N >>= 1, l++);
        }

        int DisjointSupplies(int s = int()){
            unique_ptr<Tree> t(new Tree(n));
            p.assign(n, vector<int>(l, S)); 
            vector<int> deg(n);
            d.assign(n, int());
            std::queue<int> q;
            for(int u = 0; u < n; u++){
                for(auto& v : edges[u]){
                    deg[v] += 1;
                }
            }
            for(auto& v : edges[s]){
                if(--deg[v] == 0){ 
                    q.push(v);
                }
            }
            while(!q.empty()){
                int u = q.front(); q.pop();
                for(auto& v : edges[u]){
                    if(--deg[v] == 0){
                        q.push(v);
                    }
                }
                auto dom = IDom(u);
                t->AddEdge(u, dom);
                p[u].front() = dom;
                d[u] = d[dom] + 1;
                BinaryLifting(u);
            }
            return t->Disjoint(s);
        }

        int Lift(int u, int h){
            for(int k = 0; h; h >>= 1, k++){
                if((h & 1) == 0) continue;
                if(p[u][k] != S){
                    u = p[u][k];
                }
            }
            return u;
        }

        int LCA(int u, int v){
            if(d[u] < d[v]) swap(u, v);
            u = Lift(u, d[u] - d[v]); 
            if(u == v) return v;
            for(int k = l - 1; k >= 0; k--){
                if(p[u][k] != p[v][k]){
                    u = p[u][k],
                    v = p[v][k];
                }
            }
            return p[v].front();
        }

        void BinaryLifting(int u){
            for(int k = 1; k < l; k++){
                auto& v = p[u][k - 1];
                if(v == S) continue;
                p[u][k] = p[v][k - 1];
            }
        }

        int IDom(int u){
            int lca = up[u].front();
            for(auto& v : up[u]){
                lca = LCA(lca, v); 
            }
            return lca;
        }

        void AddEdge(int u, int v){
            edges[u].push_front(v);
            up[v].push_front(u);
        }

    };

};

using Graphs::Graph;

int main(){
    std::ios_base::sync_with_stdio(0);
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            g->AddEdge(u, v);
        }
        cout << g->DisjointSupplies(),
        cout << '\n';
    }
    return 0;
}
