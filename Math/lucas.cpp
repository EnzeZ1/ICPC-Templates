ll qpow(ll a, ll b){
    ll r = 1;
    while(b){
        if(b & 1) r = r * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return r;
}

ll C(ll n, ll k){           
    if(k < 0 || k > n) return 0;
    if(k > n - k) k = n - k;
    ll res = 1;

    for(ll i = 1; i <= k; ++i){
        res = res * (n - i + 1) % mod; 
        res = res * qpow(i, mod - 2) % mod;
    }
    return res;
}


ll lucas(ll n, ll k){
    if(k == 0) return 1;
    return C(n % mod, k % mod) * lucas(n / mod, k / mod) % mod;
}
 