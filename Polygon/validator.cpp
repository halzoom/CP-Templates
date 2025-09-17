#include <bits/stdc++.h>
#include "testlib.h"

using namespace std;

string set_name(string _, int id) {
    return (_ + "[" + to_string(id) + "]");
}

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(1, 4000, "n-str_size");
    inf.readEoln();
    for(int i = 0 ; i < n ; ++i){
        const string ptrn = "[a-z]{" + to_string(n) + "}";
        string s = inf.readWord(ptrn, "s-string");
        inf.readSpace();
    }
    inf.readEoln();
    const string ptrn = "[a-z]{" + to_string(n) + "}";
    string s = inf.readWord(ptrn, "s-string");
    inf.readEoln();
    inf.readEof();
}

#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;

using ll = long long;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int n = inf.readInt(1, 4000, "n");
    inf.readEoln();
    for (int i = 0; i < n; ++i) {
        string c = inf.readWord();
        inf.readSpace();
    }
    inf.readEoln();
    int T = inf.readInt(1, 4000, "T");
    inf.readEoln();
    string c = inf.readWord();
    inf.readEoln();
    inf.readEof();
}
