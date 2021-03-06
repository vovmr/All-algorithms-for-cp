#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define all(x)              (x).begin(), (x).end()
#define inf                 1e18
#define PI                  -2 * atan2(-1, 0)

using namespace std;

vector <ll> ar, t, add;

void build(int v, int vl, int vr)
{
    if (vl == vr)
    {
        t[v] = ar[vl];
        return;
    }
    int m = (vl + vr) >> 1;
    build(2 * v + 1, vl, m);
    build(2 * v + 2, m + 1, vr);
    t[v] = t[2 * v + 1] + t[2 * v + 2];
}

void push(int v, int vl, int vr)
{
    if (add[v])
    {
        if (vl != vr)
        {
            add[2 * v + 1] += add[v];
            add[2 * v + 2] += add[v];
        }
        t[v] += (vr - vl + 1) * add[v];
        add[v] = 0;
    }
}

ll sum(int v, int vl, int vr, int l, int r)
{
    push(v, vl, vr);
    if (l > r)
        return 0;
    else if (vl == l && vr == r)
        return t[v];
    int m = (vl + vr) >> 1;
    ll a = sum(2 * v + 1, vl, m, l, min(r, m));
    ll b = sum(2 * v + 2, m + 1, vr, max(l, m + 1), r);
    return (a + b);
}

