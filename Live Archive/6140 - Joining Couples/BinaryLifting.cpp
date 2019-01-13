#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::cout;
using std::swap;
using std::cin;

namespace Constants{

    const int oo = std::numeric_limits<int>::max();
    const int S = -1;

};

using namespace Constants;

namespace Graphs{

    struct Graph{

        typedef std::forward_list<int> List;
        
        vector<vector<int>> p;
        vector<int> index;
        vector<List> rev;
        vector<int> next;
        vector<int> size;
        vector<int> d;
        vector<int> s;
        int l;
        int n;
        
        Graph(int N): next(N), n(N), rev(N) {
            for(l = 0; N; N >>= 1, l++); 
        }
        
        int Lift(int u, int h){
            for(int k = 0; h; h >>= 1, k++){
                if(!(h & 1)) continue;
                if(p[u][k] != S){
                    u = p[u][k];
                }
            }
            return u;
        }

        int LCA(int u, int v){
            if(d[u] < d[u]) swap(u, v);
            u = Lift(u, d[u] - d[v]);
            if(u == v) return v;
            for(int k = l - 1; k >= 0; k--){
                if(p[u][k] != p[v][k]){
                    u = p[u][k];
                    v = p[v][k];
                }
            }
            u = p[u].front();
            v = p[v].front();
            if(u == v){
                return v;
            }
            return S;
        }

        void CompressCycles(){
            vector<bool> seen(n);
            vector<int> deg(n);
            queue<int> q;
            for(int u = 0; u < n; u++){
                deg[next[u]]++;
            }
            for(int v = 0; v < n; v++){
                if(!deg[v]) q.push(v);
            }
            p.assign(n, vector<int>(l, S));
            while(!q.empty()){
                int u = q.front(); q.pop();
                if(--deg[next[u]] == 0){
                    q.push(next[u]);
                }
                seen[u] = true;
            }
            int component = int();
            index.assign(n, S);
            d.assign(n, oo); 
            s.assign(n, S);
            for(int u = 0; u < n; u++){
                if(seen[u]) continue;
                int cl = int(), v = u;
                while(!seen[v]){
                    s[v] = component;
                    index[v] = cl++;
                    seen[v] = true;
                    d[v] = int();
                    q.push(v);
                    v = next[v]; 
                }
                size.push_back(cl);
                component++;
            }
            while(!q.empty()){
                int u = q.front(); q.pop();
                for(auto& v : rev[u]){
                    if(s[v] == S){
                        d[v] = d[u] + 1;
                        p[v][0] = u;
                        s[v] = s[u];
                        q.push(v);
                    }
                }
            }
        } 

        void BinaryLifting(){
            for(int k = 1; k < l; k++){
                for(int u = 0; u < n; u++){
                    auto& v = p[u][k - 1];
                    if(v == S) continue;
                    p[u][k] = p[v][k - 1];
                }
            }
        }

        int CycleDistance(int u, int v){
            if(index[u] < index[v]){
                swap(u, v);
            }
            int x = index[u] - index[v];
            int y = size[s[u]];
            y -= index[u];
            y += index[v];
            return std::min(x, y); 
        }

        int Distance(int u, int v){
            if(s[u] != s[v]) return oo;
            if(d[u] < d[v]) swap(u, v);
            int lca = LCA(u, v);
            if(lca != S){
                int w = d[u] + d[v];
                w -= 2 * d[lca];
                return w;
            }
            auto path = int();
            path += d[u];
            u = Lift(u, d[u]);
            path += d[v];
            v = Lift(v, d[v]);
            if(u == v) return path; 
            path += CycleDistance(u, v);
            return path;
        }

        void AddEdge(int u, int v){
            rev[v].push_front(u),
            next[u] = v;
        }

        void Build(){
            CompressCycles();
            BinaryLifting();
        }
    
    };

};

using Graphs::Graph;

int main(){
    std::ios_base::sync_with_stdio(0);
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        std::unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int u = 0; u < n; u++){
            int v; cin >> v, --v;
            g->AddEdge(u, v);
        }
        int q; cin >> q;
        g->Build();
        for(int k = 0; k < q; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            int d = g->Distance(u, v);
            if(d < oo){
                cout << d;
            }else{
                cout << S;
            }
            cout << '\n';
        } 
    }
    return 0;
}
