//https://atcoder.jp/contests/abc388/tasks
//A
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;
    cin >> s;
    string t = s[0] + string("UPC");  // 直接使用字符拼接字符串
    cout << t << '\n';
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n,d;cin>>n>>d;
    vector<pair<int, int>> snakes(n);
    for(int i = 0; i < n; i++) {
        cin>>snakes[i].first>>snakes[i].second;
    }
    for(int k = 1; k <= d; k++) {
        int ax = 0;
        for(const auto& snake : snakes) {
            int w = snake.first * (snake.second + k);
            ax = max(ax, w);
        }
        cout<<ax<<endl;
    }
    return 0;
}
//C
#include <bits/stdc++.h>
using ll = long long;
using namespace std;
int main() {
    int n;cin>>n;
    vector<ll> a(n);
    for(int i = 0; i < n; i++) {
        cin>>a[i];
    }
    ll res = 0;
    for(int i = 0; i < n; i++) {
        ll target = 2 * a[i];
        auto it = lower_bound(a.begin()+i+1, a.end(), target);
        int cnt = a.end() - it;
        res += cnt;
    }
    cout<<res<<"\n";
    return 0;
}
//D
