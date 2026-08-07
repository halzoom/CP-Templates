#include <bits/stdc++.h>
using namespace std;

struct Eertree {
    struct Node {
        int32_t len = 0, link = 0, occ = 0, firstPos = -1;
        array<int32_t,26> next;
        Node(int32_t len=0,int32_t link=0):len(len),link(link){ next.fill(-1); }
    };

private:
    struct WaveletMatrix {
        int32_t n = 0, lg = 0;
        vector<int32_t> z;
        vector<vector<int32_t>> pref;

        void clear(){
            n=lg=0;
            z.clear();
            pref.clear();
        }

        void build(vector<int32_t> a,int32_t sigma){
            clear();
            n=a.size();
            while((1LL<<lg)<max<int32_t>(1,sigma))lg++;
            lg=max<int32_t>(lg,1);
            z.resize(lg);
            pref.assign(lg,vector<int32_t>(n+1));
            vector<int32_t> nxt(n);
            for(int32_t lv=0;lv<lg;lv++){
                int32_t bit=lg-1-lv;
                for(int32_t i=0;i<n;i++)
                    pref[lv][i+1]=pref[lv][i]+(((a[i]>>bit)&1)==0);
                z[lv]=pref[lv][n];
                int32_t p0=0,p1=z[lv];
                for(int32_t x:a){
                    if((x>>bit)&1)nxt[p1++]=x;
                    else nxt[p0++]=x;
                }
                a.swap(nxt);
            }
        }

        int32_t countLess(int32_t l,int32_t r,int32_t x) const {
            if(l>=r || x<=0)return 0;
            if(x>=(1LL<<lg))return r-l;
            int32_t ans=0;
            for(int32_t lv=0;lv<lg;lv++){
                int32_t bit=lg-1-lv;
                int32_t zl=pref[lv][l],zr=pref[lv][r];
                if((x>>bit)&1){
                    ans+=zr-zl;
                    l=z[lv]+(l-zl);
                    r=z[lv]+(r-zr);
                }else{
                    l=zl;
                    r=zr;
                }
            }
            return ans;
        }

        int32_t count(int32_t l,int32_t r,int32_t x,int32_t y) const {
            if(l>=r || x>y)return 0;
            return countLess(l,r,y+1)-countLess(l,r,x);
        }
    };

    WaveletMatrix wm;
    vector<int32_t> tin,tout;
    bool rangeReady=false;

    int32_t getLink(int32_t v,int32_t pos,char ch) const {
        while(true){
            int32_t L=tree[v].len;
            if(pos-L-1>=0 && s[pos-L-1]==ch)return v;
            v=tree[v].link;
        }
    }

public:
    vector<Node> tree;
    vector<int32_t> suffAt;
    string s;
    int32_t suff=1;

    Eertree(){ clear(); }
    Eertree(const string &str){ clear(); build(str); }

    // Resets the tree in O(1) amortized.
    void clear(){
        tree.clear();
        tree.emplace_back(-1,0);
        tree.emplace_back(0,0);
        suffAt.clear();
        s.clear();
        suff=1;
        rangeReady=false;
        wm.clear();
        tin.clear();
        tout.clear();
    }

    // Appends ch and returns the node of the longest palindromic suffix in O(1) amortized for fixed alphabet.
    int extend(char ch){
        assert('a'<=ch && ch<='z');
        rangeReady=false;
        int32_t pos=s.size(),c=ch-'a';
        s+=ch;
        int32_t cur=getLink(suff,pos,ch);
        if(tree[cur].next[c]!=-1){
            suff=tree[cur].next[c];
            tree[suff].occ++;
            suffAt.push_back(suff);
            return suff;
        }
        int32_t v=tree.size();
        tree.emplace_back(tree[cur].len+2,0);
        tree[v].firstPos=pos;
        tree[v].occ=1;
        tree[cur].next[c]=v;
        if(tree[v].len==1)tree[v].link=1;
        else{
            int32_t p=getLink(tree[cur].link,pos,ch);
            tree[v].link=tree[p].next[c];
        }
        suff=v;
        suffAt.push_back(suff);
        return suff;
    }

