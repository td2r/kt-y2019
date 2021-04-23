sys_exit:       equ             60

                section         .text
                global          _start

buf_size:       equ             8192
_start:
                xor             ebx, ebx
                sub             rsp, buf_size
                mov             rsi, rsp

                mov             r8, 1

read_again:
                xor             eax, eax
                xor             edi, edi
                mov             rdx, buf_size
                syscall

                test            rax, rax
                jz              quit
                js              read_error
                xor             ecx, ecx

check_char:
                cmp             rcx, rax
                je              read_again

                cmp             byte [rsi + rcx], 0x09
                je             skip_whitespace

                cmp             byte [rsi + rcx], 0x0a
                je             skip_whitespace

                cmp             byte [rsi + rcx], 0x0b
                je             skip_whitespace

                cmp             byte [rsi + rcx], 0x0c
                je             skip_whitespace

                cmp             byte [rsi + rcx], 0x0d
                je             skip_whitespace

                cmp             byte [rsi + rcx], 0x20
                je             skip_whitespace

                test            r8, r8
                jz              skip

                inc             rbx
                xor             r8, r8

skip:
                inc             rcx
                jmp             check_char

skip_whitespace:
                inc             r8
                inc             rcx
                jmp             check_char

quit:
                mov             rax, rbx
                call            print_int

                mov             rax, sys_exit
                xor             rdi, rdi
                syscall

; rax -- number to print
print_int:
                mov             rsi, rsp
                mov             ebx, 10
                dec             rsi
                mov             byte [rsi], 0x0a

next_char:
                xor             edx, edx
                div             ebx
                add             dl, '0'
                dec             rsi
                mov             [rsi], dl
                test            rax, rax
                jnz             next_char

                mov             eax, 1
                mov             edi, 1
                mov             rdx, rsp
                sub             rdx, rsi
                syscall

                ret

read_error:
                mov             eax, 1
                mov             edi, 2
                mov             rsi, read_error_msg
                mov             rdx, read_error_len
                syscall

                mov             rax, sys_exit
                mov             edi, 1
                syscall

                section         .rodata
read_error_msg: db              "read failure", 0x0a
read_error_len: equ             $ - read_error_msg
