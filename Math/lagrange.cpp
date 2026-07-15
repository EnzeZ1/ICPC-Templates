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