    // Builds the tree for str in O(|str|).
    void build(const string &str){
        clear();
        for(char ch:str)extend(ch);
    }

    // Returns the transition P -> ch+P+ch, or -1 if it does not exist, in O(1).
    int go(int v,char ch) const {
        assert(0<=v && v<(int)tree.size() && 'a'<=ch && ch<='z');
        return tree[v].next[ch-'a'];
    }

    // Returns the node representing palindrome p, or -1 if p is not a palindrome present in the tree, in O(|p|).
    int findPalindrome(const string &p) const {
        if(p.empty())return 1;
        int32_t L=(p.size()-1)/2,R=p.size()/2;
        int32_t v=(p.size()&1)?0:1;
        while(L>=0){
            if(p[L]!=p[R])return -1;
            v=go(v,p[L]);
            if(v==-1)return -1;
            L--,R++;
        }
        return v;
    }

    // Returns the total occurrence count of every palindrome node in O(number of nodes).
    vector<int64_t> occurrences() const {
        int32_t m=tree.size();
        vector<vector<int32_t>> child(m);
        for(int32_t v=1;v<m;v++)child[tree[v].link].push_back(v);
        vector<int32_t> order;
        order.reserve(m);
        vector<int32_t> st={0};
        while(!st.empty()){
            int32_t v=st.back();st.pop_back();
            order.push_back(v);
            for(int32_t to:child[v])st.push_back(to);
        }
        vector<int64_t> cnt(m);
        for(int32_t v=0;v<m;v++)cnt[v]=tree[v].occ;
        for(int32_t i=(int32_t)order.size()-1;i>0;i--){
            int32_t v=order[i];
            cnt[tree[v].link]+=cnt[v];
        }
        return cnt;
    }

    // Preprocesses range occurrence queries in O(n log number_of_nodes) time and memory.
    void buildRangeQueries(){
        int32_t m=tree.size();
        vector<vector<int32_t>> child(m);
        for(int32_t v=1;v<m;v++)child[tree[v].link].push_back(v);
        tin.assign(m,0);
        tout.assign(m,0);
        vector<pair<int32_t,int32_t>> st;
        st.reserve(2*m);
        st.push_back({0,0});
        int32_t timer=0;
        while(!st.empty()){
            auto [v,state]=st.back();st.pop_back();
            if(!state){
                tin[v]=timer++;
                st.push_back({v,1});
                for(int32_t i=(int32_t)child[v].size()-1;i>=0;i--)
                    st.push_back({child[v][i],0});
            }else tout[v]=timer-1;
        }
        vector<int32_t> a(s.size());
        for(int32_t i=0;i<(int32_t)s.size();i++)a[i]=tin[suffAt[i]];
        wm.build(a,m);
        rangeReady=true;
    }

    // Counts occurrences of palindrome node v fully inside s[l..r] in O(log number_of_nodes); l,r are 1-based inclusive.
    int countInRange(int v,int l,int r) const {
        assert(rangeReady && 0<=v && v<(int)tree.size());
        if(v<2 || l>r)return 0;
        int32_t L=l+tree[v].len-1;
        if(L>r)return 0;
        return wm.count(L-1,r,tin[v],tout[v]);
    }

    // Checks whether palindrome node v occurs inside s[l..r] in O(log number_of_nodes); l,r are 1-based inclusive.
    bool existsInRange(int v,int l,int r) const {
        return countInRange(v,l,r)>0;
    }

    // Counts occurrences of palindrome p fully inside s[l..r] in O(|p|+log number_of_nodes).
    int countInRange(const string &p,int l,int r) const {
        int v=findPalindrome(p);
        return v==-1?0:countInRange(v,l,r);
    }

    // Checks whether palindrome p occurs inside s[l..r] in O(|p|+log number_of_nodes).
    bool existsInRange(const string &p,int l,int r) const {
        return countInRange(p,l,r)>0;
    }
};