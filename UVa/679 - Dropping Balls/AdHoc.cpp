#include <iostream>

using std::ios_base;
using std::cout;
using std::cin;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        int d; cin >> d, --d;
        int x; cin >> x;
        int node = 1;
        for(int k = 0; k < d; k++)
            node <<= 1, node += !(x & 1),
            x = (x + 1) >> 1;
        cout << node << '\n';
    }
    return 0;
}