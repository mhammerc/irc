#include "hash_table.h"

static unsigned int		murmur3_32(const unsigned char* key, size_t len,
unsigned int seed)
{
  unsigned int h = seed;
  if (len > 3) {
    const unsigned int* key_x4 = (const unsigned int*) key;
    size_t i = len >> 2;
    do {
      unsigned int k = *key_x4++;
      k *= 0xcc9e2d51;
      k = (k << 15) | (k >> 17);
      k *= 0x1b873593;
      h ^= k;
      h = (h << 13) | (h >> 19);
      h = (h * 5) + 0xe6546b64;
    } while (--i);
    key = (const unsigned char*) key_x4;
  }
  if (len & 3) {
    size_t i = len & 3;
    unsigned int k = 0;
    key = &key[i - 1];
    do {
      k <<= 8;
      k |= *key--;
    } while (--i);
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    h ^= k;
  }
  h ^= len;
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}

unsigned int		ht_hash(char *key, size_t key_size)
{
	return (murmur3_32((unsigned char*)key, key_size, 42));
}