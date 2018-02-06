#include <algorithm>
#include <iostream>
#include <memory>
#include <limits>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::priority_queue;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::sort;
using std::cin;
using std::min;
using std::ws;

typedef long long int Long;
typedef vector<Long> vLong;

const Long oo = numeric_limits<int>::max();
const int M = -1;
const int S = 26;

struct Graph{

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef vector<Edge> vEdge;

    struct Path : Edge{

        Path(int V, Long W): Edge(V, W) {}

        bool operator<(const Path& other) const{
            if(w == other.w) return v > other.v;
            return w > other.w;
        }

    };

    vector<vEdge> edges;
    vector<bool> seen;
    vector<Long> d;
    int n;

    Graph(int N): d(N, oo), edges(N), n(N), seen(N) {}

    void ShortestPaths(int s){
        priority_queue<Path> q;
        q.emplace(s, 0), d[s] = 0;
        while(!q.empty()){
            auto u = q.top().v; q.pop();
            if(seen[u]) continue;
            seen[u] = true;
            for(auto& e : edges[u]){
                if(seen[e.v]) continue;
                if(d[u] + e.w < d[e.v]){
                    d[e.v] = d[u] + e.w;
                    Path p(e.v, d[e.v]);
                    q.push(p);
                }
            }
        }
    }

    void AddEdge(int u, int v, Long w){
        edges[u].emplace_back(v, w);
    }

    bool Reached(int u){ 
        return seen[u]; 
    }

    Long Weight(int u){ 
        return d[u]; 
    }

};

struct Edge{

    int b,u,v,y;
    Long w;

    Edge(): b(), u(), v(), w(), y() {}

};

char ReadLetter(){ char c;
    cin >> ws >> c;
    return c;
}

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m; 
    while(cin >> m and m){ int n = 0;
        vector<int> node(S, M); 
        vector<Edge> edges(m);
        unique_ptr<Graph> a;
        unique_ptr<Graph> b;
        for(auto& e : edges){
            e.y = (ReadLetter() == 'Y'),
            e.b = (ReadLetter() == 'B'),
            e.u = ReadLetter() - 'A',
            e.v = ReadLetter() - 'A';
            if(node[e.u] == M)
                node[e.u] = n++;
            if(node[e.v] == M)
                node[e.v] = n++;
            cin >> e.w;
        }
        a.reset(new Graph(n)),
        b.reset(new Graph(n));
        vector<char> c(n);
        for(auto& e : edges){
            c[node[e.u]] = 'A' + e.u,
            c[node[e.v]] = 'A' + e.v,
            e.u = node[e.u],
            e.v = node[e.v];
            if(e.y){
                a->AddEdge(e.u, e.v, e.w);
                if(!e.b) continue;
                a->AddEdge(e.v, e.u, e.w);
            }else{
                b->AddEdge(e.u, e.v, e.w);
                if(!e.b) continue;
                b->AddEdge(e.v, e.u, e.w);   
            }
        }
        auto u = node[ReadLetter() - 'A'];
        auto v = node[ReadLetter() - 'A'];
        bool meet = false; Long mn = oo; 
        if(u != M and v != M){
            a->ShortestPaths(u),
            b->ShortestPaths(v);
            for(int w = 0; w < n; w++){
                if(!a->Reached(w)) continue;
                if(!b->Reached(w)) continue; 
                auto uw = a->Weight(w);
                auto vw = b->Weight(w);
                mn = min(mn, uw + vw),
                meet = true;
            }
        }
        if(!meet){
            cout << "You will never",
            cout << " meet.\n";
            continue;
        } 
        vector<int> best;
        for(int w = 0; w < n; w++){
            auto uw = a->Weight(w);
            auto vw = b->Weight(w);
            if(uw + vw == mn)
                best.push_back(w);
        }
        sort(best.begin(), best.end());
        cout << mn;
        for(auto& w : best)
            cout << ' ' << c[w];
        cout << '\n';
    }
    return 0;
}