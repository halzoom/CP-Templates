#include <bits/stdc++.h>

using namespace std;
#define int long long
const int N = 1e7 + 1e6 + 5;
using namespace std;

template<int sigma = 26, char mch = 'a'>
struct eertree {
    eertree(size_t q) : cnt(q + 2), len(q + 2), par(q + 2), qlink(q + 2) {
        q += 2;
        to.resize(q);
        link.resize(q);
        link[0].fill(1);
        qlink[0] = 1;
        len[1] = -1;
    }

    template<bool back = 1>
    static int get(auto const &d, size_t idx) {
        if (idx >= size(d)) {
            return -1;
        } else {
            return back ? rbegin(d)[idx] : d[idx];
        }
    }

    template<bool back = 1>
    static void push(auto &d, auto c) {
        back ? d.push_back(c) : d.push_front(c);
    }

    template<bool back = 1>
    static void pop(auto &d) {
        back ? d.pop_back() : d.pop_front();
    }

    template<bool back = 1>
    void add_letter(char c) {
        c -= mch;
        push<back>(s, c);
        int pre = get<back>(states, 0);
        int last = make_to<back>(pre, c);
        active += !(cnt[last]++);
        int D = 2 + len[pre] - len[last];
        while (D + len[pre] <= len[last]) {
            pop<back>(states);
            if (!empty(states)) {
                pre = get<back>(states, 0);
                D += get<back>(diffs, 0);
                pop<back>(diffs);
            } else {
                break;
            }
        }
        if (!empty(states)) {
            push<back>(diffs, D);
        }
        push<back>(states, last);
    }

    template<bool back = 1>
    void pop_letter() {
        int last = get<back>(states, 0);
        active -= !(--cnt[last]);
        pop<back>(states);
        pop<back>(s);
        vector<pair<int, int>> cands = {pair{qlink[last], len[last] - len[qlink[last]]}, pair{par[last], 0}};
        for (auto [state, diff]: cands) {
            if (empty(states)) {
                states = {state};
                diffs = {diff};
            } else {
                int D = get<back>(diffs, 0) - diff;
                int pre = get<back>(states, 0);
                if (D + len[state] > len[pre]) {
                    push<back>(states, state);
                    pop<back>(diffs);
                    push<back>(diffs, D);
                    push<back>(diffs, diff);
                }
            }
        }
        pop<back>(diffs);
    }

    void add_letter(char c, bool back) {
        back ? add_letter<1>(c) : add_letter<0>(c);
    }

    void pop_letter(bool back) {
        back ? pop_letter<1>() : pop_letter<0>();
    }

    int distinct() {
        return active;
    }

    template<bool back = 1>
    int maxlen() {
        return len[get<back>(states, 0)];
    }

private:
    vector<array<int, sigma>> to, link;
    vector<int> len, qlink, par, cnt;

    deque<char> s;
    deque<int> states = {0}, diffs;
    int sz = 2, active = 0;

    template<bool back = 1>
    int make_to(int last, int c) {
        if (c != get<back>(s, len[last] + 1)) {
            last = link[last][c];
        }
        if (!to[last][c]) {
            int u = to[link[last][c]][c];
            qlink[sz] = u;
            link[sz] = link[u];
            link[sz][get<back>(s, len[u])] = u;
            len[sz] = len[last] + 2;
            par[sz] = last;
            to[last][c] = sz++;
        }
        return to[last][c];
    }
};


vector<int> linear_phi(int n) {
    vector<int> lp(n + 1);
    vector<int> pr;
    vector<int> phi(n + 1);
    phi[1] = 1;
    for (int i = 2; i < n; ++i) {
        if (!lp[i]) {
            pr.push_back(i);
            phi[i] = i - 1;  // i is pr
        }
        for (int j = 0; j < pr.size() && i * pr[j] <= n; ++j) {
            lp[i * pr[j]] = true;
            if (i % pr[j] == 0) {
                phi[i * pr[j]] = phi[i] * pr[j];  // pr[j] divides i
                break;
            } else {
                phi[i * pr[j]] = phi[i] * phi[pr[j]];  // pr[j] does not divide i
            }
        }
    }
    return phi;
}

vector<int> phi;

void solve() {
    int q;
    cin >> q;
    string s;
    eertree tree(q);
    while (q--) {
        int t;
        cin >> t;
        if (t == 1) {
            int x;
            cin >> x;
            char c = char('a' + (phi[(s.size() + x)] % 26));
            s.push_back(c);
            tree.add_letter(c, 1);
        } else if (t == 2) {
            if (s.size()) {
                tree.pop_letter(1);
                s.pop_back();
            }
        } else {
            cout << tree.distinct() << '\n';
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int test = 1;
    cin >> test;
    phi = linear_phi(N);
    for (int i = 1; i <= test; ++i) {
        solve();
    }
    return 0;
}