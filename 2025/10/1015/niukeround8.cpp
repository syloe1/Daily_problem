// 每日一题 https://www.nowcoder.com/practice/3053ad348a4c43ec9785fd684dbc3516?channelPut=w251acm
int main() {
	int n,m;cin>>n>>m;
	vector<int> a(n);
	for(auto& e : a) cin>>e;
	int damage = 2;
	i64 tot = 0;
	for(int k = 0; k < m; k++) {
		for(int i = 0; i < n; i++) {
			if(a[i] <= 0) continue;
			if(a[i] == damage) {
				a[i] = 0;
				damage++;
			} else {
				a[i] -= damage;
				if(a[i]<0) a[i] = 0;
			}
		}
        tot += damage;
	}cout<<tot<<'\n'; return 0;
}
// https://ac.nowcoder.com/acm/contest/63585
//A
int main() {
    int n;cin>>n;
    vector<int> a(n+1), pos(n+1);
    for(int i = 1; i <= n; i++) {
        cin>>a[i]; pos[a[i]] = i;
    }
    int x,y;cin>>x>>y;
    if(abs(pos[x] - pos[y]) == 1) {
        cout<<"Yes\n"; 
    } else {
        cout<<"No\n";
    }
    return 0;
}
//B
int main() {
    int n;cin>>n;
    vector<i64> a(n+1), pre(n+1, 0);
    for(int i = 1; i <= n; i++) {
        cin>>a[i];
        pre[i] = pre[i-1] + a[i];
    }
    int x, y;cin>>x>>y;
    if(x > y) swap(x, y);
    i64 sun = pre[y-1] - pre[x-1];
    i64 tot = pre[n];
    i64 ni = tot - sun;
    cout<<min(sun, ni)<<'\n';
    return 0;
}
//C
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int l = 1, r = n;
    vector<int> ans;

    while (l <= r) {
        ans.push_back(l++);
        if (l <= r) ans.push_back(r--);
    }

    for (int i = 0; i < n; i++) {
        cout << ans[i] << (i + 1 == n ? '\n' : ' ');
    }
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
bool check(i64 n) {
  i64 x = sqrt(n);
  while (x * x > n) {
    x -= 1;
  }
  while (x * x < n) {
    x += 1;
  }
  return x * x == n;
}
int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int n;
  cin >> n;
  vector<i64> a(n + 1);
  for (int i = 1; i <= n; i += 1) {
    cin >> a[i];
  }
  vector<vector<int>> g(n + 1);
  for (int i = 1, u, v; i < n; i += 1) {
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  int ans = 0;
  function<int(int, int)> dfs = [&](int u, int p) {
    int res = 1;
    for (int v : g[u]) {
      if (v != p) {
        int s = dfs(v, u);
        if (res and s and check(a[u] * a[v])) {
          ans += 1;
          res = 0;
        }
      }
    }
    return res;
  };
  dfs(1, 0);
  cout << ans * 2;
}