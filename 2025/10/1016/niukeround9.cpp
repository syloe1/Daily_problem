//https://ac.nowcoder.com/acm/contest/63869
//每日一题
#include<bits/stdc++.h>
using namespace std;

// 125的倍数在最后三位可能的形式
string check[8] = {"500", "000", "750", "250", "125", "375", "625", "875"};

int tong[10]; // 统计每个数字的出现次数

int main(){
    string s;
    cin >> s;
    
    int sum = 0;
    // 统计数字出现次数并计算数字和
    for(int i = 0; i < s.length(); i++){
        tong[s[i] - '0']++;
        sum += s[i] - '0';
    }
    
    // 检查数字和是否能被3整除
    if(sum % 3 != 0){
        cout << -1;
        return 0;
    }
    
    // 尝试每种可能的后缀
    for(int i = 0; i < 8; i++){
        // 临时减去后缀所需的数字
        for(int j = 0; j < 3; j++){
            tong[check[i][j] - '0']--;
        }
        
        // 检查是否所有数字都够用
        bool valid = true;
        for(int j = 0; j <= 9; j++){
            if(tong[j] < 0){
                valid = false;
                break;
            }
        }
        
        if(valid){
            // 输出剩余数字（降序排列，避免前导零）
            for(int j = 9; j >= 0; j--){
                while(tong[j] > 0){
                    cout << j;
                    tong[j]--;
                }
            }
            // 输出后缀
            cout << check[i];
            return 0;
        }
        
        // 恢复数字计数
        for(int j = 0; j < 3; j++){
            tong[check[i][j] - '0']++;
        }
    }
    
    cout << -1;
    return 0;
}

//A
int main() {
    int t;cin>>t;while(t--) {
        int m, x;cin>>m>>x;cout<<(x-1)%m+1<<"\n";
    }
    return 0;
}
//B
int main() {
    int n;cin>>n;vector<i64> a(n);
    for(auto& e : a) cin>>e;
    i64 s = accumulate(a.begin(), a.end(), 0LL);
    i64 ans = s; 
    for(int i = 0; i + 1 < n; i++) {
        i64 ns = s - a[i] - a[i+1] + a[i] * a[i+1];
        ans = max(ans, ns);
    }cout<<ans<<'\n';
    return 0;
}
//C
int main() {
    string s;cin>>s;
    int n = s.size();
    i64 ans = 0;
    for(int i = 0; i < n; i++) {
        int dif1 = 0, dif2 = 0;
        for(int j = i; j < n; j++) {
           int len = j - i;
           char e1 = (len % 2 == 0 ? '0' : '1') ;
           char e2 = (len % 2 == 0 ? '1' : '0');
           if(s[j] != e1) dif1++;
           if(s[j] != e2) dif2++;
           ans += min(dif1, dif2);
        }
    }cout<<ans<<'\n';
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
int main() {
  int n; cin >> n;
  vector<int> a(n);
  for (auto &e: a)
    cin >> e;
  long long sm = 0;
  for (auto e: a)
    sm += e;
  if (sm % n == 0) {
    // max = n
    sm /= n;
    long long ans = 0;
    for (auto e: a)
      ans += abs(sm - e);
    cout << ans / 2 << endl;
  }
  else {
    // max = n - 1
    sort(a.begin(), a.end());
    auto duel = [&] (int l, int r) {
      long long u = 0;
      for (int i = l; i <= r; i ++)
        u += a[i];
      u /= (n - 1);
      auto dd = [&] (long long p) {
        long long pos = 0, neg = 0;
        for (int i = l; i <= r; i ++)
          if (a[i] >= p)
            pos += a[i] - p;
          else
            neg += p - a[i];
        return max(pos, neg);
      };
      return min(dd(u), dd(u + 1));
    };
    cout << min(duel(0, n - 2), duel(1, n - 1)) << endl;
  }
  return 0;
}