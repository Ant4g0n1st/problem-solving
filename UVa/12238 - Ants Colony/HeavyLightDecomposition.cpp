#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::swap;
using std::cin;

typedef long long int Long;
typedef vector<Long> vLong;
typedef vector<bool> vBool;

const int S = -1; 

struct Tree{

	struct Edge{

		Long w;
		int v;

		Edge(int V, Long W): v(V), w(W) {}

	};

	typedef vector<Edge> vEdge;

	vector<vEdge> edges;
	vector<int> level;
	vector<int> super;
	vector<int> size;
	vector<int> idx;
	vector<int> top;
	vector<Long> dp;
	vector<int> up;
	int c,n; 

	Tree(int N): c(), n(N), dp(N), up(N),
		super(N), idx(N), size(N),
		edges(N, vEdge()) {}

	void AddEdge(int u, int v, Long w){
		edges[u].push_back({v, w}),
		edges[v].push_back({u, w});
	}

	int Build(int u, int p = S){
		int sub = 0; up[u] = p;
		for(auto& e : edges[u]){
			if(e.v == p) continue;
			sub += Build(e.v, u);
		}
		int m = S, s = S;
		for(auto& e : edges[u]){
			if(e.v == p) continue;
			if(m < size[e.v])
				m = size[s = e.v];
		}
		if(s != S) 
			super[u] = super[s],
			idx[u] = idx[s] + 1;
		else super[u] = c++;
		return size[u] = ++sub;
	}

	void HeavyLight(int r = 0){ 
		Build(r); vBool seen(n);
		queue<int> q; q.push(r);
		top.assign(c, S),
		level.resize(n),
		seen[r] = true;
		while(!q.empty()){ 
			int u = q.front(); q.pop();
			for(auto& e : edges[u]){
				if(seen[e.v]) continue;
				if(super[e.v] != super[u])
					level[e.v] = level[u] + 1;
				else level[e.v] = level[u]; 
				dp[e.v] = dp[u] + e.w,
				seen[e.v] = true, 
				q.push(e.v);
			}
			if(top[super[u]] == S)
				top[super[u]] = u;
		}
	}

	int LCA(int u, int v){
		if(level[u] < level[v]) swap(u, v);
		while(level[u] > level[v])
			u = up[top[super[u]]];
		while(super[u] != super[v])
			u = up[top[super[u]]],
			v = up[top[super[v]]];
		if(idx[u] < idx[v]) swap(u, v);
		return u;
	}

	Long Weight(int u, int v){
		return dp[u] + dp[v] - 2LL * dp[LCA(u, v)];
	}

};

int main(){
	ios_base::sync_with_stdio(0),
	cout.tie(0), cin.tie(0); int n;
	while(cin >> n and n){
		unique_ptr<Tree> t(new Tree(n));
		for(int u = 1; u < n; u++){
			int v; Long w;
			cin >> v >> w;
			t->AddEdge(u, v, w);
		}
		t->HeavyLight();
		int q; cin >> q;
		vLong out(q);
		for(int k = 0; k < q; k++){
			int u,v; cin >> u >> v;
			out[k] = t->Weight(u, v);
		}
		cout << out.front(); 
		for(int k = 1; k < q; k++)
			cout << ' ' << out[k];
		cout << '\n';
	}
	return 0;
} 
