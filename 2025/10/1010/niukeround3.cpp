//https://ac.nowcoder.com/acm/contest/61570
//A
#include <bits/stdc++.h>
using namespace std;

int main() {
    string x;
    cin >> x;
    long long best = LLONG_MAX;

    for (int pos = 0; pos <= (int)x.size(); ++pos) {
        for (char d = '0'; d <= '9'; ++d) {
            if (pos == 0 && d == '0') continue;

            string y = x.substr(0, pos) + d + x.substr(pos);
            long long num = stoll(y);

            if (num % 7 == 0) {
                best = min(best, num);
            }
        }
    }

    cout << best << endl;
}
//B
/*
枚举a-z, O(26n) ，顺序走dif，逆序26-dif 线性
*/
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    string s;cin>>s;
    int ans = INT_MAX;
    for(char tar = 'a'; tar <= 'z'; tar++) {
        int tot = 0;
        for(char c : s) {
            int dif = abs(c-tar);
            tot += min(dif, 26 - dif);
        }
        ans = min(ans, tot);
    }cout<<ans<<'\n';
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n, m, a, b;
    cin >> n >> m >> a >> b;

    long long ans = 0;
    // 枚举一号礼包的数量
    for (long long x = 0; x <= min(n / 2, m); ++x) {
        long long n_left = n - 2 * x;
        long long m_left = m - x;

        if (n_left < 0 || m_left < 0) continue;

        long long y = min(n_left, m_left / 2);
        ans = max(ans, a * x + b * y);
    }

    cout << ans << endl;
    return 0;
}
//D
