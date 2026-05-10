using ull = unsigned long long;
using ui = unsigned int;

mt19937_64 rng{
    std::chrono::steady_clock::now().time_since_epoch().count()
};

ull mask = rng(), _hash[N];

ull shift(ull x) {
  x ^= mask;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  x ^= mask;
  return x;
}

// hash[u] = 1 + sum{v ∈ U} shift(hash[v]), hash[v] = 1 if v is leaf