//https://ac.nowcoder.com/acm/contest/63091
//每日一题 https://www.nowcoder.com/practice/86efbfbf3b6740459b530c80ef0301f2?channelPut=w251acm
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    vector<i64> a(n);
    for (auto &x : a) cin >> x;

    priority_queue<i64> maxHeap;
    priority_queue<i64, vector<i64>, greater<i64>> minHeap;

    for (i64 x : a) {
        if ((int)maxHeap.size() < k) {
            maxHeap.push(x);
        } else {
            if (x < maxHeap.top()) {
                minHeap.push(maxHeap.top());
                maxHeap.pop();
                maxHeap.push(x);
            } else {
                minHeap.push(x);
            }
        }
    }

    for (int i = 0; i < m; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            i64 x; cin >> x;
            if ((int)maxHeap.size() < k) {
                maxHeap.push(x);
            } else {
                if (x < maxHeap.top()) {
                    minHeap.push(maxHeap.top());
                    maxHeap.pop();
                    maxHeap.push(x);
                } else {
                    minHeap.push(x);
                }
            }
        } else if (op == 2) {
            if ((int)maxHeap.size() < k) cout << -1 << '\n';
            else cout << maxHeap.top() << '\n';
        }
    }

    return 0;
}
//A
int main() {
    int n,m;cin>>n>>m;
    vector<string> g(n);
    for(auto&e : g) cin>>e;
    int ans = 0;
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < m - 1; j++) {
            set<char> s;
            s.insert(g[i][j]);
            s.insert(g[i][j+1]);
            s.insert(g[i+1][j]);
            s.insert(g[i+1][j+1]);
            if(s.count('y') && s.count('o') && s.count('u')) ans++;
        }
    }
    cout<<ans<<'\n';
    return 0;
}
//B
/*
010101
101010 比较两个开销
*/
int main() {
    string s;cin>>s;
    i64 ca = 0, cb = 0;
    for(int i = 0; i < s.size(); i++) {
        char c = s[i];
        char ea = (i % 2 == 0 ? '0' : '1');
        char eb = (i % 2 == 0 ? '1' : '0');
        if(c != ea) ca += (i+1);
        if(c != eb) cb += (i+1);
    }cout<<min(ca, cb)<<'\n';
    return 0;
}
//C
int main() {
    int t;cin>>t;while(t--) {
        i64 a, b, l, r;cin>>a>>b>>l>>r;
        i64 diff = b - a;
        if(diff == 0) {
            cout<<"0 0\n";
            continue;
        }
        i64 k_min = (diff + r - 1) / r;
        i64 k_max = diff / l; 
        if(k_min > k_max) cout<<"-1\n";
        else cout<<k_min<<" "<<k_max<<"\n";
    }
    return 0;
}
//D
const int N = 100001;
int n, k, x, cc[40][40];ll ans;
int main(){scanf("%d%d", &n, &k);
	for (int i = 1, c1, c2;i <= n;i ++){
		scanf("%d", &x), c1 = c2 = 0;
		while (x % 2 == 0) x /= 2, c1 ++;
		while (x % 5 == 0) x /= 5, c2 ++;
		cc[c1][c2] ++;
	}
	for (int a = 0;a < 40;a ++)
	for (int b = 0;b < 40;b ++)
	if (min(a + a, b + b) >= k)
	ans += (cc[a][b]) * (cc[a][b] - 1ll) / 2;
	for (int a = 0;a < 40;a ++)
	for (int b = 0;b < 40;b ++)
	for (int c = a;c < 40;c ++)
	for (int d = a == c ? b + 1 : 0;d < 40;d ++)
	if (min(a + c, b + d) >= k)
	ans += 1ll * cc[a][b] * cc[c][d];
	printf("%lld", ans);
}