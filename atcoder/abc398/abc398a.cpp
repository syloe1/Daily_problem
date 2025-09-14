/*
如果n是奇数，中间放一个= 
n是偶数，中心放两个=
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;cin>>n;
    string s(n, '-');
    if(n % 2 == 1) {
        s[n/2] = '=';
    } else {
        s[n/2 - 1] = '=';
        s[n/2] = '=';
    }
    cout<<s<<'\n';
    return 0;
}