#include <unordered_map>
#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>
#include <set>

using std::numeric_limits;
using std::priority_queue;
using std::unordered_map;
using std::forward_list;
using std::unique_ptr;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::set;

namespace Constants{

    const int oo = numeric_limits<int>::max();
    const int S = -1;
    const int L = 2;

};

using namespace Constants;

struct Graph{

    typedef forward_list<int> List;

    struct Path{

        int v,w;

        Path(int V, int W): v(V), w(W) {}

        bool operator<(const Path& other) const{
            return w > other.w;
        }

    };

    vector<bool> finish;
    vector<List> edges;
    vector<bool> start;
    vector<int> w;
    int n;

    Graph(int N): edges(N), finish(N), n(N), start(N), w(N) {}

    void SetFinish(int v){ finish[v] = true; }

    void SetWeight(int u, int k){ w[u] = k; }

    void SetStart(int u){ start[u] = true; }

    int ShortestPaths(){
        priority_queue<Path> q;
        vector<bool> seen(n);
        vector<int> d(n, oo);
        for(int u = 0; u < n; u++){
            if(!start[u]) continue; 
            Path p(u, d[u] = w[u]);
            q.push(p);
        }
        while(!q.empty()){
            int u = q.top().v; q.pop();
            if(finish[u]) return d[u];
            if(seen[u]) continue;
            seen[u] = true;
            for(auto& v : edges[u]){
                if(seen[v]) continue;
                if(d[u] + w[v] < d[v]){
                    d[v] = d[u] + w[v];
                    Path p(v, d[v]);
                    q.push(p);
                }
            }
        }
        return oo;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n and n){ int p = 0;
        unordered_map<string,int> m;
        vector<set<int>> langs(n);
        vector<string> words(n);
        string x; cin >> x;
        int s = m[x] = p++;
        string y; cin >> y;
        int t = m[y] = p++;
        for(int k = 0; k < n; k++){
            string a; cin >> a;
            string b; cin >> b;
            cin >> words[k];
            if(!m.count(a)) 
                m[a] = p++;
            if(!m.count(b))
                m[b] = p++;
            langs[k].insert(m[a]),
            langs[k].insert(m[b]);
        }
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int u = 0; u < n; u++){
            int w = words[u].size();
            g->SetWeight(u, w);
            for(auto& k : langs[u]){
                if(k == t)  
                    g->SetFinish(u);
                if(k == s)
                    g->SetStart(u);
            }
        }
        for(int u = 0; u < n; u++)
            for(int v = u + 1; v < n; v++){
                auto a = words[u].front();
                auto b = words[v].front();
                if(a == b) continue;
                auto& lu = langs[u];
                auto& lv = langs[v];
                for(auto& k : lu){
                    if(!lv.count(k))
                        continue;
                    g->AddEdge(u, v);
                    break;
                }
            }
        auto w = g->ShortestPaths();
        if(w < oo){
            cout << w << '\n';
            continue;
        }
        cout << "impossivel",
        cout << '\n';
    }
    return 0;
}