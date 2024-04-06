#include <bits/stdc++.h>

using namespace std;

int main() {
    ifstream inf;
    ofstream ouf;
    stringstream data;
    inf.open("./0.json");
    data << inf.rdbuf();
    inf.close();
    for(int i=1;i<46;i++){
        ouf.open(string("./")+to_string(i)+string(".json"));
        ouf << data.str();
        ouf.close();
    }
    return 0;
}