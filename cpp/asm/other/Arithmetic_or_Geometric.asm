
                section .text
                global _start


_start:
                mov             rsi, rsp
                sub             rsi, buf_size
                mov             rax, rcx
                xor             rbx, rbx
read_t:
                call            cchar
                inc             rcx

                cmp             dl, `\n`
                je              cycle

                add             rbx, rbx
                mov             r8, rbx
                add             rbx, rbx
                add             rbx, rbx
                add             rbx, r8

                sub             dl, '0'
                add             rbx, rdx
                jmp             read_t
cycle:
                test            rbx, rbx
                jz              exit
                dec             rbx

                finit
                fdisi

                call            read_real
                fdecstp
                fdecstp
                fld             st2
                fincstp
                fincstp
                fincstp

                call            read_real
                fdecstp
                fld             st1
                fincstp
                fincstp

                call            read_real
                fld             st0
                fincstp                         ; (st5, st6, st7) = (a, b ,c)

                xor             rdx, rdx        ; answer flag
check_arit:
                fdecstp
                fdecstp
                fdecstp
                ffree           st7
                fld             st0
                fadd            st3
                fsub            st2
                fsub            st2
                fabs                            ; st0 = |a + c - 2b|

                ffree           st7
                fld             qword [eps]

                fcomi           st1
                jb              check_geom
                inc             rdx
check_geom:
                fincstp
                fincstp
                ffree           st7
                fld             st0
                fmul            st3

                ffree           st7
                fld             st2
                fmul            st0
                fsub            st1
                fabs                            ; st0 = |ac - b**2|

                ffree           st7
                fld             qword [eps]
                fcomi           st1
                jb              choose_answer
                add             rdx, 2
choose_answer:
                test            rdx, rdx
                jz              answer_none

                dec             rdx
                test            rdx, rdx
                jz              answer_arithmetic

                dec             rdx
                test            rdx, rdx
                jz              answer_geometric

                dec             rdx
                test            rdx, rdx
                jz              answer_both

                jmp             assertion_error


read_real:      ; st0 = next real number
                push            qword 10
                ffree           st0
                fincstp
                fldz

                call            cchar
                xor             r8, r8          ; r8 = neg ? 1 : 0
                cmp             dl, '-'
                jne             build_int_part
                inc             r8
                inc             rcx
build_int_part:
                call            cchar
                inc             rcx

                cmp             dl, `\n`
                je              read_real_end
                cmp             dl, ' '
                je              read_real_end
                cmp             dl, '.'
                je              build_frac_part

                fimul           dword [rsp]
                sub             dl, '0'
                push            rdx
                fiadd           dword [rsp]
                add             rsp, 8
                jmp             build_int_part
build_frac_part:
                call            cchar
                inc             rcx

                cmp             dl, `\n`
                je              read_real_end
                cmp             dl, ' '
                je              read_real_end

                fincstp
                fincstp
                sub             dl, '0'
                push            rdx
                ffree           st7
                fild            qword [rsp]
                add             rsp, 8

                fidiv           dword [rsp]
                fdecstp
                fadd            st1

                mov             r9, [rsp]
                add             r9, r9
                mov             r10, r9
                add             r9, r9
                add             r9, r9
                add             r9, r10
                mov             [rsp], r9
                jmp             build_frac_part
read_real_end:
                test            r8, r8
                jz              read_real_ready
                fchs
read_real_ready:
                add             rsp, 8
                ret


cchar:          ; dl = current char in buffer
                cmp             rcx, rax
                jne             cchar_ready
read_in_buffer:
                xor             rax, rax
                xor             rdi, rdi
                mov             rdx, buf_size
                syscall

                test            rax, rax
                jz              read_error

                xor             rcx, rcx
cchar_ready:
                xor             rdx, rdx
                mov             dl, byte [rsi + rcx]
                ret


answer_none:
                push            rax
                push            rcx
                push            rsi
                mov             rsi, none_ans_msg
                mov             rdx, none_ans_len
                jmp             print_ans
answer_arithmetic:
                push            rax
                push            rcx
                push            rsi
                mov             rsi, arit_ans_msg
                mov             rdx, arit_ans_len
                jmp             print_ans
answer_geometric:
                push            rax
                push            rcx
                push            rsi
                mov             rsi, geom_ans_msg
                mov             rdx, geom_ans_len
                jmp             print_ans
answer_both:
                push            rax
                push            rcx
                push            rsi
                mov             rsi, both_ans_msg
                mov             rdx, both_ans_len
print_ans:
                mov             rax, 1
                mov             rdi, 1
                syscall
                pop             rsi
                pop             rcx
                pop             rax
                jmp             cycle


exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

read_error:
                mov             rax, 1
                mov             rdi, 1
                mov             rsi, read_error_msg
                mov             rdx, read_error_len
                syscall

                mov             rax, 60
                mov             rdi, 1
                syscall

assertion_error:
                mov             rax, 1
                mov             rdi, 1
                mov             rsi, assertion_error_msg
                mov             rdx, assertion_error_len
                syscall

                mov             rax, 60
                mov             rdi, 2
                syscall


                section .rodata

buf_size:       equ             8192

eps:            dq              1.e-6

none_ans_msg:   db              "None", 0x0a
none_ans_len:   equ             $ - none_ans_msg

arit_ans_msg:   db              "Arithmetic", 0x0a
arit_ans_len:   equ             $ - arit_ans_msg

geom_ans_msg:   db              "Geometric", 0x0a
geom_ans_len:   equ             $ - geom_ans_msg

both_ans_msg:   db              "Both", 0x0a
both_ans_len:   equ             $ - both_ans_msg

assertion_error_msg:            db              "Assertion error", 0x0a
assertion_error_len:            equ             $ - assertion_error_msg

read_error_msg: db              "Read erorr", 0x0a
read_error_len: equ             $ - read_error_msg
