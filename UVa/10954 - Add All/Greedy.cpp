#include <iostream>
#include <queue>

using std::priority_queue;
using std::cout;
using std::cin;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        if(n == 0) continue;
        priority_queue<int> q;
        for(int k = 0; k < n; k++){
            int v; cin >> v;
            q.push(-v);
        }
        int cost = int();
        while(q.size() > 1){
            int u = -q.top(); q.pop();
            int v = -q.top(); q.pop();
            q.push(-(u + v));
            cost += u + v;
        }
        cout << cost << '\n';
    }
    return 0;
}
