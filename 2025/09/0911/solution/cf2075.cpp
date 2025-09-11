#include <bits/stdc++.h>
using namespace std;
using ll = long long;
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(m);
        for (int i = 0; i < m; i++) cin >> a[i];
 
        // 统计前缀：cnt[x] = 颜色数目，能涂 >= x 块板
        vector<int> cnt(n + 1, 0);
        for (int ai : a) {
            cnt[1]++; // 所有颜色至少能涂 1
            if (ai + 1 <= n) cnt[ai + 1]--; 
        }
        for (int i = 1; i <= n; i++) cnt[i] += cnt[i - 1];
 
        ll ans = 0;
        for (int L = 1; L <= n - 1; L++) {
            int R = n - L;
            int cntL = cnt[L];
            int cntR = cnt[R];
            int cntBoth = cnt[max(L, R)];
            ans += 1LL * cntL * cntR - cntBoth;
        }
        cout << ans << "\n";
    }
 
    return 0;
}