//https://ac.nowcoder.com/acm/contest/68346
//A
int main() {
    string x; int k;cin>>x>>k;
    reverse(x.begin(), x.begin()+k);
    long long ans = stoll(x);
    cout<<ans<<endl;
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
bool is(int x) {
    if(x < 2) return false;
    for(int i = 2; i * i <= x; i++) {
        if(x % i == 0) return false;
    }
    return true;
}
int n, ans = 0;
bool ok[11][11];
bool us[11];
void dfs(vector<int>& path) {
    if(path.size() == n) {
        ans++; return ;
    }
    for(int i = 1; i <= n; i++) {
        if(!us[i]) {
            if(!path.empty() && !ok[path.back()][i]) continue;
            us[i] = true;
            path.push_back(i);
            dfs(path);
            path.pop_back();
            us[i] = false;
        }
    }
}
int main() {
    cin>>n;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(!is(i+j)) {
                ok[i][j] = true;
            }
        }
    }
    vector<int> path;
    dfs(path);
    cout<<ans<<"\n";
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    map<int, int> cnt;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        cnt[x % k]++;
    }

    int ans = 0;
    for (auto &[r, c] : cnt) {
        if (r == 0) {
            ans += c;
        } else if (2 * r == k) {
            ans += c / 2;
        } else if (cnt.count(k - r)) {
            ans += min(c, cnt[k - r]);
            cnt[k - r] = 0;  // 避免重复计算
        }
    }

    cout << ans << '\n';
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];

    const ll NEG = -1e18;
    vector<vector<ll>> dp(x + 1, vector<ll>(2, NEG));
    dp[x][0] = 0;  // 初始时还有 x 的钱，上一个不是原价买的

    for (int i = 0; i < n; ++i) {
        vector<vector<ll>> nxt(x + 1, vector<ll>(2, NEG));
        for (int j = 0; j <= x; ++j) {
            if (dp[j][0] == NEG && dp[j][1] == NEG) continue;

            // 不买
            nxt[j][0] = max({nxt[j][0], dp[j][0], dp[j][1]});

            // 原价买
            if (j >= a[i])
                nxt[j - a[i]][1] = max(nxt[j - a[i]][1], max(dp[j][0], dp[j][1]) + b[i]);

            // 半价买（只能在上一个原价买的情况下）
            if (j >= a[i] / 2 && dp[j][1] != NEG)
                nxt[j - a[i] / 2][0] = max(nxt[j - a[i] / 2][0], dp[j][1] + b[i]);
        }
        dp.swap(nxt);
    }

    ll ans = 0;
    for (int j = 0; j <= x; ++j)
        ans = max({ans, dp[j][0], dp[j][1]});

    cout << ans << '\n';
    return 0;
}
