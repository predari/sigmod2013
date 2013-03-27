#ifndef CORE_H
#define CORE_H

struct Query
{
    MatchType       type;
    char            dist;
    char            numWords;
    Word*           words[MAX_QUERY_WORDS];
};

struct Document
{
    DocID           id;
    char            *str;
    IndexHashTable  *words;
    set<QueryID>    *matchingQueries;
};


/* <maria> */
template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
    template<typename S, typename T> struct hash<pair<S, T>>
    {
        inline size_t operator()(const pair<S, T> & v) const {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);
            return seed;
        }
    };
}

#endif
