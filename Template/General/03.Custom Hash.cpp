#include<bits/stdc++.h>
using namespace std;
#define int long long

mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
// for strings
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(const string &s) const noexcept {
        static const uint64_t FIXED_RANDOM = chrono::high_resolution_clock::now().time_since_epoch().count();
        uint64_t x = std::hash<string>()(s) + FIXED_RANDOM + 0x9e3779b97f4a7c15ULL;
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        x = x ^ (x >> 31);
        return (size_t) x;
    }
};

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = rng();
        return splitmix64(x + FIXED_RANDOM);
    }
};

template<typename T>
using safe_set = unordered_set<T, custom_hash>;
template<typename T, typename V>
using safe_map = unordered_map<T, V, custom_hash>;

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const { // single integers
        static const uint64_t FIXED_RANDOM = rng();
        return splitmix64(x + FIXED_RANDOM);
    }

    template<size_t N>
    size_t operator()(const std::array<int, N> &arr) const {
        static const uint64_t FIXED_RANDOM = rng();
        uint64_t hash = FIXED_RANDOM;
        for (int x: arr) {
            hash ^=
                    splitmix64(static_cast<uint64_t>(x) + 0x9e3779b9 + (hash << 6) + (hash >> 2));
        }
        return hash;
    }
};