ll exgcd(ll a, ll b, ll& x, ll& y){
    if(b == 0){ x = 1; y = 0; return a >= 0 ? a : -a; }
    ll x1, y1;
    ll d = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return d;
}

static inline ll norm(ll a, ll m){
    m = llabs(m);
    a %= m;
    if(a < 0) a += m;
    return a;
}

ll crt(const vector<ll>& a, const vector<ll>& m){
    int n = (int)a.size();
    if(n == 0) return 0;

    ll a1 = a[0], m1 = m[0];
    m1 = llabs(m1);
    a1 = norm(a1, m1);

    for(int i = 1; i < n; ++i){
        ll a2 = norm(a[i], m[i]), m2 = std::llabs(m[i]);
        ll p, q;
        ll d = exgcd(m1, m2, p, q);     
        ll diff = a2 - a1;
        if(diff % d != 0){
            return -1;     
        }

        ll mod = m2 / d;
        i128 t = (i128)(diff / d) * p;
        ll k = (ll)((t % mod + mod) % mod);

        a1 = (ll)((i128)a1 + (i128)k * m1);
        i128 M = (i128)(m1 / d) * m2;
        a1 %= (ll)M;
        if(a1 < 0) a1 += (ll)M;
        m1 = (ll)M;
    }
    return a1;
}
