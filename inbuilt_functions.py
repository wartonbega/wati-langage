

print_func = """
malloc:
  ;; in rdx, there is the size needed to the alloc
  mov rdi, rdx
  push rdx
  call _malloc
  pop rdx
  ret

;;malloc_bis:
;;  ;; in rdx, there is the size needed to the alloc
;;  mov rsi, rdx                    ;; length
;;  mov rax, 0x20000c5              ;; mmap
;;  xor rdi, rdi                    ;; start address
;;  mov rdx, (0x01|0x02)            ;; rwx
;;  mov r10, (0x1000|0x0001)        ;; flags
;;  xor r8, r8                      ;; file descriptor
;;  xor r9, r9                      ;; offset
;;  syscall ;; the adress should be in rax
;;  ret

;;free: ;; Arguments: rdi = pointer to allocated memory, rsi = size
;;  mov rax, 0x200000b     ; syscall number for munmap
;;  syscall                ; invoke the munmap system call
;;  ret

chr_copy:
  ;; the source is in rsi, destination in rdi (modifies rsi, rdi)
  mov al, byte [rsi]        ; get byte, increment src ptr.
  test al, al
  jz chr_copy_end
  inc rsi
  mov byte [rdi], al        ; put byte, increment dst ptr.
  inc rdi
  jmp chr_copy
chr_copy_end:
  ret
  
_strlen: ;; the ptr should be in rdi
  push  rcx            ; save and clear out counter
  xor   rcx, rcx

_strlen_next:
  mov rsi, qword [rdi]
  cmp   qword rsi, 0  ; null byte yet?
  jz    _strlen_null   ; yes, get out
  inc   rcx            ; char is ok, count it
  lea   rdi, [rdi + 8]            ; move to next char
  jmp   _strlen_next   ; process again

_strlen_null:
  mov   rax, rcx       ; rcx = the length (put in rax)
  pop   rcx            ; restore rcx
  ret                  ; get out


"""