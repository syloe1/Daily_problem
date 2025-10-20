//https://ac.nowcoder.com/acm/contest/65507
//A 矩阵旋转180. (i, j) 的元素会变成位置 (n - i + 1, n - j + 1)。
int main() {
    int n;
    cin >> n;
    vector<vector<int>> a(n, vector<int>(n));
    
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> a[i][j];
    
    for (int i = n - 1; i >= 0; --i) {
        for (int j = n - 1; j >= 0; --j) {
            cout << a[i][j];
            if (j > 0) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
//B 符合基金数量 k, ans = 2 ^ k - 1 
const int mod = 1e9 + 7;
i64 qpow(i64 a, i64 b) {
    i64 res = 1;
    while (b > 0) {
        if(b&1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res; 
}
int main() {
    int n; i64 x, y;cin>>n>>x>>y;
    int c = 0;
    for(int i = 0; i < n; i++) {
        i64 a, b; cin>>a>>b;
        if(a >= x && b <= y) c++;
    }
    if(c == 0) {
        cout<<0<<"\n"; return 0;
    }
    cout<<(qpow(2, c) - 1 + mod) % mod;
    return 0;
}
//C

bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }
bool is_lower(char c) { return c >= 'a' && c <= 'z'; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool is_special(char c) { return c == ',' || c == '.' || c == '?' || c == '!'; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        string s;
        cin >> s;

        int cnt_upper = 0, cnt_lower = 0, cnt_digit = 0, cnt_special = 0;

        for (char c : s) {
            if (is_upper(c)) cnt_upper++;
            else if (is_lower(c)) cnt_lower++;
            else if (is_digit(c)) cnt_digit++;
            else if (is_special(c)) cnt_special++;
        }

        long long ans = 0;
        for (char c : s) {
            if (is_upper(c)) {
                if (cnt_upper >= 2) ans += 65;
                else ans += 26 - 1;
            } 
            else if (is_lower(c)) {
                if (cnt_lower >= 2) ans += 65;
                else ans += 26 - 1;
            } 
            else if (is_digit(c)) {
                if (cnt_digit >= 2) ans += 65;
                else ans += 10 - 1;
            } 
            else if (is_special(c)) {
                if (cnt_special >= 2) ans += 65;
                else ans += 4 - 1;
            }
        }

        cout << ans << "\n";
    }

    return 0;
}
//D 最短路。从1无权bfs, 到每个点的距离 d[i], ans = d[i] (i from 1 to n)
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
constexpr i64 mod = 1000000007;
int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int n, m;
  cin >> n >> m;
  vector<vector<int>> g(n);
  for (int i = 0, u, v; i < m; i += 1) {
    cin >> u >> v;
    u -= 1;
    v -= 1;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  queue<int> q;
  q.push(0);
  vector<int> d(n, -1);
  d[0] = 0;
  while (not q.empty()) {
    int u = q.front();
    q.pop();
    for (int v : g[u]) {
      if (d[v] == -1) {
        d[v] = d[u] + 1;
        q.push(v);
      }
    }
  }
  i64 val = 0, cnt = 1;
  for (int i = 1; i < n; i += 1) {
    val += d[i];
    int c = 0;
    for (int j : g[i]) {
      if (d[j] + 1 == d[i]) { c += 1; }
    }
    i64 p = 1;
    for (int i = 0; i < c; i += 1) {
      cnt = cnt * (mod + 1) / 2 % mod;
      p = p * 2 % mod;
    }
    cnt = cnt * (p - 1) % mod;
  }
  for (int i = 0; i < m; i += 1) { cnt = cnt * 2 % mod; }
  cout << val % mod << " " << cnt;
}
//E

