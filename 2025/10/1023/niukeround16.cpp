//https://ac.nowcoder.com/acm/contest/67355
//A

int main() {
    int n;cin>>n;
    vector<i64> a(n);
    for(auto &e : a) cin>>e; 
    bool f = true;
    i64 dif = LLONG_MAX;
    for(int i = 0; i < n - 1; i++) {
        if(a[i] >= a[i+1]) {
            f = false;
            break;
        }
        i64 diff = a[i+1] - a[i];
        if(diff >= dif) {
            if(dif != LLONG_MAX) {
                f = false; break;
            }
        }
        dif = diff;
    }
    cout<<(f ? "Yes\n" : "No\n");
    return 0;
}
//B
int main() {
    int x, y;cin>>x>>y;
    int dis = x * x + y * y;
    int score = 0;
    for(int i = 1; i <= 10; i++) {
        if(dis <= i * i) {
            score = 11 - i;
            break;
        }
    }
    cout<<score<<'\n';
    return 0;
}
//C
#include<bits/stdc++.h>
using namespace std;

int main(){
  int n, h, a; cin >> n >> h >> a;
  vector<int> H(n); for (auto &x: H) cin >> x;
  vector<int> A(n); for (auto &x: A) cin >> x;
  map<int, int> M;
  for (int i = 0; i < n; i++) {
    int h = H[i], a = A[i];
    if (M.count(h)) M[h] = min(M[h], a);
    else M[h] = a;
  }
  vector<int> B;
  for (auto &[k, v]: M) if (k < h && v < a) {
    auto it = lower_bound(B.begin(), B.end(), v);
    if (it == B.end()) B.push_back(v);
    else *it = v;
  }
  cout << B.size() << endl;
}
//D
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> fa, sz;
    DSU(int n) : fa(n+1), sz(n+1,1) {
        iota(fa.begin(), fa.end(), 0);
    }
    int find(int x) {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    bool unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x,y);
        fa[y] = x;
        sz[x] += sz[y];
        return true;
    }
};

struct Edge {
    int u, v;
    long long w;
    int p, id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w >> edges[i].p;
        edges[i].id = i + 1;
    }

    DSU dsu(n);
    vector<int> chosen;

    // Step 1: 先加入所有必选边
    for (auto &e : edges) {
        if (e.p == 1) {
            dsu.unite(e.u, e.v);
            chosen.push_back(e.id);
        }
    }

    // Step 2: 对可选边按权值排序
    vector<Edge> opt;
    for (auto &e : edges)
        if (e.p == 0) opt.push_back(e);
    sort(opt.begin(), opt.end(), [](auto &a, auto &b){
        return a.w < b.w;
    });

    // Step 3: Kruskal 继续选边
    for (auto &e : opt) {
        if (dsu.unite(e.u, e.v)) {
            chosen.push_back(e.id);
        }
    }

    // Step 4: 判断是否连通
    int root = dsu.find(1);
    bool connected = true;
    for (int i = 2; i <= n; ++i)
        if (dsu.find(i) != root) {
            connected = false;
            break;
        }

    if (!connected) {
        cout << -1 << "\n";
        return 0;
    }

    // Step 5: 输出结果
    cout << chosen.size() << "\n";
    for (int i = 0; i < (int)chosen.size(); ++i) {
        cout << chosen[i] << (i+1 == chosen.size() ? '\n' : ' ');
    }

    return 0;
}
