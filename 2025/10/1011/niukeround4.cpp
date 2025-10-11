//https://ac.nowcoder.com/acm/contest/61571
//A
/*
最简单的方式， 先放入k个you,长度为3k.
剩下n-3k，全填y
*/
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    i64 n, k;cin>>n>>k;
    if(n < 3 * k) {
        cout<<-1<<'\n';return 0;
    }
    string res;
    for(int i = 0; i < k; i++) {
        res += "you";
    }
    while(res.size() < n) {
        res += 'y';
    }
    cout<<res<<'\n';
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    i64 n; cin>>n;
    i64 ans = 0;
    if(n % 3 == 0) {
        ans = ((n-1) / 3);
    } else {
        ans = ((n-1) / 3 + n / 3);
    }
    cout<<ans<<'\n';
    return 0;
}
//C 
//(O + K)
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
constexpr i64 mod = 1000000007;
int main() {
    ios::sync_with_stdio(false);
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &ai : a) {
        cin >> ai;
    }
    i64 sum = 0, min_sum = 0;
    for (int i = 0, op, x; i < k; i += 1) {
        cin >> op >> x;
        sum += op == 1 ? x : -x;
        min_sum = min(min_sum, sum);
    }
    i64 ans = 0;
    for (i64 ai : a) {
        if (ai + min_sum >= 0) {
            (ans += ai + sum) %= mod;
        } else {
            (ans += sum - min_sum) %= mod;
        }
    }
    cout << ans << "\n";
}
//D
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
vector<pair<i64, int>> divide(i64 n) {
    vector<pair<i64, int>> res; //<质数, 幂>
    for(i64 i = 2; i * i <= n; i++) {
        if(n % i == 0) {
            int cnt = 0;
            while(n % i == 0) {
                n /= i; cnt++;
            }
            res.push_back({i, cnt});
        }
    }
    if(n>1) res.push_back({n, 1});
    return res;
}
void dfs(const vector<pair<i64, int>>& a, int idx, i64 cur, vector<i64> &divs) {
    if (idx == a.size()) {
        divs.push_back(cur);
        return ;
    }
    i64 p = a[idx].first;
    int cnt = a[idx].second;
    i64 v = 1;
    for(int i = 0; i <= cnt; i++) {
        dfs(a, idx+1,cur*v, divs);
        v *= p;
    }
}
int main() {
    i64 a,b;cin>>a>>b;
    auto fa = divide(a);
    auto fb = divide(b);
    map<i64, int> mp;
    for(auto [p, c] : fa) mp[p] += c;
    for(auto [p, c] : fb) mp[p] += c;
    vector<pair<i64, int>> f;
    for(auto [p, c] : mp) f.push_back({p, c});
    vector<i64> divs;
    dfs(f, 0 , 1, divs);
    sort(divs.begin(), divs.end());
    cout<<divs.size()<<'\n';
    for(i64 v : divs) cout<<v<<" ";
    cout<<'\n';
    return 0;
}
//E
