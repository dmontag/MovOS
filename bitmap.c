
#include "bitmap.h"
#include "std.h"

int bitmap_zero( bitmap_t *bitmap, unsigned int size ) {
    
    unsigned int bitmap_length = size / 8;
    unsigned int i;
    char *ptr = bitmap->map;
    
    for ( i = 0; i < bitmap_length; i++ ) {
        ptr[i] = 0;
    }
    
}

int bitmap_set( bitmap_t *bitmap, int bit ) {
    
    int byte;
    int offset;
    char *ptr = bitmap->map;
    
    byte = bit / 8;
    offset = bit % 8;
    
    ptr[byte] |= ( 1 << offset );
    
}

int bitmap_unset( bitmap_t *bitmap, int bit ) {
    
    int byte;
    int offset;
    char *ptr = bitmap->map;
    
    byte = bit / 8;
    offset = bit % 8;
    
    ptr[byte] &= ( 0xFF ^ ( 1 << offset ) );
    
}

int bitmap_find_free( bitmap_t *bitmap, int size ) {
    
    int bitmap_length = size / 8;
    int i, j;
    char *ptr = bitmap->map;
    
    for ( i = 0; i < bitmap_length; i++ ) {
        if ( (unsigned char)ptr[i] != 0xFF ) {
            for ( j = 0; j < 8; j++ ) {
                if ( ptr[i] & ( 0xFF ^ ( 1 << j ) ) == 0 ) {
                    return i*8 + j;
                }
            }
        }
    }
    
    return -1;
    
}
