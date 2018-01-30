#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <queue>

using std::unordered_map;
using std::unique_ptr;
using std::ios_base;
using std::string;
using std::vector;
using std::queue;
using std::cout;
using std::abs; 
using std::cin;
using std::ws;

typedef vector<int> vInt;

const int S = -1;

struct Bipartite{

    vector<int> couple;
    vector<vInt> edges;
    vector<bool> match;
    vector<bool> seen;
    vector<bool> side;
    queue<int> path;
    vector<int> d;
    vector<int> p;
    int n,oo;

    Bipartite(int N): oo(N + 1), n(N), d(N), p(N),
        couple(N, S), edges(N), match(N), seen(N), side(N) {}

    int MaximumMatching(){
        while(true){
            ShortestPaths(); int m = 0;
            for(int u = 0; u < n; u++){
                if(!side[u]) continue;
                if(match[u]) continue;
                if(AugmentingPath(u) != S) ++m;
                while(!path.empty())
                    seen[path.front()] = false,
                    path.pop();
            } if(!m) break;
        }
        int matching = 0;
        for(int v = 0; v < n; v++){
            if(side[v]) continue;
            if(couple[v] != S) ++matching;
        }
        return matching;
    }

    void SetRight(int v){ side[v] = false; }

    void SetLeft(int u){ side[u] = true; }

    int AugmentingPath(int u){
        seen[u] = true, path.push(u);
        for(auto& v : edges[u]){
            auto& w = couple[v];
            if(w == S){
                match[u] = true;
                return w = u;
            }
        }
        for(auto& v : edges[u]){
            auto& w = couple[v];
            if(seen[w] or p[w] != u) 
                continue;
            if(AugmentingPath(w) != S){
                match[u] = true;
                return w = u;
            }
        }
        return S;
    }

    void ShortestPaths(){ queue<int> q;
        for(int u = 0; u < n; u++){
            d[u] = oo;
            if(!side[u]) continue;
            if(match[u]) continue;
            q.push(u),
            d[u] = 0;
        }
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& v : edges[u]){
                int w = couple[v];
                if(w == S) continue;
                if(d[u] + 1 < d[w])
                    d[w] = d[u] + 1,
                    q.push(w),
                    p[w] = u;
            }
        }
    }

    void AddEdge(int u, int v){
        edges[u].push_back(v);
    }

};

struct Student{

    static const int H = 40;

    int h,m,s;

    Student(int H, int M, int Z): h(H), m(M), s(Z) {}

    bool operator==(const Student& other) const{
        return m == other.m and s != other.s 
            and abs(h - other.h) <= H;
    }

};

int main(){
    ios_base::sync_with_stdio(0), 
    cout.tie(0), cin.tie(0); 
    int t; cin >> t;
    for(int c = 0; c < t; c++){
        unordered_map<string,int> m;
        unordered_map<string,int> s;
        unique_ptr<Bipartite> bp;
        int mp = 0, sp = 0;
        vector<Student> x;
        vector<Student> y;
        int n; cin >> n;
        for(int k = 0; k < n; k++){  
            int h; cin >> ws >> h;
            char g; cin >> ws >> g;
            string a; cin >> ws >> a;
            if(!m.count(a)) m[a] = mp++;
            string b; cin >> ws >> b;
            if(!s.count(b)) s[b] = sp++;
            if(g == 'F')
                x.emplace_back(h, m[a], s[b]);
            else 
                y.emplace_back(h, m[a], s[b]);
        } 
        int a = x.size(), b = y.size();
        bp.reset(new Bipartite(n));
        for(int u = 0; u < a; u++)
            bp->SetLeft(u);
        for(int v = 0; v < b; v++)
            bp->SetRight(a + v);
        for(int u = 0; u < a; u++)
            for(int v = 0; v < b; v++)
                if(x[u] == y[v])
                    bp->AddEdge(u, a + v);
        cout << n - bp->MaximumMatching();
        cout << '\n'; 
    }
    return 0;
}