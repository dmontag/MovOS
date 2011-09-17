#include "time.h"

inline static unsigned char bcd_to_char( unsigned char bcd ) {
    return ( ( bcd & 0xF ) + ( ( bcd >> 4 ) & 0xF ) * 10 );
}

static unsigned char _get_reg( char i ) {
    char var;
    __asm__( 
        "movb %%al, %1\n"
        "outb %%al, $0x70\n"
        "inb $0x71, %%al\n"
        "movb %%al, %0"
        : "=r"( var )  // output
        : "r"( i )     // input
        : "%al"        // clobbered register
    );
    return var;
}

// Returns the number of seconds since Jan 1, 2000.
unsigned int time_2k() {
    unsigned int _bcd_date = bcd_date();
    unsigned int _bcd_time = bcd_time();
    unsigned int time_seconds, date_seconds, day, month, year, 
        days_this_year, days_previous_years = 0, days;
    int i;
    unsigned char days_in_month[12] = 
        {31, 30, 28, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Extract numbers from BCD time and date.
    time_seconds = bcd_to_char( ( _bcd_time >> 16 ) & 0xFF ) + 
        bcd_to_char( ( _bcd_time >> 8 ) & 0xFF ) * 60 + 
        bcd_to_char( _bcd_time & 0xFF ) * 3600;
    day = bcd_to_char( ( _bcd_date >> 16 ) & 0xFF );
    month = bcd_to_char( ( _bcd_date >> 8 ) & 0xFF );
    year = bcd_to_char( _bcd_date & 0xFF );
    
    // Adjust for leap day in february.
    if ( ( year % 400 != 0 ) && ( year & 0x3 == 0 ) ) {
        days_in_month[1] += 1;
    }
    
    // Add all days from the previous months.
    days_this_year = day;
    for ( i = 0; i < month - 1; i++ ) {
        days_this_year += days_in_month[i];
    }
    
    // Add all years since the year 2000 (00, actually).
    for ( i = 0; i < year; i++ ) {
        days_previous_years += 365;
        if ( ( year % 400 != 0 ) && ( year & 0x3 == 0 ) ) {
            days_previous_years += 1;
        }
    }
    
    // Total number of days since Jan 1, 2000.
    days = days_this_year + days_previous_years;
    
    date_seconds = days * 3600 * 24;
    
    return time_seconds + date_seconds;
}

// Returns the current time in the BCD format 00SSMMHH.
unsigned int bcd_time() {
    int res = 0;
    char i;

    for ( i = 0; i < 6; i += 2 ) {
        res <<= 8;
        res |= _get_reg( i );
    }

    return res;  
}

// Returns the current date in the BCD format 00DDMMYY.
unsigned int bcd_date() {
    int res = 0;
    char i;

    for ( i = 7; i < 10; i++ ) {
        res <<= 8;
        res |= _get_reg( i );
    }

    return res;  
}


