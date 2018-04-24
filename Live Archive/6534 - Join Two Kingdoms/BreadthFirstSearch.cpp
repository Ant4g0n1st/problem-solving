#include <forward_list>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>
#include <vector>
#include <cmath>
#include <queue>

using std::numeric_limits;
using std::forward_list;
using std::setprecision;
using std::unique_ptr;
using std::vector;
using std::fixed;
using std::queue;
using std::round;
using std::cout;
using std::sort;
using std::swap;
using std::cin;

typedef long long int Long;
typedef long double Double;

namespace Constants{

    const int oo = numeric_limits<int>::max();
    const Double R = 1000;
    const int S = -1;
    const int P = 3;

};

using namespace Constants;

struct Tree{

    typedef forward_list<int> List;

    vector<List> edges;
    int n;

    Tree(int N): edges(N), n(N) {}

    vector<Long> LongestPaths(Long& d){
        vector<Long> path;
        bool two = false;
        vector<int> deg;
        auto max = -oo;
        vector<int> w;
        queue<int> q;
        auto a = 0;
        auto b = 0;
        deg.resize(n);
        for(auto& adj : edges)
            for(auto& u : adj) ++deg[u];
        w.assign(n, oo);
        for(int u = 0; u < n; u++)
            if(--deg[u] == 0) q.push(u),
                w[u] = 0;
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(--deg[v] == 0)
                    w[v] = w[u] + 1,
                    q.push(v);
            swap(a, b), a = u;
        }
        if(w[b] < w[a]) b = S;
        else two = (b != a);
        w.assign(n, oo);
        q.push(a), w[a] = 0;
        if(b != S) w[b] = 0,
            q.push(b);
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(w[u] + 1 < w[v])
                    w[v] = w[u] + 1,
                    q.push(v);
            if(max < w[u]) 
                max = w[u];
        }
        path.resize(n);
        for(int u = 0; u < n; u++)
            path[u] = w[u] + max + two;
        d = (max << 1) + two;
        return path;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n,q;
    cout << fixed << setprecision(P);
    while(cin >> n >> q){
        unique_ptr<Tree> nlog;
        unique_ptr<Tree> quad;
        nlog.reset(new Tree(n));
        quad.reset(new Tree(q));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            nlog->AddEdge(u, v);
        }
        for(int k = 1; k < q; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            quad->AddEdge(u, v);
        }
        vector<Long> dp;
        Long sum = 0;
        Long ds,dt;
        auto ps = nlog->LongestPaths(ds);
        auto pt = quad->LongestPaths(dt);
        sort(ps.begin(), ps.end());
        Long max = ds;
        if(max < dt) 
            max = dt;
        dp.resize(n);
        dp.front() = ps.front();
        for(int k = 1; k < n; k++)
            dp[k] = dp[k - 1] + ps[k];
        for(auto& p : pt){
            int e = n - 1; 
            int b = 0;
            int k = n;
            while(b <= e){
                int h = (b + e) >> 1;
                int z = ps[h];
                if(z + p + 1 > max)
                    e = h - 1, k = h;
                else
                    b = h + 1;
            }
            sum += max * k;
            if(k == n) 
                continue;
            Long r = n - k;
            Long s = dp.back();
            if(k) s -= dp[--k];
            sum += r * p,
            sum += r + s;
        }
        Double out = sum;
        out /= Double(n);
        out /= Double(q);
        out = round(out * R) / R;
        cout << double(out),
        cout << '\n';
    }
    return 0;
}