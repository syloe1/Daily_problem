//https://ac.nowcoder.com/acm/contest/66943
//A
int main() {
    string s;cin>>s;
    i64 c = 0;
    int n = s.size();
    for(int i = 1; i < s.size(); i++) {
        int l = s[i-1] - '0';
        int r = s[n-1] - '0';
        if((l +r) % 2 == 0) {
            c++;
        }
    }cout<<c<<"\n";
    return 0;
}
//B
int main() {
    string s;cin>>s;
    int ans = INT_MAX;
    for(char tar = 'a'; tar <= 'z'; tar++) {
        int tot = 0;
        for(char c : s) {
            int dif = abs(c-tar);
            tot += min(dif, 26-dif);
        }
        ans = min(ans, tot);
    }cout<<ans<<'\n';
    return 0;
}
//C

//D
/*
假设当前节点 u 有子节点 v，边权为 w (u,v)：
当 u 未被选中（dp [u][0]）：u 可以选择是否与某个子节点 v 连一条红边。若选择连 v，则 v 不能再参与其他边；若不连，则 v 自己在其子树中随意选择。因此：dp [u][0] = 求和（对所有子节点 v）max (dp [v][0], dp [v][1])
当 u 被某条边选中（dp [u][1]）：需要尝试选择一条边 (u,v)，此时的转移方程为：dp [u][1] = 取最大值（对所有子节点 v）[ dp [u][0] - max (dp [v][0], dp [v][1]) + dp [v][0] + w (u,v) ]
*/
struct Edge {
    int to;
    long long w;
};
const int N = 100005;
vector<Edge> g[N];
long long dp[N][2];
bool vis[N];

void dfs(int u, int parent) {
    dp[u][0] = 0;
    dp[u][1] = 0;
    for (auto [v, w] : g[u]) {
        if (v == parent) continue;
        dfs(v, u);
        dp[u][0] += max(dp[v][0], dp[v][1]);
    }
    for (auto [v, w] : g[u]) {
        if (v == parent) continue;
        long long candidate = dp[u][0] - max(dp[v][0], dp[v][1]) + dp[v][0] + w;
        dp[u][1] = max(dp[u][1], candidate);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    for (int i = 1; i < n; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    dfs(1, -1);
    cout << max(dp[1][0], dp[1][1]) << "\n";
    return 0;
}
