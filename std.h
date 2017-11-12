#ifndef STD_H
#define STD_H

void print( char *str, unsigned int row, unsigned int col, 
        unsigned int color );
void print_int( unsigned int num, unsigned int row, unsigned int col, 
        unsigned int color );
void print_decimal_int( unsigned int num, unsigned int row, 
        unsigned int col, unsigned int color );
        
void print_set_cursor( unsigned int row, unsigned int col );
void print_move_cursor( int rowdiff, int coldiff );
void print_clear_screen();

#endif