void upd(int v, int vl, int vr, int l, int r, ll x)
{
    push(v, vl, vr);
    if (l > r)
        return;
    else if (vl == l && vr == r)
    {
        add[v] += x;
        push(v, vl, vr);
        return;
    }
    int m = (vl + vr) >> 1;
    upd(2 * v + 1, vl, m, l, min(r, m), x);
    upd(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
    t[v] = t[2 * v + 1] + t[2 * v + 2];
}

vector <ll> fenw;

void upd_fenw(int j, ll x)
{
    for (int i = j; i < fenw.size(); i += (i & (-i)))
        fenw[i] += x;
}

ll sum_fenw(int j)
{
    ll ans = 0;
    for (int i = j; i > 0; i -= (i & (-i)))
        ans += fenw[i];
    return ans;
}

vector <int> p, rnk;

void make(int v)
{
    p[v] = -1;
    rnk[v] = 1;
}

int par(int v)
{
    if (p[v] == -1)
        return v;
    else
        return (p[v] = par(p[v]));
}

void un(int a, int b)
{
    a = par(a); b = par(b);
    if (a != b)
    {
        if (rnk[a] > rnk[b])
            swap(a, b);
        p[a] = b;
        rnk[b] += rnk[a];
    }
}

struct edge
{
    ll w;
    int u;
    edge(int u, ll w) { this->u = u; this->w = w; }
};

vector <ll> dist;
vector <bool> used;
vector < vector <edge> > gr;

void dfs(int v)
{
    used[v] = 1;
    for (auto to : gr[v])
    {
        if (!used[to.u])
        {
            dfs(to.u);
        }
    }
}

void dfs_mx(int v) /// finds the maximum path in graph without visiting a single vertex twice
{
    used[v] = 1;
    for (auto to : gr[v])
    {
        if (!used[to.u])
        {
            dist[to.u] = dist[v] + to.w;
            dfs_mx(to.u);
        }
    }
}

void dijkstra(int s)
{
    for (auto &i : dist)
        i = inf;
    dist[--s] = 0;
    set < pair <ll, int> > st;
    st.insert(make_pair(0, s));
    while (!st.empty())
    {
        int v = (*st.begin()).second;
        st.erase(st.begin());
        for (auto to : gr[v])
        {
            ll w = to.w;
            int u = to.u;
            if (dist[u] > dist[v] + w)
            {
                dist[u] = dist[v] + w;
                st.insert(make_pair(dist[u], u));
            }
        }
    }
}

struct st
{
    ll w;
    int v, u;
    st(int v, int u, ll w) : v(v), u(u), w(w) {}
};

int n, m;

vector <st> gr1;

vector <ll> d(int s)
{
    int tf = 0;
    vector <ll> dist(n, inf);
    dist[--s] = 0;
    for (int i = 0; i < n; ++i)
    {
        tf = 0;
        for (int j = 0; j < gr1.size(); ++j)
        {
            if (dist[gr1[j].v] > dist[gr1[j].u] + gr1[j].w)
            {
                dist[gr1[j].v] = dist[gr1[j].u] + gr1[j].w;
                tf = 1;
            }
        }
        if (!tf)
            break;
    }
    return dist;
}

vector <int> prefix_function(string &s)
{
    int n = s.size();
    vector <int> pref(n);
    for (int i = 1; i < n; ++i)
    {
        int j = p[i - 1];
        while (s[i] != s[j] && j > 0)
            j = p[j - 1];
        if (s[i] == s[j])
            ++j;
        p[i] = j;
    }
    return p;
}

vector <int> z_function(string &s)
{
    int n = s.size();
    vector <int> z(n);
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i)
    {
        if (i <= r)
            z[i] = min(z[i - l], r - i + 1);
        else
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
                ++z[i];
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}

vector <int> eratosphene(int n)
{
    vector <int> prime(2 * n); /// 0 - prime, 1 - not prime
    prime[0] = prime[1] = 1;
    for (ll i = 2; i * i <= 2 * n; ++i)
        if (!prime[i])
            if (i * i <= 2 * n)
                for (ll j = i * i; j <= 2 * n; j += i)
                    prime[j] = 1;
    return prime;
}

ll bin_pow(ll a, ll n)
{
    ll res = 1;
    while (n)
    {
        if (n & 1)
            res *= a;
        a *= a;
        n >>= 1;
    }
    return res;
}

ll hsh(string &s, ll mod = 1e9 + 7)
{
    ll t = 0;
    ll prime = 101;
    vector <ll> pows(s.size() + 1);
    pows[0] = 1; pows[1] = prime;
    for (int i = 2; i < s.size() + 1; ++i)
        pows[i] = (pows[i - 1] * prime) % mod;
    for (int i = 0; i < s.size(); ++i)
    {
        t += ((s[i] - 'a' + 1) * pows[i]) % mod;
        t %= mod;
    }
    return t % mod;
}

ll gcd(ll a, ll b)
{
    while (a && b)
    {
        if (a >= b)
            a %= b;
        else
            b %= a;
    }
    return (a + b);
}

typedef complex <double> cd;

vector <cd> fft(vector <cd> &ar)
{
    int n = (int)ar.size();
    if (n == 1)
        return vector <cd> (1, ar[0]);
    vector <cd> w(n, 1);
    for (int i = 1; i < n; ++i)
    {
        double a = 2.0 * PI * i / (double)n;
        w[i] = cd(cos(a), sin(a));
    }
    vector <cd> a(n >> 1), b(n >> 1);
    for (int i = 0; i < (n >> 1); ++i)
    {
        a[i] = ar[2 * i];
        b[i] = ar[2 * i + 1];
    }
    vector <cd> res(n);
    a = fft(a), b = fft(b);
    for (int i = 0; i < n; ++i)
    {
        res[i] = a[i % (n / 2)] + w[i] * b[i % (n / 2)];
    }
    return res;
}

//vector <cd> fft(vector <cd> &v)
//{
//    int n = v.size(), k = 1;
//    while ((1 << (k * 1LL)) < n * 1LL)
//        ++k;
//    int high = -1;
//    vector <int> rev(n);
//    for (int i = 1; i < n; ++i)
//    {
//        high += (!(i & (i - 1)));
//        rev[i] = rev[i ^ (1 << high)];
//        rev[i] |= (1 << (k - high - 1));
//    }
//    vector <cd> w(n);
//    for (int i = 0; i < n; ++i)
//    {
//        double a = 2.0 * PI * i / n;
//        w[i] = cd(cos(a), sin(a));
//    }
//    vector <cd> cur(n);
//    for (int i = 0; i < n; ++i)
//    {
//        cur[i] = v[rev[i]];
//    }
//    for (int len = 1; len < n; len <<= 1)
//    {
//        vector <cd> now(n);
//        int st = w.size() / (2 * len);
//        for (int beg = 0; beg < n;)
//        {
//            int h = beg;
//            for (int i = 0; i < len; ++i)
//            {
//                cd val = w[i * st] * cur[h + len];
//                now[beg] = cur[h] + val;
//                now[beg + len] = cur[h] - val;
//                ++beg, ++h;
//            }
//            beg += len;
//        }
//        cur.swap(now);
//    }
//    return cur;
//}

vector <cd> fft_rev(vector <cd> &as)
{
    vector <cd> res = fft(as);
    for (int i = 0; i < (int)res.size(); i++)
        res[i] /= as.size();
    reverse(res.begin() + 1, res.end());
    return res;
}

int main()
{
    ios_base::sync_with_stdio(false);
//    ifstream cin("input.txt");
//    ofstream cout("output.txt");
    int n;
    cin >> n;
    vector <cd> a(n), b(n);
    for (int i = 0; i < n; ++i)
    {
        int c;
        cin >> c;
        a[i] = c;
    }
    for (int i = 0; i < n; ++i)
    {
        int c;
        cin >> c;
        b[i] = c;
    }
    while (n & (n - 1))
    {
        a.push_back(0);
        b.push_back(0);
        ++n;
    }
    cout << n << endl;
    int g = n;
    while (g--)
    {
        a.push_back(0);
        b.push_back(0);
        ++n;
    }
    a = fft(a), b = fft(b);
    vector <cd> res(n);
    for (int i = 0; i < n; ++i)
    {
        res[i] = a[i] * b[i];
    }
    res = fft_rev(res);
    for (int i = 0; i < n; ++i)
    {
        if (res[i].real() < 0.0001)
            continue;
        cout << res[i].real() << " ";
    }
}
