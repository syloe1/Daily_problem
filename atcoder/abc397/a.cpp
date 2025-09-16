#include <iostream>
using namespace std;
int main() {
    double c;
    cin>>c;
    if(c >= 38) cout<<"1";
    else if(c >= 37.5 && c < 38) cout<<"2";
    else if(c < 37.5) cout<<"3";
    return 0;
}