#ifndef BITMAP_H
#define BITMAP_H

typedef struct bitmap_struct {
    char *map;
} bitmap_t;

int bitmap_zero( bitmap_t *bitmap, unsigned int bits );
int bitmap_set( bitmap_t *bitmap, int bit );
int bitmap_unset( bitmap_t *bitmap, int bit );
int bitmap_find_free( bitmap_t *bitmap, int size );

#endif
