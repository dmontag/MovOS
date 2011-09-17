; el torito bootable cd rom

%define SECTOR_SIZE 0x800

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Preamble (system reserved)
times (SECTOR_SIZE*16) db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Primary volume descriptor

db 1                      ; 8000 BYTE   Volume Descriptor Type = 1
db 'CD001'                ; 8001 [5]    Standard Identifier = "CD001"
dw 1                      ; 8006 WORD   Volume Descriptor Version = 1

times 0x50-16 db 0x20
dd 0, 0

db 0xFF, 0, 0, 0          ; 8050 DWORD  Volume Space Size (sectors) = 15h
db 0, 0, 0, 0xFF          ; 8054 DWORD  Volume Space Size (big-endian)

times 0x20 db 0
db 1, 0                   ; 8078 WORD   Volume Set Size = 1
db 0, 1                   ; 807A WORD   Volume Set Size (big-endian)

db 1, 0                   ; 807C WORD   Volume Sequence Number = 1
db 0, 1                   ; 807E WORD   Volume Sequence Number (big-endian)

db 0, 8,                  ; 8080 WORD   Logical Block Size = 0800h
db 8, 0                   ; 8082 WORD   Logical Block Size (big-endian)

times 0x9C-0x82 db 0x22                          
times 0x22 db 0x33        ; 809C [22h]  Directory Record for Root Directory
;db 1                     ; 809C BYTE   Length of Directory Record = 1

db 2                      ; 80B5 BYTE   File Flags = 02h (Directory)

db 1, 0                   ; 80B8 WORD   Volume Sequence Number = 1
db 0, 1                   ; 80BA WORD   Volume Sequence Number (big-endian)
                          
dw 1                      ; 80BC WORD   Length of File Identifier = 1
                          ; 80BE [1]    File Identifier = {0}
times SECTOR_SIZE-0xBE db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Boot Record descriptor

; Offset 0x0 - Boot Record indicator
db 0

; Offset 0x1 - ISO-9660 id
db 'CD001'

; Offset 0x6 - Version
db 1

; Offset 0x7 - Name of specification
db 'EL TORITO SPECIFICATION'
times 9 db 0  ; Padding of string

; Offset 0x27 - NULL padding
times 0x20 db 0

; Offset 0x47 - Boot Catalog absolute address
dd SECTOR_SIZE*19
    
; Offset 0x4A - NULL padding
times (0x800-0x4A) db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Terminator descriptor

db 0xFF
db 'CD001'
times SECTOR_SIZE-6 db 0


