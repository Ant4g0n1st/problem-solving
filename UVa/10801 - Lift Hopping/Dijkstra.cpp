#include <forward_list>
#include <iostream>
#include <sstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <map>

using std::numeric_limits;
using std::priority_queue;
using std::forward_list;
using std::stringstream;
using std::unique_ptr;
using std::getline;
using std::vector;
using std::string;
using std::cout;
using std::swap;
using std::cin;
using std::map;
using std::ws;

typedef vector<bool> vBool;
typedef vector<int> vInt;

const int oo = numeric_limits<int>::max();
const int S = 60;

struct Graph{

    struct Edge{

        int e,v,w;

        Edge(int V, int W, int E): e(E), v(V), w(W) {}

    };

    typedef forward_list<Edge> List;

    struct Path : Edge{

        Path(int V, int W, int E): Edge(V, W, E) {}

        bool operator<(const Edge& other) const{
            return w > other.w;
        }

    };

    vector<forward_list<int>> l;
    vector<vBool> seen;
    vector<List> edges;
    vector<vInt> d;
    int n,m;

    Graph(int N, int M): edges(N), l(N), m(M), n(N) {}

    void AddEdge(int u, int v, int w, int e){
        edges[u].emplace_front(v, w, e);
    }

    int ShortestPath(int s, int t){
        d.assign(n, vInt(m, oo));
        seen.assign(n, vBool(m));
        priority_queue<Path> q;
        for(auto& e : l[s])
            q.emplace(s, 0, e),
            d[s][e] = 0;
        while(!q.empty()){
            auto e = q.top().e;
            auto u = q.top().v;
            auto w = q.top().w; q.pop();
            if(seen[u][e]) continue;
            if(u == t) return w;
            seen[u][e] = true;
            for(auto& r : edges[u]){
                auto f = r.e;
                auto v = r.v;
                auto x = r.w;
                if(seen[v][f]) continue;
                if(f != e) x += S;
                if(w + x < d[v][f]){
                    Path p(v, w + x, f);
                    d[v][f] = w + x,
                    q.push(p);
                }
            }
        }
        return oo;
    }

    void AddElevator(int u, int e){
        l[u].push_front(e);
    }

};

typedef forward_list<int> List;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n,t;
    while(cin >> n >> t){
        vector<int> s(n);
        for(auto& k : s) cin >> k;
        map<int,int> m; int z = 0;
        unique_ptr<Graph> g;
        vector<List> f(n);
        vector<int> floor;
        for(int k = 0; k < n; k++){
            string in; cin >> ws;
            getline(cin, in);
            stringstream st(in); int r;
            while(st >> ws >> r){
                if(!m.count(r))
                    floor.push_back(r),
                    m[r] = z++;
                f[k].push_front(m[r]);
            }
        }
        g.reset(new Graph(z, n));
        vector<List> e(z);
        for(int k = 0; k < n; k++)
            for(auto& u : f[k])
                e[u].push_front(k);
        for(int u = 0; u < z; u++)
            for(auto& w : e[u]){
                g->AddElevator(u, w);
                for(auto& v : f[w]){
                    if(v == u) continue;
                    auto x = floor[u];
                    auto y = floor[v];
                    if(x < y) swap(x, y);
                    int d = (x - y) * s[w];
                    g->AddEdge(u, v, d, w);
                }
            }
        if(!m.count(t)) 
            m[t] = z++;
        auto a = m[0];
        auto b = m[t];
        int sp = g->ShortestPath(a, b); 
        if(sp < oo){
            cout << sp << '\n';
            continue;
        }
        cout << "IMPOSSIBLE\n";
    }
    return 0;
}