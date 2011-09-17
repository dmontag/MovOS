
#include "std.h"
#include "lock.h"

lock_t screen_lock = LOCK_UNLOCKED;

unsigned char _print_row = 0;
unsigned char _print_col = 0;

void print( char *str, unsigned int row, unsigned int col, 
        unsigned int color ) {

    unsigned int i = 0, j = 0;
    unsigned int offset;
    unsigned char ch;
    
    row &= 0xFF;
    col &= 0xFF;
    color &= 0xFF;

    do_lock( &screen_lock );

    if ( ( row == 0xFF ) && ( col == 0xFF ) ) {
    	row = _print_row;
	    col = _print_col;
	}
	    
	while ( 1 ) {
	
	    ch = str[i];
	    
	    /*
	    //str = (void *)0x12345678;
        *(char *)0xB80A0 = 0x30 + ( ( (unsigned int)str & 0xF0000000 ) >> 28 );
        *(char *)0xB80A2 = 0x30 + ( ( (unsigned int)str & 0x0F000000 ) >> 24 );
        *(char *)0xB80A4 = 0x30 + ( ( (unsigned int)str & 0x00F00000 ) >> 20 );
        *(char *)0xB80A6 = 0x30 + ( ( (unsigned int)str & 0x000F0000 ) >> 16 );
        *(char *)0xB80A8 = 0x30 + ( ( (unsigned int)str & 0x0000F000 ) >> 12 );
        *(char *)0xB80AA = 0x30 + ( ( (unsigned int)str & 0x00000F00 ) >> 8 );
        *(char *)0xB80AC = 0x30 + ( ( (unsigned int)str & 0x000000F0 ) >> 4 );
        *(char *)0xB80AE = 0x30 + ( ( (unsigned int)str & 0x0000000F ) >> 0 );
        */
        
	    if ( ch == 0 ) {
    	    break;
	    }
	        
	    if ( ch == '\n' ) {
	    
    	    // New line
	        row += 1;
	        col = 0;
	        
	    } else {
    	    
    	    if ( col > 80 ) {
        	    // Column wraps off edge of screen
    	        col = 0;
    	        row += 1;
    	    }
    	    
    	    if ( row > 25 ) {
    	        // Row wraps off bottom of screen
    	        row = 0;
    	    }
    	    
    	    offset = 0xB8000 + ( 2 * ( ( row * 80 ) + col ) );
	        
	        *(char *)offset = ch;
	        *(char *)( offset + 1 ) = color;
	        
	        col++;
	    }
	    
        i += 1;
        
	}

    _print_col = col;
    _print_row = row;

    do_unlock( &screen_lock );

}

void print_int( unsigned int num, unsigned int row, unsigned int col, 
        unsigned int color ) {

    char numstr[9] = {0, 0, 0, 0,  0, 0, 0, 0,  0};
    numstr[0] = 0x30 + ( ( num & 0xF0000000 ) >> 28 );
    numstr[1] = 0x30 + ( ( num & 0x0F000000 ) >> 24 );
    numstr[2] = 0x30 + ( ( num & 0x00F00000 ) >> 20 );
    numstr[3] = 0x30 + ( ( num & 0x000F0000 ) >> 16 );
    numstr[4] = 0x30 + ( ( num & 0x0000F000 ) >> 12 );
    numstr[5] = 0x30 + ( ( num & 0x00000F00 ) >> 8 );
    numstr[6] = 0x30 + ( ( num & 0x000000F0 ) >> 4 );
    numstr[7] = 0x30 + ( ( num & 0x0000000F ) >> 0 );
    print( numstr, row, col, color );
}

void print_set_cursor( unsigned int row, unsigned int col ) {

    _print_row = (char)row;
    _print_col = (char)col;

}

void print_move_cursor( int rowdiff, int coldiff ) {

	_print_row = (char)_print_row + (char)rowdiff;
	_print_col = (char)_print_col + (char)coldiff;

}

void print_clear_screen() {
    int row, col, offset;
    for ( row = 0; row < 25; row++ ) {
        for ( col = 0; col < 80; col++ ) {
            offset = 0xB8000 + ( 2 * ( ( row * 80 ) + col ) );
            *(char *)offset = 0;
	        *(char *)( offset + 1 ) = 0;
        }
    }
}
