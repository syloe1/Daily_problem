//https://ac.nowcoder.com/acm/contest/62033
//A

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    string s;cin>>s;
    for(char& c : s) {
        if(c >= 'A' && c <= 'Z') {
            c = (c == 'Z' ? 'A' : c + 1);
        } else if(c >= 'a' && c <= 'z') {
            c = (c == 'a' ? 'z' : c - 1);
        }
    }
    cout<<s<<'\n';
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n, -1);
    for (int i = 0, j = n - k + 1; j <= n; i += 2, j++) a[i] = j;
    for (int i = 0, j = 1; i < n; i++) if (a[i] == -1) a[i] = j++;
    for (int i = 0; i < n; i++) cout << a[i] << (i + 1 == n ? '\n' : ' ');
    return 0;
}
//C
/*O(n^2)
枚举起点u, 从u DFS向下，维护当前v, > r提前剪枝
*/
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int n; i64 l ,r;string s;
vector<vector<int>> g;
i64 ans = 0;
void dfs(int u, int fa, i64 val) {
    for(int v : g[u]) {
        if(v == fa) continue;
        i64 nv = (val << 1) | (s[v-1] - '0');
        if(nv > r) continue;
        if(nv >= l) ans++;
        dfs(v, u, nv);
    }
}
int main() {
    cin>>n>>l>>r;
    cin>>s; 
    g.assign(n+1, {});
    for(int i = 0; i < n - 1; i++) {
        int u, v;cin>>u>>v; 
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for(int i = 1; i <= n; i++) {
        dfs(i, 0, s[i-1] - '0');
    }
    cout<<ans<<'\n';
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
constexpr i64 mod = 998244353;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    i64 n, ans = 0;
    int m1, m2;
    cin >> n >> m1 >> m2;
    vector<pair<int, i64>> vp1(m1), vp2(m2);
    for (auto &[u, c] : vp1) {
        cin >> u >> c;
    }
    for (auto &[u, c] : vp2) {
        cin >> u >> c;
    }
    int x1 = 0, x2 = 0;
    i64 y1 = 0, y2 = 0;
    int l1 = -1, l2 = -1;
    while (x1 < m1 and x2 < m2) {
        i64 h = min(vp1[x1].second - y1, vp2[x2].second - y2);
        int u1 = vp1[x1].first, u2 = vp2[x2].first;
        ans += l1 != -1 and set({l1, l2, u1, u2}).size() == 2;
        if (u1 != u2) {
            ans += h - 1;
        }
        l1 = u1;
        l2 = u2;
        y1 += h;
        if (y1 == vp1[x1].second) {
            x1 += 1;
            y1 = 0;
        }
        y2 += h;
        if (y2 == vp2[x2].second) {
            x2 += 1;
            y2 = 0;
        }
    }
    cout << ans;
}
//E
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
constexpr i64 mod = 1000000007;
i64 power(i64 a, i64 r) {
    i64 res = 1;
    for (; r; r >>= 1, a = a * a % mod) {
        if (r & 1) {
            res = res * a % mod;
        }
    }
    return res;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    i64 n, k;
    cin >> n >> k;
    k = max(k, n - k);
    cout << power(4, n - k) * power(3, k - 1) % mod *
                power(4, (k - 1) * (k - 2) / 2) % mod *
                power(6, (n - k) * (n - k - 1) / 2) % mod *
                power(6, (n - k) * (k - 1)) % mod;
}