#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>
#include <set>

using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::set;

namespace Constants{

    const int oo = std::numeric_limits<int>::max();
    const int S = -1;

};

using namespace Constants;

struct Bipartite{

    struct Edge{

        int v,w;

        Edge(int V, int W): v(V), w(W) {}

    };

    typedef std::forward_list<Edge> List;

    vector<int> couple;
    vector<List> edges;
    vector<bool> match;
    vector<bool> seen;
    vector<bool> side;
    queue<int> path;
    vector<int> d;
    set<int> s;
    int n;

    Bipartite(int N): edges(N), n(N), side(N) {}

    int MaximumMatching(int max = oo){
        match.assign(n, false),
        couple.assign(n, S);
        while(true){
            ShortestPaths(max); 
            int matching = 0;
            for(int u = 0; u < n; u++){
                if(!side[u]) continue;
                if(match[u]) continue;
                if(AugmentingPath(u, max) != S) 
                    ++matching;
                while(!path.empty())
                    seen[path.front()] = false,
                    path.pop();
            }
            if(!matching) break;
        }
        int matching = 0;
        for(int u = 0; u < n; u++)
            if(match[u]) ++matching;
        return matching;
    }

    int AugmentingPath(int u, int max = oo){
        seen[u] = true, path.push(u);
        for(auto& e : edges[u]){
            if(max < e.w) continue;
            auto& z = couple[e.v];
            if(z == S){
                match[u] = true;
                return z = u;
            }
        }
        for(auto& e : edges[u]){
            if(max < e.w) continue;
            auto& z = couple[e.v];
            if(seen[z]) 
                continue;
            if(d[z] != d[u] + 1) 
                continue;
            if(AugmentingPath(z, max) != S){
                match[u] = true;
                return z = u;
            }
        }
        return S;
    }

    int Lightest(int expected){
        vector<int> w(s.begin(), s.end());
        int b = w.size() - 1, k = b;
        int a = 0; seen.resize(n);
        while(a <= b){
            auto h = (a + b) >> 1;
            int m = MaximumMatching(w[h]);
            if(m < expected)
                a = h + 1;
            else
                b = h - 1,
                k = h;
        }
        return w[k];
    }

    void ShortestPaths(int max = oo){ 
        d.assign(n, oo);
        queue<int> q;
        for(int u = 0; u < n; u++){
            if(!side[u]) continue;
            if(match[u]) continue;
            d[u] = 0, q.push(u);
        }
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& e : edges[u]){
                if(max < e.w) continue;
                auto& z = couple[e.v];
                if(z == S) continue;
                if(d[u] + 1 < d[z])
                    d[z] = d[u] + 1,
                    q.push(z);
            }
        }
    }

    void AddEdge(int u, int v, int w){
        edges[u].emplace_front(v, w),
        s.insert(w);
    }

    void SetRight(int v){
        side[v] = false;
    }

    void SetLeft(int u){
        side[u] = true;
    }

};

struct Location{

    int x,y;

    Location(): x(), y() {}

    int Distance(const Location& other){
        auto dx = x - other.x;
        if(dx < 0) dx = -dx;
        auto dy = y - other.y;
        if(dy < 0) dy = -dy;
        return dx + dy;
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        vector<Location> student(n);
        for(auto& s : student)
            cin >> s.x >> s.y;
        vector<Location> teacher(n);
        for(auto& t : teacher)
            cin >> t.x >> t.y;
        unique_ptr<Bipartite> z;
        z.reset(new Bipartite(n << 1));
        for(int u = 0; u < n; u++){
            auto& s = student[u];
            z->SetRight(n + u),
            z->SetLeft(u);
            for(int v = 0; v < n; v++){
                auto& t = teacher[v];
                auto d = s.Distance(t);
                z->AddEdge(u, n + v, d);
            }
        }
        cout << z->Lightest(n),
        cout << '\n';
    }
    return 0;
}