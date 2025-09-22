//https://ac.nowcoder.com/acm/contest/116657
//A
#include <bits/stdc++.h>
using namespace std;
bool is(const string &s) {
    int fre[26] = {0};
    for(char c : s) fre[c-'a']++;
    vector<int> cnts; 
    for(int i = 0; i < 26; i++) {
        if(fre[i] > 0) {
            cnts.push_back(fre[i]);
        }
    }
    sort(cnts.begin(), cnts.end());
    for(int i = 1; i < cnts.size(); i++) {
        if(cnts[i] != cnts[i-1]+1) {
            return false;
        }
    }
    return true;
}   
int main() {
    int t;cin>>t;while(t--) {
        string s; int n;cin>>n>>s;
        if(is(s)) cout<<"YES\n";
        else cout<<"NO"<<'\n';
    }
}
//B
#include <bits/stdc++.h>
using namespace std;

// 判断一个数是否为质数
bool isPrime(int n) {
    if (n <= 1) return false; // 1不是质数
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// 判断一个数是否为合数
bool isComposite(int n) {
    return n > 1 && !isPrime(n);
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        int x;
        cin >> x;
        int y = -1;
        // 枚举 y
        for (int i = 1; i < x; i++) {
            int z = x ^ i; // 异或运算
            if (isComposite(z)) {
                y = i;
                break; // 找到一个即可
            }
        }
        cout << y << endl;
    }
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
int main() {
    int t;cin>>t;while(t--) {
        int n;cin>>n;
        if(n%2==1) {
            for(int i = 1; i <= n; i++) cout<<i<<(i==n?'\n':' ');
        } else{
            if(n==2) cout<<-1<<'\n';
            else{
                cout<<2<<' ';
                for(int i = n; i >= 1; i--) {
                    if(i != 2) cout<<i<<(i==1 ? '\n' : ' ');
                }
            }
        }
    }
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int N, M;
        cin >> N >> M;
        vector<int> A(N), B(M);
        for (int &x : A) cin >> x;
        for (int &x : B) cin >> x;

        auto compress = [&](vector<int> &v) {
            vector<pair<int,int>> p;
            for (int x : v) {
                if (p.empty() || p.back().first != x)
                    p.push_back({x,1});
                else
                    p.back().second++;
            }
            return p;
        };

        auto a = compress(A), b = compress(B);
        if (a.size() != b.size()) {
            cout << -1 << "\n";
            continue;
        }

        bool ok = true;
        int ans = 0;
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i].first != b[i].first || b[i].second < a[i].second) {
                ok = false;
                break;
            }
            int d = (b[i].second + a[i].second - 1) / a[i].second; // ceil division
            int e = 31 - __builtin_clz(d); // floor(log2(d))
            if ((1 << e) < d) e++;          // ceil(log2(d))
            ans = max(ans, e);
        }
        cout << (ok ? ans : -1) << "\n";
    }

    return 0;
}
//E
