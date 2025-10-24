//https://ac.nowcoder.com/acm/contest/68338
//A
int main() {
    double n;int k;cin>>n>>k;
    int r = k / 2, c = k - r;
    double area = n * n / ((r+1.0) * (c+1.0));
    cout<<fixed<<setprecision(2)<<area<<'\n';
    return 0;
}
//B
int main(){
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;

    for (int i = 0; i < q; ++i) {
        int l, r;
        cin >> l >> r;
        string t;
        t.reserve(s.size() + (r - l + 1)); // 提前扩容以加速
        for (int j = 0; j < l - 1; ++j) t.push_back(s[j]);
        for (int j = l - 1; j < r; ++j) {
            t.push_back(s[j]);
            t.push_back(s[j]);
        }
        for (int j = r; j < (int)s.size(); ++j) t.push_back(s[j]);
        s.swap(t); // 替换
    }

    cout << s << "\n";
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;

int main() {    
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;
    vector<int> v(N);
    for (int i = 0; i < N; ++i) cin >> v[i];
    sort(v.begin(), v.end());

    vector<long long> pre(N + 1, 0);
    for (int i = 0; i < N; ++i) pre[i + 1] = pre[i] + v[i];

    const double INF = 1e18;
    double ans = INF;

    // i 表示从左边选 i 个，从右边选 K - i 个
    for (int i = 0; i <= K; ++i) {
        long long sum = pre[i] + pre[N] - pre[N - (K - i)];
        double avg = (double)sum / K;

        double mn = v[i];
        double mx = v[N - (K - i) - 1];
        mn = min(mn, avg);
        mx = max(mx, avg);

        ans = min(ans, mx - mn);
    }

    cout << fixed << setprecision(12) << ans << '\n';
    return 0;
}

//D
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1e18;

struct Edge {
    int to;
    ll w, d;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    ll h;
    cin >> n >> m >> h;

    struct Info { int u, v; ll w, d; };
    vector<Info> edges(m);
    ll maxW = 0;
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w >> edges[i].d;
        maxW = max(maxW, edges[i].w);
    }

    auto check = [&](ll limit) -> bool {
        vector<vector<Edge>> g(n + 1);
        for (auto &e : edges) {
            if (e.w >= limit) {
                g[e.u].push_back({e.v, e.w, e.d});
                g[e.v].push_back({e.u, e.w, e.d});
            }
        }

        vector<ll> dist(n + 1, INF);
        dist[1] = 0;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
        pq.push({0, 1});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;
            for (auto &e : g[u]) {
                if (dist[e.to] > dist[u] + e.d) {
                    dist[e.to] = dist[u] + e.d;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        return dist[n] <= h;
    };

    ll l = 1, r = maxW, ans = -1;
    while (l <= r) {
        ll mid = (l + r) / 2;
        if (check(mid)) {
            ans = mid;
            l = mid + 1;  // 尝试更大的承重
        } else {
            r = mid - 1;
        }
    }

    cout << ans << "\n";
    return 0;
}
