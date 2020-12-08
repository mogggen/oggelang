#ifndef UTIL_H
#define UTIL_H 

constexpr unsigned long hash_djb2(const char *str)
{
    unsigned long hash = 5381;
    int c = 0;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

constexpr unsigned long hash_djb2(unsigned long hash, const char *str)
{
    int c = 0;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

#endif /* UTIL_H */
