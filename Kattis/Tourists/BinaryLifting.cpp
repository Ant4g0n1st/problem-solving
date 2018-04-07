#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::swap;
using std::cin;

typedef long long int Long;

namespace Constants{

    const Long oo = numeric_limits<int>::max();
    const int S = -1;

};

using namespace Constants;

struct Tree{

    typedef forward_list<int> List;
    typedef vector<int> vInt;

    vector<List> edges;
    vector<vInt> p;
    vector<Long> w;
    int l,n;

    Tree(int N): edges(N), l(), n(N) {
        for(; N; N >>= 1, l++);
    }

    int Weight(int u, int v){
        return w[u] + w[v] - 2 * w[LCA(u, v)];
    }

    void BinaryLifting(int s = 0){
        w.assign(n, oo), w[s] = 0,
        p.assign(n, vInt(l, S));
        queue<int> q; q.push(s);
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(w[u] + 1 < w[v])
                    w[v] = w[u] + 1,
                    p[v][0] = u,
                    q.push(v);
        }
        for(int k = 1; k < l; k++)
            for(int u = 0; u < n; u++){
                auto& w = p[u][k - 1];
                if(w == S) continue;
                p[u][k] = p[w][k - 1];
            }
    }

    int Lift(int u, int h){
        for(int k = 0; h; h >>= 1, k++){
            if(!(h & 1)) continue;
            if(p[u][k] != S)
                u = p[u][k];
        }
        return u;
    }

    int LCA(int u, int v){
        if(w[u] < w[v]) swap(u, v);
        u = Lift(u, w[u] - w[v]);
        if(u == v) return v;
        for(int k = l - 1; k >= 0; k--)
            if(p[u][k] != p[v][k])
                u = p[u][k], 
                v = p[v][k];
        return p[u].front();
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        unique_ptr<Tree> t;
        t.reset(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            t->AddEdge(u, v);
        }
        t->BinaryLifting();
        Long sum = 0;
        for(int u = 1; u <= n; u++)
            for(int v = u << 1; v <= n; v += u)
                sum += t->Weight(--u, --v),
                ++sum, ++u, ++v;
        cout << sum << '\n';
    }
    return 0;
}