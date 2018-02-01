#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <set>

using std::unique_ptr;
using std::ios_base;
using std::reverse;
using std::vector;
using std::queue;
using std::cout;
using std::sort;
using std::cin;
using std::set;

typedef vector<int> vInt;
typedef int Bag;

const int S = -1;

struct Bipartite{

    vector<int> couple;
    vector<vInt> edges;
    vector<bool> seen;
    vector<bool> side;
    queue<int> path;
    int n,oo;

    Bipartite(int N): side(N), 
        n(N), couple(N, S), edges(N), seen(N) {} 

    vector<int> MaximumMatching(){
        for(int u = 0; u < n; u++){
            if(!side[u]) continue;
            AugmentingPath(u);
            while(!path.empty())
                seen[path.front()] = false,
                path.pop();
        }
        vector<int> matching;
        for(int v = 0; v < n; v++)
            if(!side[v] and couple[v] != S)
                matching.push_back(couple[v]);
        return matching;
    }

    int AugmentingPath(int u){
        seen[u] = true, path.push(u);
        for(auto& v : edges[u])
            if(couple[v] == S)
                return couple[v] = u;
        for(auto& v : edges[u]){
            auto& w = couple[v];
            if(seen[w]) continue;
            if(AugmentingPath(w) != S)
                return w = u;
        }
        return S;
    }

    void AddEdge(int u, int v){
        edges[u].push_back(v);
    }

    void SetRight(int v){ 
        side[v] = false; 
    }

    void SetLeft(int u){ 
        side[u] = true; 
    }

};

struct Vault{

    vector<int> a;
    vector<int> b;

    Vault(vector<int>& v){ 
        int n = v.size(); 
        int x = n >> 1;
        int z = 1 << x;
        set<int> c;
        for(int m = 0; m < z; m++){
            int s = 0;
            for(int k = 0; k < x; k++)
                if(m & (1 << k)) 
                    s += v[k];
            c.insert(s);
        }
        a.assign(c.begin(), c.end());
        int y = n - x; 
        z = 1 << y;
        c.clear();
        for(int m = 0; m < z; m++){
            int s = 0;
            for(int k = 0; k < y; k++)
                if(m & (1 << k))
                    s += v[x + k];
            c.insert(s);
        }
        b.assign(c.begin(), c.end());
    }

    bool CanFill(Bag w){
        int lb = 0, ub = b.size() - 1;
        int la = 0, ua = a.size();
        while(la < ua and ub >= lb){
            if(a[la] + b[ub] > w){
                --ub; continue;
            }
            if(a[la] + b[ub] < w){
                ++la; continue;
            }
            if(a[la] + b[ub] == w)
                return true;
        }
        return false;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int t; cin >> t;
    for(int c = 0; c < t; c++){ 
        int m,n; cin >> m >> n;
        vector<Vault> vaults;
        vector<Bag> b(m);
        for(auto& w : b) cin >> w;
        sort(b.begin(), b.end());
        for(int k = 0; k < n; k++){
            int x; cin >> x;
            vector<int> g(x);
            for(auto& it : g) 
                cin >> it;
            vaults.emplace_back(g);
        }
        unique_ptr<Bipartite> bp;
        reverse(b.begin(), b.end());
        bp.reset(new Bipartite(m + n));
        for(int u = 0; u < m; u++)
            bp->SetLeft(u);
        for(int v = 0; v < n; v++)
            bp->SetRight(m + v);
        for(int u = 0; u < m; u++)
            for(int v = 0; v < n; v++){
                if(!vaults[v].CanFill(b[u]))
                    continue;
                bp->AddEdge(u, m + v);
            }
        vector<int> matching; int max = 0;
        matching = bp->MaximumMatching();
        int r = matching.size();
        for(int k = 0; k < r; k++)
            max += b[matching[k]];
        cout << max << '\n';
    }
    return 0;
}