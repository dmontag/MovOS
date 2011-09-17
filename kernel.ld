
SECTIONS
{
    /DISCARD/ : {
        *(.comment);
    }
    . = 0x00100000;
    kernel_start = .;
    .init : {
        kerninit.o(.text);
        kerninit.o(.data);
        kerninit.o(.bss);
    }
    /* init:    8000-9000 */
    /* dir:     9000-A000 */
    /* low_tbl: A000-B000 */
    /* shd_tbl: B000-C000 */
    /* . = ALIGN( 0x1000 ); */
    .text : {
        kernel.o(.text);
        switch.o(.text);
        scheduler.o(.text);
        memory_manager.o(.text);
        std.o(.text);
        lock.o(.text);
        bitmap.o(.text);
        time.o(.text);
        queue.o(.text);
    }
    /* text:    C000-D000 */

    /* . = ALIGN( 0x1000 ); */
    .data : {
        kernel.o(.data);
        scheduler.o(.data);
        memory_manager.o(.data);
        std.o(.data);
        lock.o(.data);
        bitmap.o(.data);
        time.o(.data);
        queue.o(.data);
    }
    /*. = ALIGN( 0x1000 ); */
    .bss : {
        kernel.o(.bss);
        scheduler.o(.bss);
        memory_manager.o(.bss);
        std.o(.bss);
        lock.o(.bss);
        bitmap.o(.bss);
        time.o(.bss);
        queue.o(.bss);
        
        . = ALIGN( 0x1000 );
        pad.o(.text);
        pad.o(.data);
        pad.o(.bss);
    }
    /* data:    D000-E000 */

    /* . = ALIGN( 0x1000 ); */
    /* kernel_end = .; */
    /*
    .pad : {
        pad.o(.text);
        pad.o(.data);
        pad.o(.bss);
    }
    */
}
