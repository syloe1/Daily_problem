//https://ac.nowcoder.com/acm/contest/86?channelPut=w251acm
//A
int n, a[1005][1005];
int main()
{ 
    int cnt = 1;
    cin >> n;
 
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n - 1; j++)
            a[i][j] = cnt++;
 
    for (int i = 1; i <= n; i++)
        a[i][n] = cnt++;
 
    if (n >= 5)
        swap(a[1][n], a[n][1]);
 
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cout << a[i][j] << " \n"[j == n];
        }
    }
 
    return 0;
}

//B
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    cout << fixed << setprecision(7);
    
    while (T--) {
        double Px, Py, Ux, Uy, Vx, Vy;
        cin >> Px >> Py >> Ux >> Uy >> Vx >> Vy;

        // 向量 UV 和 UP
        double ux = Vx - Ux, uy = Vy - Uy;
        double px = Px - Ux, py = Py - Uy;

        double dot = px * ux + py * uy;
        double len2 = ux * ux + uy * uy;

        double t = dot / len2;

        double Qx = Ux + t * ux;
        double Qy = Uy + t * uy;

        cout << Qx << " " << Qy << "\n";
    }

    return 0;
}
//C
int main() {
    unordered_set<string> headers = {
        "algorithm", "bitset", "cctype", "cerrno", "clocale", "cmath", "complex",
        "cstdio", "cstdlib", "cstring", "ctime", "deque", "exception", "fstream",
        "functional", "limits", "list", "map", "iomanip", "ios", "iosfwd",
        "iostream", "istream", "ostream", "queue", "set", "sstream", "stack",
        "stdexcept", "streambuf", "string", "utility", "vector", "cwchar", "cwctype"
    };

    int T;
    cin >> T;
    while (T--) {
        string s;
        cin >> s;
        if (headers.count(s))
            cout << "Qian\n";
        else
            cout << "Kun\n";
    }
    return 0;
}
//D
const int N = 35;
vector<int> g[N];
bool vs[N];
void dfs(int u) {
    vs[u] = true;
    for(int v : g[u]) {
        if(!vs[v]) dfs(v);
    }
}
int main() {
    int t;cin>>t;while(t--) {
        int n,m;cin>>n>>m;
        for(int i = 1; i <= n; i++) {
            g[i].clear();
            vs[i] = false;
        }
        vector<int> deg(n+1, 0);
        for(int i = 0; i < m; i++) {
            int u,v;cin>>u>>v;
            g[u].push_back(v);
            g[v].push_back(u);
            deg[u]++; deg[v]++;
        }
        int st = -1;
        for(int i = 1; i <= n; i++) {
            if(deg[i] > 0) {
                st = i;
                break;
            }
        }
        if(st == -1) {
            cout<<"Xun\n";
            continue;
        }
        dfs(st);
        bool connected = true;
        for(int i = 1; i <= n; i++) {
            if(deg[i] > 0 && !vs[i]) {
                connected = false;
            }
        }
        int odd = 0, isolated = 0;
        for(int i = 1; i <= n; i++) {
            if(deg[i] % 2 == 1) odd++;
            if(deg[i] == 0) isolated++;
        }
        if(connected && odd <= 2 && isolated == 0) {
            cout<<"Zhen\n";
        } else {
            cout<<"Xun\n";
        }
    }
    return 0;
}
//E
int main() {
    int n, q;cin>>n>>q;
    vector<int> a(n+1);
    int xor_sum = 0;
    for(int i = 1; i <= n; i++) {
        cin>>a[i];
        xor_sum ^= a[i];
    }
    while(q--) {
        int x, y;cin>>x>>y;
        xor_sum ^= a[x]; xor_sum ^= y;
        a[x] = y;
        if(xor_sum != 0) cout<<"Kan\n";
        else cout<<"Li\n";
    }
    return 0;
}
//F
const int MAXN = 10005;
vector<int> edge[MAXN];
bool mark[MAXN];
void dfs(int x, int f) {
    mark[x] = 0; // 默认是必败态
    for (int v : edge[x]) {
        if (v == f) continue;
        dfs(v, x);
        if (mark[v] == 0) mark[x] = 1; // 有一个子节点是败态，则当前是胜态
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, r;
        cin >> n >> r;

        for (int i = 1; i <= n; ++i) edge[i].clear();

        for (int i = 1; i < n; ++i) {
            int u, v;
            cin >> u >> v;
            edge[u].push_back(v);
            edge[v].push_back(u);
        }

        dfs(r, 0);
        cout << (mark[r] ? "Gen" : "Dui") << '\n';
    }

    return 0;
}
//G
int main() {
    int N, M;
    cin >> N >> M;
    string standard;
    cin >> standard;

    string best_name;
    double best_score = -1.0;

    for (int i = 0; i < M; ++i) {
        string name, ans;
        cin >> name >> ans;
        int correct = 0;
        for (int j = 0; j < N; ++j) {
            if (ans[j] == standard[j]) correct++;
        }
        double score = (double)correct / N * 100.0;

        if (score > best_score ||
           (fabs(score - best_score) < 1e-9 && name < best_name)) {
            best_score = score;
            best_name = name;
        }
    }

    cout << best_name << "\n";
    cout << fixed << setprecision(2) << best_score << "\n";

    return 0;
}
//H
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1e15;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, P, K;
    cin >> N >> P >> K;

    vector<vector<pair<int, int>>> g(N + 1);
    for (int i = 0; i < N - 1; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    vector<ll> dist(N + 1, INF);
    vector<bool> vis(N + 1, false);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;

    dist[P] = 0;
    pq.push({0, P});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = true;

        for (auto [v, w] : g[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }

    vector<ll> dis;
    for (int i = 1; i <= N; ++i)
        if (i != P) dis.push_back(dist[i]);

    sort(dis.begin(), dis.end());
    cout << dis[K - 1] << "\n";

    return 0;
}
//I
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    long long T;
    if (!(cin >> N >> M >> T)) return 0;
    
    vector<pair<long long,int>> a(N), b(M); // (start_time, value)
    for (int i = 0; i < N; ++i) cin >> a[i].first >> a[i].second;
    for (int j = 0; j < M; ++j) cin >> b[j].first >> b[j].second;
    
    // 只保留 start < T 的事件（>=T 的对结果无影响）
    auto filter_and_sort = [&](vector<pair<long long,int>>& v){
        sort(v.begin(), v.end());
        vector<pair<long long,int>> out;
        out.reserve(v.size());
        for (auto &p : v) {
            if (p.first < T) out.push_back(p);
        }
        // Ensure there is a start at 0 (problem guarantees it), but if not, still safe handling:
        if (out.empty() || out[0].first != 0) {
            // (理论上题目保证有 0 开始的节目)
            out.insert(out.begin(), {0, 0});
        }
        return out;
    };
    
    auto A = filter_and_sort(a);
    auto B = filter_and_sort(b);

    // Collect breakpoints: all start times from both stages, 0, and T
    vector<long long> pts;
    pts.reserve(A.size() + B.size() + 2);
    pts.push_back(0);
    for (auto &p : A) pts.push_back(p.first);
    for (auto &p : B) pts.push_back(p.first);
    pts.push_back(T);
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    
    // Ensure 0 and T present
    if (pts.front() != 0) pts.insert(pts.begin(), 0);
    if (pts.back() != T) pts.push_back(T);
    
    // iterate over intervals [pts[i], pts[i+1])
    ll ans = 0;
    int ia = 0, ib = 0;
    // pointers to current program indices for A and B
    for (size_t i = 0; i + 1 < pts.size(); ++i) {
        long long l = pts[i], r = pts[i+1];
        if (l >= T) break; // out of range
        // move ia such that A[ia].first <= l < next start (or ia is last)
        while (ia + 1 < (int)A.size() && A[ia+1].first <= l) ++ia;
        while (ib + 1 < (int)B.size() && B[ib+1].first <= l) ++ib;
        int va = A[ia].second;
        int vb = B[ib].second;
        long long len = r - l;
        long long best = max(0, max(va, vb));
        ans += best * len;
    }
    
    cout << ans << "\n";
    return 0;
}
//J

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<ll> R(n);
    for (int i = 0; i < n; ++i) cin >> R[i];
    sort(R.begin(), R.end());

    // 交叉构造新排列
    deque<ll> dq;
    int l = 0, r = n - 1;
    while (l <= r) {
        if (l == r) {
            dq.push_back(R[l]);
            break;
        }
        dq.push_back(R[r--]);
        dq.push_back(R[l++]);
    }

    // 计算环形美值
    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ll a = dq[i];
        ll b = dq[(i + 1) % n];
        ans += abs(a - b);
    }

    cout << ans << "\n";
    return 0;
}
