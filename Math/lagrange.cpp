long long lagrange(vector<long long>& x, vector<long long>& y, long long k){
    int n = y.size();
    k %= mod;

    if(k < n){
        return y[k]; 
    }

    long long ans = 0;
    for(int i = 0; i < n; i++){
        long long up = 1, down = 1;
        
        for(int j = 0; j < n; j++){
            if(i == j){
                continue;
            }
            up = up * ((k - x[j] + mod) % mod) % mod;
            down = down * ((x[i] - x[j] + mod) % mod) % mod;
        }

        long long cur = y[i] * up % mod * qpow(down, mod - 2, mod) % mod;
        ans = (ans + cur) % mod;
    }
    return ans;
}

long long lagrange(vector<long long>& y, long long k, long long mod){
    int n = y.size();

    k %= mod;

    if(k < n){
        return y[k];
    }

    vector<long long> fac(n), ifac(n);
    fac[0] = 1;
    for(int i = 1; i < n; i++){
        fac[i] = fac[i - 1] * i % mod;
    }

    ifac[n - 1] = qpow(fac[n - 1], mod - 2, mod);
    for(int i = n - 1; i >= 1; i--){
        ifac[i - 1] = ifac[i] * i % mod;
    }
 
    vector<long long> pre(n + 1, 1);
    for(int i = 0; i < n; i++){
        pre[i + 1] = pre[i] * ((k - i + mod) % mod) % mod;
    }
 
    vector<long long> suf(n + 1, 1);
    for(int i = n - 1; i >= 0; i--){
        suf[i] = suf[i + 1] * ((k - i + mod) % mod) % mod;
    }

    long long ans = 0;

    for(int i = 0; i < n; i++){
        long long up = pre[i] * suf[i + 1] % mod;
        long long coef = ifac[i] * ifac[n - 1 - i] % mod;
        long long cur = y[i] * up % mod * coef % mod;
 
        if((n - 1 - i) & 1){
            cur = (mod - cur) % mod;
        }
        ans += cur;
        if(ans >= mod) ans -= mod;
    }

    return ans;
}
