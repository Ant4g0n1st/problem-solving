#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::cout;
using std::pair;
using std::cin;
using std::max;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

namespace Graphs{

    struct Bipartite{
        
        typedef std::forward_list<int> List;

        vector<pair<int,int>> p;
        vector<vector<int>> dp;
        vector<List> edges;
        vector<bool> seen;
        vector<bool> side;
        vector<int> cost;
        vector<int> w;
        int n;
        int s;

        Bipartite(int N): 
            edges(N), n(N), side(N), w(N) {}

        void FindComponents(){
            seen.assign(n, false);
            for(int u = 0; u < n; u++){
                if(!side[u]) continue;
                if(seen[u]) continue;
                auto weight = w[u];
                auto l = int();
                auto r = int();
                seen[u] = true;
                queue<int> q;
                q.push(u);
                while(!q.empty()){
                    int u = q.front(); q.pop();
                    for(auto& v : edges[u]){
                        if(seen[v]) continue;
                        seen[v] = true;
                        weight += w[v];
                        q.push(v);
                    }
                    if(side[u]){ 
                        l++;
                    }else{ 
                        r++;
                    }
                }
                cost.push_back(weight);
                p.emplace_back(l, r);
            }
            for(int v = 0; v < n; v++){
                if(side[v]) continue;
                if(seen[v]) continue;
                cost.push_back(w[v]);
                p.emplace_back(0, 1);
            }
        }

        pair<int,int> MaxCorruption(int m){
            FindComponents(), s = cost.size();
            dp.assign(s, vector<int>(++m, S));
            const auto x = MaxMembers(0, --m);
            for(int k = 0; k < s; k++){
                auto r = p[k].second;
                auto l = p[k].first;
                p[k].second = l;
                p[k].first = r; 
                dp[k].assign(m + 1, S);
            }
            const auto y = MaxMembers(0, m);
            return pair<int,int>(x, y); 
        }

        void SetWeight(int u, int weight){
            w[u] = weight;
        }

        int MaxMembers(int k, int money){
            if(k >= s) return int();
            if(!money) return int();
            auto& v = dp[k][money];
            if(v != S) return v;
            auto& z = p[k++];
            v = MaxMembers(k--, money);
            v += z.first;
            if(money < cost[k]){
                return v;
            }
            money -= cost[k++];
            int x = MaxMembers(k, money);
            x += z.second;
            v = max(v, x);
            return v; 
        }

        void AddEdge(int u, int v){
            edges[u].push_front(v),
            edges[v].push_front(u);
        }
    
        void SetRight(int v){
            side[v] = false;
        }
        
        void SetLeft(int u){
            side[u] = true;
        }

    };

};

using Graphs::Bipartite;

int main(){
    std::ios_base::sync_with_stdio(0);
    cout.tie(0), cin.tie(0); int d,p;
    while(cin >> d >> p){
        const int n = d + p;
        std::unique_ptr<Bipartite> bp;
        bp.reset(new Bipartite(n));
        int r; cin >> r;
        int b; cin >> b;
        for(int u = 0; u < d; u++){
            int w; cin >> w;
            bp->SetWeight(u, w);
            bp->SetLeft(u);
        }
        for(int v = d; v < n; v++){
            int w; cin >> w;
            bp->SetWeight(v, w);
            bp->SetRight(v);
        }
        for(int k = 0; k < r; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            bp->AddEdge(u, d + v);
        }
        auto m = bp->MaxCorruption(b);
        cout << m.first << ' ',
        cout << m.second,
        cout << '\n';
    }
    return 0;
}
