//https://ac.nowcoder.com/acm/contest/65051
//A
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    i64 x, y, z;
    cin >> x >> y >> z;

    // 第一天施肥浇水
    if (x + y >= z) {
        cout << 1 << "\n";
        return 0;
    }

    // 已经完成的成长
    i64 growth = x + y;
    // 每3天（一个周期）的成长
    i64 cycleGrow = y + 3 * x;

    // 完整周期数
    i64 cycles = (z - growth) / cycleGrow;
    i64 days = 1 + 3 * cycles; // 每周期3天，第一天已用

    growth += cycles * cycleGrow;

    // 逐天补足
    while (growth < z) {
        days++;
        // 下一次施肥日在每个周期的第1天（mod 3 == 1）
        if ((days - 1) % 3 == 0)
            growth += x + y; // 施肥 + 浇水
        else
            growth += x;     // 仅浇水
    }

    cout << days << "\n";
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<string> grid(n);
    for (int i = 0; i < n; ++i) cin >> grid[i];

    string target = "meituan";
    int p = 0; // 当前匹配到 target[p]

    for (int i = 0; i < n && p < (int)target.size(); ++i) {
        // 判断这一行是否有 target[p]
        if (grid[i].find(target[p]) != string::npos) {
            p++; // 找到了，下一个目标字符
        }
    }

    cout << (p == (int)target.size() ? "YES" : "NO") << "\n";
    return 0;
}
//C
constexpr i64 mod = 1000000007;
int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  i64 ans = 1;
  int n, k;
  cin >> n >> k;
  vector<int> a(n);
  for (int &ai : a) { cin >> ai; }
  sort(a.rbegin(), a.rend());
  for (int i = 0; i < n and i < k + 1; i += 1) { ans = ans * a[i] % mod; }
  ans = (ans + k) % mod;
  for (int i = k + 1; i < n; i += 1) { ans = (ans + a[i]) % mod; }
  cout << ans;
}
//D
constexpr i64 mod = 1000000007;
int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int n;
  cin >> n;
  vector<int> a(n);
  for (int &ai : a) { cin >> ai; }
  i64 ans = 0;
  for (int i = 0; i < 30; i += 1) {
    array<i64, 2> sum = {0, 0};
    for (int j = 0; j < n; j += 1) {
      int k = (a[j] >> i) % 2;
      ans = (ans + (sum[k ^ 1] * (n - j) % mod << i)) % mod;
      sum[k] = (sum[k] + j + 1) % mod;
    }
  }
  cout << ans;
}