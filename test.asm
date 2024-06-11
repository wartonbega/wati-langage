
default rel
section .text
    global _start

_start:
  call _big_or_little_endian
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  mov qword [__stdin], rax
  mov rax, 1
  push rax
  xor rax, rax
  pop rax
  mov qword [__stdout], rax
  mov rax, 2
  push rax
  xor rax, rax
  pop rax
  mov qword [__icanon], rax
  mov rax, 8
  push rax
  xor rax, rax
  pop rax
  mov qword [__echo], rax
  mov rax, 1
  push rax
  xor rdx, rdx
  pop rdx
  lea rdx, [rdx * 8]
  call malloc
  mov qword [__chr_tampon], rax
  mov rax, 1024
  push rax
  xor rax, rax
  pop rax
  mov qword [__taille_mem], rax
  call _alloue_block_memoire__
  push rax
  xor rax, rax
  pop rax
  mov qword [__memoire], rax
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  mov qword [__nul], rax
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  mov qword [__blocs_alloue], rax
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  mov qword [__blocs_libres], rax
  push qword [__taille_mem]
  xor rdx, rdx
  pop rdx
  lea rdx, [rdx * 8]
  call malloc
  mov qword [__section_mem], rax
  push qword [__taille_mem]
  xor rdx, rdx
  pop rdx
  lea rdx, [rdx * 8]
  call malloc
  mov qword [__section_libre], rax
  mov rax, 0
  call _ptrliste..ent...constructeur__
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrliste..ent...println__
  mov rax, 1
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrliste..ent...ajoute_ent_
  mov rax, 2
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrliste..ent...ajoute_ent_
  mov rax, 3
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrliste..ent...ajoute_ent_
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrliste..ent...println__
  mov rax, 2
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrliste..ent...index_ent_
  push rax
  xor rax, rax
  pop rax
  call _println_ent_
  add rsp, 8
  mov rax, 0
  call _ptrliste..chr...constructeur__
  push rax
  mov rax, 49
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrliste..chr...ajoute_chr_
  mov rax, 50
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrliste..chr...ajoute_chr_
  mov rax, 51
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrliste..chr...ajoute_chr_
  mov rax, 2
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrliste..chr...pop_ent_
  push rax
  xor rax, rax
  pop rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrliste..chr...println__
  add rsp, 8

  mov rax, 0x2000001
  mov rdi, r15
  syscall

_exit_ent8_:
  push rax
  push qword [rsp + 0]
  xor rdi, rdi
  pop rdi
  mov rax, 0x2000001
  syscall
  push rax
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_read_ent_liste_chr__ent_:
  push rax
  push rbx
  push rcx
  push qword [rsp + 16]
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rdx, rdx
  pop rdx
  xor rsi, rsi
  pop rsi
  xor rdi, rdi
  pop rdi
  mov rax, 0x2000003
  syscall
  push rax
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_write_ent_liste_chr__ent_:
  push rax
  push rbx
  push rcx
  push qword [rsp + 16]
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rdx, rdx
  pop rdx
  xor rsi, rsi
  pop rsi
  xor rdi, rdi
  pop rdi
  mov rax, 0x2000004
  syscall
  push rax
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_read_liste_chr__ent_ent_:
  push rax
  push rbx
  push rcx
  push qword [rsp + 16]
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rdx, rdx
  pop rdx
  xor rsi, rsi
  pop rsi
  xor rdi, rdi
  pop rdi
  mov rax, 0x2000005
  syscall
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ioctl_ent_ent_liste_chr__:
  push rax
  push rbx
  push rcx
  push qword [rsp + 16]
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rdx, rdx
  pop rdx
  xor rsi, rsi
  pop rsi
  xor rdi, rdi
  pop rdi
  mov rax, 0x2000036
  syscall
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_mmap_ptrrien_ent_ent_ent_ent_ent_:
  push rax
  push rbx
  push rcx
  push rdx
  push rsi
  push rdi
  push qword [rsp + 40]
  push qword [rsp + 40]
  push qword [rsp + 40]
  push qword [rsp + 40]
  push qword [rsp + 40]
  push qword [rsp + 40]
  xor r9, r9
  pop r9
  xor r8, r8
  pop r8
  xor r10, r10
  pop r10
  xor rdx, rdx
  pop rdx
  xor rsi, rsi
  pop rsi
  xor rdi, rdi
  pop rdi
  mov rax, 0x20000c5
  syscall
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_print_chr_:
  push rax
  mov rax, 0
  push rax
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [__chr_tampon]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [__stdout]
  push qword [__chr_tampon]
  mov rax, 1
  push rax
  xor rcx, rcx
  pop rcx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _write_ent_liste_chr__ent_
  xor r15, r15
  pop r15

  ret

_println_chr_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _print_chr_
  mov rax, 10
  push rax
  xor rax, rax
  pop rax
  call _print_chr_
  xor r15, r15
  pop r15

  ret

_print_liste_chr__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _long_liste_chr__
  push rax
  mov rax, 0
  push rax
LabelWhileBegin0:
  push qword [rsp + 0]
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt3
  mov rax, 1
  jmp LabelLt2
LabelLt3:
  mov rax, 0
LabelLt2:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd1
  push qword [rsp + 16]
  push qword [rsp + 8]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*1]
  push qword [rax]
  xor rax, rax
  pop rax
  call _print_chr_
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin0
LabelWhileEnd1:
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_input_liste_chr__ent_:
  push rax
  push rbx
  push qword [__stdin]
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rcx, rcx
  pop rcx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _read_ent_liste_chr__ent_
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_exit_ent_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  call _exit_ent8_
  xor r15, r15
  pop r15

  ret

_chr._plus_chr_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_init_liste_ent_:
  push rax
  mov rax, 0
  push rax
  push qword [rsp + 8]
  xor rdx, rdx
  pop rdx
  lea rdx, [rdx * 8]
  call malloc
  push rax
LabelWhileBegin4:
  push qword [rsp + 8]
  push qword [rsp + 24]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt7
  mov rax, 1
  jmp LabelLt6
LabelLt7:
  mov rax, 0
LabelLt6:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd5
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 0]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  jmp LabelWhileBegin4
LabelWhileEnd5:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_long_liste_chr__:
  push rax
  mov rax, 0
  push rax
LabelWhileBegin8:
  push qword [rsp + 8]
  push qword [rsp + 8]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*1]
  push qword [rax]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jne LabelNeq10
  mov rax, 0
  jmp LabelNeq11
LabelNeq10:
  mov rax, 1
LabelNeq11:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd9
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin8
LabelWhileEnd9:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_println_liste_chr__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _print_liste_chr__
  mov rax, 10
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  call _print_chr_
  xor r15, r15
  pop r15

  ret

_copie_ent_liste_chr__liste_chr__:
  push rax
  push rbx
  push rcx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  call _long_liste_chr__
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  call _long_liste_chr__
  push rax
  mov rax, 0
  push rax
LabelWhileBegin12:
  push qword [rsp + 0]
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt15
  mov rax, 1
  jmp LabelLt14
LabelLt15:
  mov rax, 0
LabelLt14:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd13
  push qword [rsp + 40]
  push qword [rsp + 32]
  push qword [rsp + 16]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*1]
  push qword [rax]
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 32]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 40]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 40], rax
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin12
LabelWhileEnd13:
  push qword [rsp + 32]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._ou_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  or rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._et_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  and rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._plus_petit_eg_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  jns LabelLt17
  mov rax, 1
  jmp LabelLt16
LabelLt17:
  mov rax, 0
LabelLt16:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._plus_grand_eg_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt19
  mov rax, 1
  jmp LabelMt18
LabelMt19:
  mov rax, 0
LabelMt18:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._pas_eg_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jne LabelNeq20
  mov rax, 0
  jmp LabelNeq21
LabelNeq20:
  mov rax, 1
LabelNeq21:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._egal_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq22
  mov rax, 0
  jmp LabelEq23
LabelEq22:
  mov rax, 1
LabelEq23:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._plus_petit_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt25
  mov rax, 1
  jmp LabelLt24
LabelLt25:
  mov rax, 0
LabelLt24:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._plus_grand_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt27
  mov rax, 1
  jmp LabelMt26
LabelMt27:
  mov rax, 0
LabelMt26:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._puissance_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._fois_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mul rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._div_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._plus_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._moins_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._modulo_ent32_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  mov rax, rdx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent32._negatif_ent32_:
  push rax
  mov rax, 0
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov ebx, eax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15

  ret

_ent16._ou_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  or rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._et_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  and rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._plus_petit_eg_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  jns LabelLt29
  mov rax, 1
  jmp LabelLt28
LabelLt29:
  mov rax, 0
LabelLt28:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._plus_grand_eg_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt31
  mov rax, 1
  jmp LabelMt30
LabelMt31:
  mov rax, 0
LabelMt30:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._pas_eg_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jne LabelNeq32
  mov rax, 0
  jmp LabelNeq33
LabelNeq32:
  mov rax, 1
LabelNeq33:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._egal_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq34
  mov rax, 0
  jmp LabelEq35
LabelEq34:
  mov rax, 1
LabelEq35:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._plus_petit_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt37
  mov rax, 1
  jmp LabelLt36
LabelLt37:
  mov rax, 0
LabelLt36:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._plus_grand_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt39
  mov rax, 1
  jmp LabelMt38
LabelMt39:
  mov rax, 0
LabelMt38:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._puissance_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._fois_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mul rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._div_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._plus_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._moins_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._modulo_ent16_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  mov rax, rdx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent16._negatif_ent16_:
  push rax
  mov rax, 0
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bx, ax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15

  ret

_ent8._ou_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  or rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._et_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  and rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._plus_petit_eg_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  jns LabelLt41
  mov rax, 1
  jmp LabelLt40
LabelLt41:
  mov rax, 0
LabelLt40:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._plus_grand_eg_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt43
  mov rax, 1
  jmp LabelMt42
LabelMt43:
  mov rax, 0
LabelMt42:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._pas_eg_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jne LabelNeq44
  mov rax, 0
  jmp LabelNeq45
LabelNeq44:
  mov rax, 1
LabelNeq45:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._egal_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq46
  mov rax, 0
  jmp LabelEq47
LabelEq46:
  mov rax, 1
LabelEq47:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._plus_petit_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt49
  mov rax, 1
  jmp LabelLt48
LabelLt49:
  mov rax, 0
LabelLt48:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._plus_grand_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt51
  mov rax, 1
  jmp LabelMt50
LabelMt51:
  mov rax, 0
LabelMt50:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._puissance_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._fois_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mul rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._div_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._plus_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._moins_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._modulo_ent8_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  mov rax, rdx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent8._negatif_ent8_:
  push rax
  mov rax, 0
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15

  ret

_puissance_dix_ent_:
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  push rax
LabelWhileBegin52:
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  push rax
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt55
  mov rax, 1
  jmp LabelMt54
LabelMt55:
  mov rax, 0
LabelMt54:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd53
  push qword [rsp + 8]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mul rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin52
LabelWhileEnd53:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_puissance_seize_ent_:
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  push rax
LabelWhileBegin56:
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  push rax
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt59
  mov rax, 1
  jmp LabelMt58
LabelMt59:
  mov rax, 0
LabelMt58:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd57
  push qword [rsp + 8]
  mov rax, 16
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mul rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin56
LabelWhileEnd57:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_puissance_dix_ent32_:
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  push rax
LabelWhileBegin60:
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent32._div_ent32_ent_
  push rax
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent32._plus_grand_ent32_ent_
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd61
  push qword [rsp + 8]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mul rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin60
LabelWhileEnd61:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_puissance_dix_ent16_:
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  push rax
LabelWhileBegin62:
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent16._div_ent16_ent_
  push rax
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent16._plus_grand_ent16_ent_
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd63
  push qword [rsp + 8]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mul rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin62
LabelWhileEnd63:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_puissance_dix_ent8_:
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  push rax
LabelWhileBegin64:
  push qword [rsp + 16]
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent8._div_ent8_ent_
  push rax
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent8._plus_grand_ent8_ent_
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd65
  push qword [rsp + 8]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mul rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin64
LabelWhileEnd65:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent_en_chaine_ent_:
  push rax
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq66
  mov rax, 0
  jmp LabelEq67
LabelEq66:
  mov rax, 1
LabelEq67:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label68
  mov rax, 1
  push rax
  xor rdx, rdx
  pop rdx
  lea rdx, [rdx * 8]
  call malloc
  push rax
  mov rax, 0
  push rax
  mov rax, 48
  push rax
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 0]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  add rsp, 8
  jmp LabelEnd69
Label68:
LabelEnd69:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _puissance_dix_ent_
  push rax
  push qword [rsp + 0]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  call _init_liste_ent_
  push rax
LabelWhileBegin70:
  push qword [rsp + 16]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt73
  mov rax, 1
  jmp LabelMt72
LabelMt73:
  mov rax, 0
LabelMt72:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd71
  push qword [rsp + 24]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  push rax
  push qword [rsp + 32]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  mov rax, rdx
  push rax
  push qword [rsp + 32]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 32], rax
  push qword [rsp + 32]
  mov rax, 48
  push rax
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 16]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  mov qword [rsp + 40], rax
  add rsp, 16
  jmp LabelWhileBegin70
LabelWhileEnd71:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_hex_ent_:
  push rax
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq74
  mov rax, 0
  jmp LabelEq75
LabelEq74:
  mov rax, 1
LabelEq75:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label76
  mov rax, qword msg0
  push rax
  mov rdx, 3
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  ret
  jmp LabelEnd77
Label76:
LabelEnd77:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _puissance_seize_ent_
  push rax
  push qword [rsp + 0]
  push qword [rsp + 8]
  mov rax, 2
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  call _init_liste_ent_
  push rax
  mov rax, 0
  push rax
  mov rax, 48
  push rax
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 0]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  mov rax, 1
  push rax
  mov rax, 120
  push rax
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 0]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
LabelWhileBegin78:
  push qword [rsp + 16]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt81
  mov rax, 1
  jmp LabelMt80
LabelMt81:
  mov rax, 0
LabelMt80:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd79
  push qword [rsp + 24]
  mov rax, 16
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  push rax
  push qword [rsp + 32]
  mov rax, 16
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  xor rdx, rdx
  div rbx
  mov rax, rdx
  push rax
  push qword [rsp + 32]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 32], rax
  push qword [rsp + 0]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt83
  mov rax, 1
  jmp LabelMt82
LabelMt83:
  mov rax, 0
LabelMt82:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label84
  push qword [rsp + 32]
  mov rax, 2
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  mov rax, 97
  push rax
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 16]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  jmp LabelEnd85
Label84:
  push qword [rsp + 32]
  mov rax, 2
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  mov rax, 48
  push rax
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 16]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  jmp LabelEnd85
LabelEnd85:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  mov qword [rsp + 40], rax
  add rsp, 16
  jmp LabelWhileBegin78
LabelWhileEnd79:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent_en_chaine_ent32_:
  push rax
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent32._egal_ent32_ent_
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label86
  mov rax, 1
  push rax
  xor rdx, rdx
  pop rdx
  lea rdx, [rdx * 8]
  call malloc
  push rax
  mov rax, 0
  push rax
  mov rax, 48
  push rax
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 0]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  add rsp, 8
  jmp LabelEnd87
Label86:
LabelEnd87:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _puissance_dix_ent32_
  push rax
  push qword [rsp + 0]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  call _init_liste_ent_
  push rax
LabelWhileBegin88:
  push qword [rsp + 16]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt91
  mov rax, 1
  jmp LabelMt90
LabelMt91:
  mov rax, 0
LabelMt90:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd89
  push qword [rsp + 24]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent32._div_ent32_ent_
  push rax
  push qword [rsp + 32]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent32._modulo_ent32_ent_
  push rax
  push qword [rsp + 32]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 32], rax
  push qword [rsp + 32]
  push qword [rsp + 8]
  mov rax, 48
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent32._plus_ent32_ent_
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 16]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 8]
  xor eax, eax
  pop rax
  mov dword [rsp + 40], eax
  add rsp, 16
  jmp LabelWhileBegin88
LabelWhileEnd89:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent_en_chaine_ent16_:
  push rax
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent16._egal_ent16_ent_
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label92
  mov rax, 1
  push rax
  xor rdx, rdx
  pop rdx
  lea rdx, [rdx * 8]
  call malloc
  push rax
  mov rax, 0
  push rax
  mov rax, 48
  push rax
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 0]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  add rsp, 8
  jmp LabelEnd93
Label92:
LabelEnd93:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _puissance_dix_ent16_
  push rax
  push qword [rsp + 0]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  call _init_liste_ent_
  push rax
LabelWhileBegin94:
  push qword [rsp + 16]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt97
  mov rax, 1
  jmp LabelMt96
LabelMt97:
  mov rax, 0
LabelMt96:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd95
  push qword [rsp + 24]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent16._div_ent16_ent_
  push rax
  push qword [rsp + 32]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent16._modulo_ent16_ent_
  push rax
  push qword [rsp + 32]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 32], rax
  push qword [rsp + 32]
  push qword [rsp + 8]
  mov rax, 48
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent16._plus_ent16_ent_
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 16]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 8]
  xor ax, ax
  pop rax
  mov word [rsp + 40], ax
  add rsp, 16
  jmp LabelWhileBegin94
LabelWhileEnd95:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ent_en_chaine_ent8_:
  push rax
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent8._egal_ent8_ent_
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label98
  mov rax, 1
  push rax
  xor rdx, rdx
  pop rdx
  lea rdx, [rdx * 8]
  call malloc
  push rax
  mov rax, 0
  push rax
  mov rax, 48
  push rax
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 0]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  add rsp, 8
  jmp LabelEnd99
Label98:
LabelEnd99:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _puissance_dix_ent8_
  push rax
  push qword [rsp + 0]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  call _init_liste_ent_
  push rax
LabelWhileBegin100:
  push qword [rsp + 16]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  inc rbx
  cmp rax, rbx
  js LabelMt103
  mov rax, 1
  jmp LabelMt102
LabelMt103:
  mov rax, 0
LabelMt102:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd101
  push qword [rsp + 24]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent8._div_ent8_ent_
  push rax
  push qword [rsp + 32]
  mov rax, 10
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent8._modulo_ent8_ent_
  push rax
  push qword [rsp + 32]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 32], rax
  push qword [rsp + 32]
  push qword [rsp + 8]
  mov rax, 48
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _ent8._plus_ent8_ent_
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 16]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 8]
  xor al, al
  pop rax
  mov byte [rsp + 40], al
  add rsp, 16
  jmp LabelWhileBegin100
LabelWhileEnd101:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_print_ent_:
  push rax
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt105
  mov rax, 1
  jmp LabelLt104
LabelLt105:
  mov rax, 0
LabelLt104:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label106
  mov rax, 45
  push rax
  xor rax, rax
  pop rax
  call _print_chr_
  mov rax, 0
  push rax
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  call _ent_en_chaine_ent_
  push rax
  xor rax, rax
  pop rax
  call _print_liste_chr__
  jmp LabelEnd107
Label106:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ent_en_chaine_ent_
  push rax
  xor rax, rax
  pop rax
  call _print_liste_chr__
  jmp LabelEnd107
LabelEnd107:
  xor r15, r15
  pop r15

  ret

_println_ent_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _print_ent_
  mov rax, 10
  push rax
  xor rax, rax
  pop rax
  call _print_chr_
  xor r15, r15
  pop r15

  ret

_print_ent32_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _print_ent_
  xor r15, r15
  pop r15

  ret

_print_ent16_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _print_ent_
  xor r15, r15
  pop r15

  ret

_print_ent8_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _print_ent_
  xor r15, r15
  pop r15

  ret

_println_ent32_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _println_ent_
  xor r15, r15
  pop r15

  ret

_println_ent16_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _println_ent_
  xor r15, r15
  pop r15

  ret

_println_ent8_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _println_ent_
  xor r15, r15
  pop r15

  ret

_erreur_liste_chr__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _println_liste_chr__
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  call _exit_ent_
  xor r15, r15
  pop r15

  ret

_bool._egal_bool_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq108
  mov rax, 0
  jmp LabelEq109
LabelEq108:
  mov rax, 1
LabelEq109:
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_println_bool_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  test rax, rax
  jz Label110
  mov rax, qword msg1
  push rax
  mov rdx, 4
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _println_liste_chr__
  jmp LabelEnd111
Label110:
  mov rax, qword msg2
  push rax
  mov rdx, 4
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _println_liste_chr__
  jmp LabelEnd111
LabelEnd111:
  xor r15, r15
  pop r15

  ret

_print_bool_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  test rax, rax
  jz Label112
  mov rax, qword msg1
  push rax
  mov rdx, 4
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  jmp LabelEnd113
Label112:
  mov rax, qword msg2
  push rax
  mov rdx, 4
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  jmp LabelEnd113
LabelEnd113:
  xor r15, r15
  pop r15

  ret

_bool._pas_bool_:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  test rax, rax
  jz Label114
  xor rax, rax
  mov al, 0
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  ret
  jmp LabelEnd115
Label114:
LabelEnd115:
  xor rax, rax
  mov al, 0
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15

  ret

_ptrmemoire_tuple.constructeur_ptrrien_ent_:
  push rax
  push rbx
  push rcx
  mov rax, 16
  push rax
  xor rdi, rdi
  pop rdi
  call _malloc
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  mov qword [rsp + 16], rax
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 0
  push rax
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_alloue_block_memoire__:
  mov rax, 10000
  push rax
  xor rdi, rdi
  pop rdi
  call _malloc
  push rax
  xor rax, rax
  pop rax
  ret

  ret

_pop_liste_ptrmemoire_tuple__ent_ent_:
  push rax
  push rbx
  push rcx
  push qword [rsp + 0]
LabelWhileBegin116:
  push qword [rsp + 0]
  push qword [rsp + 24]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt119
  mov rax, 1
  jmp LabelLt118
LabelLt119:
  mov rax, 0
LabelLt118:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd117
  push qword [rsp + 0]
  push qword [rsp + 32]
  push qword [rsp + 16]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*8]
  push qword [rax]
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 24]
  lea rax, [rax + rdx*8]
  mov qword [rax], rbx ;; test
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin116
LabelWhileEnd117:
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 24]
  lea rax, [rax + rdx*8]
  mov qword [rax], rbx ;; test
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_trouve_liste_ptrmemoire_tuple__ent_ptrrien_:
  push rax
  push rbx
  push rcx
  mov rax, 0
  push rax
LabelWhileBegin120:
  push qword [rsp + 0]
  push qword [rsp + 24]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt123
  mov rax, 1
  jmp LabelLt122
LabelLt123:
  mov rax, 0
LabelLt122:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd121
  push qword [rsp + 24]
  push qword [rsp + 8]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*8]
  push qword [rax]
  push qword [rsp + 16]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq124
  mov rax, 0
  jmp LabelEq125
LabelEq124:
  mov rax, 1
LabelEq125:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label126
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  jmp LabelEnd127
Label126:
LabelEnd127:
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  add rsp, 8
  jmp LabelWhileBegin120
LabelWhileEnd121:
  push qword [__nul]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_mallom_ent_:
  push rax
  mov rax, 0
  push rax
  mov rax, 0
  push rax
LabelWhileBegin128:
  push qword [rsp + 8]
  push qword [__blocs_libres]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt131
  mov rax, 1
  jmp LabelLt130
LabelLt131:
  mov rax, 0
LabelLt130:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd129
  push qword [__section_libre]
  push qword [rsp + 16]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*8]
  push qword [rax]
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 32]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq132
  mov rax, 0
  jmp LabelEq133
LabelEq132:
  mov rax, 1
LabelEq133:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label134
  push qword [__section_libre]
  push qword [__blocs_libres]
  push qword [rsp + 32]
  xor rcx, rcx
  pop rcx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _pop_liste_ptrmemoire_tuple__ent_ent_
  push qword [__blocs_libres]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [__blocs_libres], rax
  push qword [__blocs_alloue]
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [__section_mem]
  lea rax, [rax + rdx*8]
  mov qword [rax], rbx ;; test
  push qword [__blocs_alloue]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [__blocs_alloue], rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  jmp LabelEnd135
Label134:
LabelEnd135:
  push qword [rsp + 16]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 16], rax
  add rsp, 8
  jmp LabelWhileBegin128
LabelWhileEnd129:
  push qword [__blocs_alloue]
  push qword [__taille_mem]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt137
  mov rax, 1
  jmp LabelMt136
LabelMt137:
  mov rax, 0
LabelMt136:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label138
  mov rax, qword msg3
  push rax
  mov rdx, 82
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  jmp LabelEnd139
Label138:
LabelEnd139:
  push qword [__blocs_alloue]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jne LabelNeq140
  mov rax, 0
  jmp LabelNeq141
LabelNeq140:
  mov rax, 1
LabelNeq141:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label142
  push qword [__section_mem]
  push qword [__blocs_alloue]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*8]
  push qword [rax]
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 0]
  push qword [rsp + 56]
  xor rcx, rcx
  pop rcx
  xor rbx, rbx
  pop rbx
  mov rax, 0
  call _ptrmemoire_tuple.constructeur_ptrrien_ent_
  push rax
  push qword [__blocs_alloue]
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [__section_mem]
  lea rax, [rax + rdx*8]
  mov qword [rax], rbx ;; test
  push qword [__blocs_alloue]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [__blocs_alloue], rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  add rsp, 40
  jmp LabelEnd143
Label142:
  push qword [__memoire]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  push qword [rsp + 0]
  push qword [rsp + 32]
  xor rcx, rcx
  pop rcx
  xor rbx, rbx
  pop rbx
  mov rax, 0
  call _ptrmemoire_tuple.constructeur_ptrrien_ent_
  push rax
  push qword [__blocs_alloue]
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [__section_mem]
  lea rax, [rax + rdx*8]
  mov qword [rax], rbx ;; test
  push qword [__blocs_alloue]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [__blocs_alloue], rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  add rsp, 16
  jmp LabelEnd143
LabelEnd143:
  push qword [__nul]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_taille_ptrrien_:
  push rax
  mov rax, 0
  push rax
LabelWhileBegin144:
  push qword [rsp + 0]
  push qword [__blocs_alloue]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt147
  mov rax, 1
  jmp LabelLt146
LabelLt147:
  mov rax, 0
LabelLt146:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd145
  push qword [__section_mem]
  push qword [rsp + 8]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*8]
  push qword [rax]
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 24]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq148
  mov rax, 0
  jmp LabelEq149
LabelEq148:
  mov rax, 1
LabelEq149:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label150
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  jmp LabelEnd151
Label150:
LabelEnd151:
  add rsp, 8
  jmp LabelWhileBegin144
LabelWhileEnd145:
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_met_a_zero_ptrrien_ent_:
  push rax
  push rbx
  mov rax, 0
  push rax
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
LabelWhileBegin152:
  push qword [rsp + 8]
  push qword [rsp + 24]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt155
  mov rax, 1
  jmp LabelLt154
LabelLt155:
  mov rax, 0
LabelLt154:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd153
  push qword [rsp + 8]
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [rsp + 0]
  lea rax, [rax + rdx*1]
  mov byte [rax], bl ;; test
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  jmp LabelWhileBegin152
LabelWhileEnd153:
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_libere_ptrrien_:
  push rax
  mov rax, 0
  push rax
LabelWhileBegin156:
  push qword [rsp + 0]
  push qword [__blocs_alloue]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt159
  mov rax, 1
  jmp LabelLt158
LabelLt159:
  mov rax, 0
LabelLt158:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd157
  push qword [__section_mem]
  push qword [rsp + 8]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*8]
  push qword [rax]
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 24]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq160
  mov rax, 0
  jmp LabelEq161
LabelEq160:
  mov rax, 1
LabelEq161:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label162
  push qword [__section_mem]
  push qword [__blocs_alloue]
  push qword [rsp + 24]
  xor rcx, rcx
  pop rcx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _pop_liste_ptrmemoire_tuple__ent_ent_
  push qword [__blocs_libres]
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rdx, rdx
  pop rdx
  mov rax, qword [__section_libre]
  lea rax, [rax + rdx*8]
  mov qword [rax], rbx ;; test
  push qword [__blocs_libres]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [__blocs_libres], rax
  push qword [__blocs_alloue]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [__blocs_alloue], rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _met_a_zero_ptrrien_ent_
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  jmp LabelEnd163
Label162:
LabelEnd163:
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  add rsp, 8
  jmp LabelWhileBegin156
LabelWhileEnd157:
  mov rax, qword msg4
  push rax
  mov rdx, 34
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_copimem_ptrrien_ptrrien_:
  push rax
  push rbx
  push qword [__section_mem]
  push qword [__blocs_alloue]
  push qword [rsp + 24]
  xor rcx, rcx
  pop rcx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _trouve_liste_ptrmemoire_tuple__ent_ptrrien_
  push rax
  push qword [__section_mem]
  push qword [__blocs_alloue]
  push qword [rsp + 32]
  xor rcx, rcx
  pop rcx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  call _trouve_liste_ptrmemoire_tuple__ent_ptrrien_
  push rax
  push qword [rsp + 8]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq164
  mov rax, 0
  jmp LabelEq165
LabelEq164:
  mov rax, 1
LabelEq165:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label166
  mov rax, qword msg5
  push rax
  mov rdx, 69
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  jmp LabelEnd167
Label166:
LabelEnd167:
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq168
  mov rax, 0
  jmp LabelEq169
LabelEq168:
  mov rax, 1
LabelEq169:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label170
  mov rax, qword msg6
  push rax
  mov rdx, 74
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  jmp LabelEnd171
Label170:
LabelEnd171:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jne LabelNeq172
  mov rax, 0
  jmp LabelNeq173
LabelNeq172:
  mov rax, 1
LabelNeq173:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label174
  mov rax, qword msg7
  push rax
  mov rdx, 60
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  jmp LabelEnd175
Label174:
LabelEnd175:
  mov rax, 0
  push rax
LabelWhileBegin176:
  push qword [rsp + 0]
  push qword [rsp + 24]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt179
  mov rax, 1
  jmp LabelLt178
LabelLt179:
  mov rax, 0
LabelLt178:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd177
  push qword [rsp + 48]
  xor al, al
  pop rax
  xor rbx, rbx
  mov bl, byte [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  mov rax, qword [rsp + 40]
  mov byte [rax], bl
  push qword [rsp + 40]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  mov qword [rsp + 40], rax
  push qword [rsp + 48]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  mov qword [rsp + 48], rax
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  jmp LabelWhileBegin176
LabelWhileEnd177:
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_affiche_memoire__:
  mov rax, 0
  push rax
  mov rax, qword msg8
  push rax
  mov rdx, 46
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _println_liste_chr__
LabelWhileBegin180:
  push qword [rsp + 0]
  push qword [__blocs_alloue]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt183
  mov rax, 1
  jmp LabelLt182
LabelLt183:
  mov rax, 0
LabelLt182:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd181
  push qword [__section_mem]
  push qword [rsp + 8]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*8]
  push qword [rax]
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _hex_ent_
  push rax
  xor rax, rax
  pop rax
  call _print_liste_chr__
  mov rax, qword msg9
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  call _print_ent_
  mov rax, qword msg9
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  call _println_ent_
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  add rsp, 8
  jmp LabelWhileBegin180
LabelWhileEnd181:
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  mov rax, qword msg10
  push rax
  mov rdx, 44
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _println_liste_chr__
LabelWhileBegin184:
  push qword [rsp + 0]
  push qword [__blocs_libres]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  jns LabelLt187
  mov rax, 1
  jmp LabelLt186
LabelLt187:
  mov rax, 0
LabelLt186:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz LabelWhileEnd185
  push qword [__section_libre]
  push qword [rsp + 8]
  xor rdx, rdx
  pop rdx
  xor rax, rax
  pop rax
  lea rax, [rax + rdx*8]
  push qword [rax]
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _hex_ent_
  push rax
  xor rax, rax
  pop rax
  call _print_liste_chr__
  mov rax, qword msg9
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  call _print_ent_
  mov rax, qword msg9
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  call _println_ent_
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  add rsp, 8
  jmp LabelWhileBegin184
LabelWhileEnd185:
  xor r15, r15
  pop r15

  ret

_ptrliste..ent...constructeur__:
  push rax
  mov rax, 16
  push rax
  xor rax, rax
  pop rax
  call _mallom_ent_
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 0
  push rax
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rbx, rbx
  pop rbx
  mov rax, 0
  call _ptrnoeud..ent...constructeur_ent_
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15

  ret

_ptrnoeud..ent...constructeur_ent_:
  push rax
  push rbx
  mov rax, 16
  push rax
  xor rax, rax
  pop rax
  call _mallom_ent_
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 0
  push rax
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrnoeud..ent...pousse_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq188
  mov rax, 0
  jmp LabelEq189
LabelEq188:
  mov rax, 1
LabelEq189:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label190
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  mov rax, 0
  call _ptrnoeud..ent...constructeur_ent_
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  jmp LabelEnd191
Label190:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..ent...pousse_ent_
  add rsp, 8
  jmp LabelEnd191
LabelEnd191:
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrnoeud..ent...print__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq192
  mov rax, 0
  jmp LabelEq193
LabelEq192:
  mov rax, 1
LabelEq193:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label194
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  call _print_ent_
  jmp LabelEnd195
Label194:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  call _print_ent_
  mov rax, qword msg9
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrnoeud..ent...print__
  add rsp, 8
  jmp LabelEnd195
LabelEnd195:
  xor r15, r15
  pop r15

  ret

_ptrnoeud..ent...index_ent_:
  push rax
  push rbx
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq196
  mov rax, 0
  jmp LabelEq197
LabelEq196:
  mov rax, 1
LabelEq197:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label198
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  jmp LabelEnd199
Label198:
LabelEnd199:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..ent...index_ent_
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrnoeud..ent...pop_ent_:
  push rax
  push rbx
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq200
  mov rax, 0
  jmp LabelEq201
LabelEq200:
  mov rax, 1
LabelEq201:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label202
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq204
  mov rax, 0
  jmp LabelEq205
LabelEq204:
  mov rax, 1
LabelEq205:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label206
  push qword [rsp + 24]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  push qword [rsp + 32]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  jmp LabelEnd207
Label206:
  push qword [rsp + 24]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  jmp LabelEnd207
LabelEnd207:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _libere_ptrrien_
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  add rsp, 16
  jmp LabelEnd203
Label202:
LabelEnd203:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..ent...pop_ent_
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..ent...ajoute_ent_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..ent...pousse_ent_
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..ent...print__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq208
  mov rax, 0
  jmp LabelEq209
LabelEq208:
  mov rax, 1
LabelEq209:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label210
  mov rax, qword msg11
  push rax
  mov rdx, 3
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  pop r15 ;; unused values
  ret
  jmp LabelEnd211
Label210:
LabelEnd211:
  mov rax, qword msg12
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  push qword [rax]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrnoeud..ent...print__
  mov rax, qword msg13
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..ent...println__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrliste..ent...print__
  mov rax, 10
  push rax
  xor rax, rax
  pop rax
  call _print_chr_
  xor r15, r15
  pop r15

  ret

_ptrliste..ent...index_ent_:
  push rax
  push rbx
  push qword [rsp + 0]
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt213
  mov rax, 1
  jmp LabelMt212
LabelMt213:
  mov rax, 0
LabelMt212:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label214
  mov rax, qword msg14
  push rax
  mov rdx, 63
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  jmp LabelEnd215
Label214:
LabelEnd215:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..ent...index_ent_
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..ent...pop_ent_:
  push rax
  push rbx
  push qword [rsp + 0]
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt217
  mov rax, 1
  jmp LabelMt216
LabelMt217:
  mov rax, 0
LabelMt216:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label218
  mov rax, qword msg15
  push rax
  mov rdx, 61
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  jmp LabelEnd219
Label218:
LabelEnd219:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..ent...pop_ent_
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..chr...constructeur__:
  push rax
  mov rax, 16
  push rax
  xor rax, rax
  pop rax
  call _mallom_ent_
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  mov qword [rsp + 0], rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 0
  push rax
  mov rax, 0
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, al
  push rbx
  xor rbx, rbx
  pop rbx
  mov rax, 0
  call _ptrnoeud..chr...constructeur_chr_
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15

  ret

_ptrnoeud..chr...constructeur_chr_:
  push rax
  push rbx
  mov rax, 16
  push rax
  xor rax, rax
  pop rax
  call _mallom_ent_
  push rax
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  mov qword [rsp + 8], rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 0
  push rax
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov byte [rax], bl
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 1
  push rax
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrnoeud..chr...pousse_chr_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq220
  mov rax, 0
  jmp LabelEq221
LabelEq220:
  mov rax, 1
LabelEq221:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label222
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 1
  push rax
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  mov rax, 0
  call _ptrnoeud..chr...constructeur_chr_
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  jmp LabelEnd223
Label222:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..chr...pousse_chr_
  add rsp, 8
  jmp LabelEnd223
LabelEnd223:
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrnoeud..chr...print__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq224
  mov rax, 0
  jmp LabelEq225
LabelEq224:
  mov rax, 1
LabelEq225:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label226
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, byte [rax]
  push rbx
  xor rax, rax
  pop rax
  call _print_chr_
  jmp LabelEnd227
Label226:
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, byte [rax]
  push rbx
  xor rax, rax
  pop rax
  call _print_chr_
  mov rax, qword msg9
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrnoeud..chr...print__
  add rsp, 8
  jmp LabelEnd227
LabelEnd227:
  xor r15, r15
  pop r15

  ret

_ptrnoeud..chr...index_ent_:
  push rax
  push rbx
  push qword [rsp + 0]
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq228
  mov rax, 0
  jmp LabelEq229
LabelEq228:
  mov rax, 1
LabelEq229:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label230
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, byte [rax]
  push rbx
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  jmp LabelEnd231
Label230:
LabelEnd231:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..chr...index_ent_
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrnoeud..chr...pop_ent_:
  push rax
  push rbx
  push qword [rsp + 0]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq232
  mov rax, 0
  jmp LabelEq233
LabelEq232:
  mov rax, 1
LabelEq233:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label234
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov bl, byte [rax]
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 16]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq236
  mov rax, 0
  jmp LabelEq237
LabelEq236:
  mov rax, 1
LabelEq237:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label238
  push qword [rsp + 24]
  xor rax, rax
  pop rax
  add rax, 1
  push rax
  push qword [rsp + 32]
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  jmp LabelEnd239
Label238:
  push qword [rsp + 24]
  xor rax, rax
  pop rax
  add rax, 1
  push rax
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  jmp LabelEnd239
LabelEnd239:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, rax
  push rbx
  xor rax, rax
  pop rax
  call _libere_ptrrien_
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  add rsp, 16
  jmp LabelEnd235
Label234:
LabelEnd235:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  sub rax, rbx
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..chr...pop_ent_
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..chr...ajoute_chr_:
  push rax
  push rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  add rax, 8
  push rax
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  mov qword [rax], rbx
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..chr...pousse_chr_
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..chr...print__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  mov rax, 0
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  je LabelEq240
  mov rax, 0
  jmp LabelEq241
LabelEq240:
  mov rax, 1
LabelEq241:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label242
  mov rax, qword msg11
  push rax
  mov rdx, 3
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  pop r15 ;; unused values
  ret
  jmp LabelEnd243
Label242:
LabelEnd243:
  mov rax, qword msg12
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  push qword [rax]
  xor rax, rax
  pop rax
  add rax, 1
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrnoeud..chr...print__
  mov rax, qword msg13
  push rax
  mov rdx, 2
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _print_liste_chr__
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..chr...println__:
  push rax
  push qword [rsp + 0]
  xor rax, rax
  pop rax
  call _ptrliste..chr...print__
  mov rax, 10
  push rax
  xor rax, rax
  pop rax
  call _print_chr_
  xor r15, r15
  pop r15

  ret

_ptrliste..chr...index_ent_:
  push rax
  push rbx
  push qword [rsp + 0]
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt245
  mov rax, 1
  jmp LabelMt244
LabelMt245:
  mov rax, 0
LabelMt244:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label246
  mov rax, qword msg14
  push rax
  mov rdx, 63
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  jmp LabelEnd247
Label246:
LabelEnd247:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..chr...index_ent_
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret

_ptrliste..chr...pop_ent_:
  push rax
  push rbx
  push qword [rsp + 0]
  push qword [rsp + 16]
  xor rax, rax
  pop rax
  add rax, 8
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  cmp rax, rbx
  js LabelMt249
  mov rax, 1
  jmp LabelMt248
LabelMt249:
  mov rax, 0
LabelMt248:
  push rax
  xor rax, rax
  pop rax
  test rax, rax
  jz Label250
  mov rax, qword msg15
  push rax
  mov rdx, 61
  call malloc
  push rax
  mov rdi, rax
  xor rsi, rsi
  pop rsi
  call chr_copy
  xor rax, rax
  pop rax
  call _erreur_liste_chr__
  jmp LabelEnd251
Label250:
LabelEnd251:
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  mov rbx, qword [rax]
  push rbx
  push qword [rsp + 8]
  mov rax, 1
  push rax
  xor rbx, rbx
  pop rbx
  xor rax, rax
  pop rax
  add rax, rbx
  push rax
  push qword [rsp + 8]
  xor rax, rax
  pop rax
  xor rbx, rbx
  pop rbx
  call _ptrnoeud..chr...pop_ent_
  push rax
  xor rax, rax
  pop rax
  pop r15 ;; unused values
  pop r15 ;; unused values
  pop r15 ;; unused values
  ret
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15
  xor r15, r15
  pop r15

  ret



div_rcx_10:
    push rax
    mov rax, rcx
    mov rbx, 10
    xor rdx, rdx
    div rbx
    mov rcx, rax
    pop rax
    ret

biggest_ten:
    push rax
    mov rax, rcx
    mov rbx, 10
    mul rbx
    mov rcx, rax
    pop rax
    push rax    
    xor rdx, rdx
    div rcx
    mov rbx, rax
    pop rax
    cmp rbx, 0
    jnz biggest_ten
    call div_rcx_10
    ret

;;print_integer:
;;    push rdx
;;    push rax
;;    call biggest_ten
;;    xor rdx, rdx
;;    div rcx 
;;    mov rbx, rax
;;    mul rcx
;;    mov rdx, rax
;;    pop rax    
;;    sub rax, rdx
;;    push rax
;;    or rbx, 0x30
;;    push rcx
;;    push rbx
;;    mov rax, 0x2000004
;;    mov rsi, rsp
;;    mov rdx, 1
;;    mov rdi, 1
;;    syscall
;;    pop rbx
;;    pop rcx
;;    call div_rcx_10
;;    pop rax
;;    pop rdx
;;    cmp rcx, 0 ;; compare the divider to know if the printing process is complete
;;    jnz print_integer
;;    ret
    
;;print_integer_ln:
;;    call print_integer
;;    mov rax, 0x2000004
;;    mov rsi, qword newline
;;    mov rdx, 1
;;    mov rdi, 1
;;    syscall
;;    ret
    
malloc:
  ;; in rdx, there is the size needed to the alloc
  mov rdi, rdx
  push rdx
  call _malloc
  pop rdx
  ret

malloc_bis:
  ;; in rdx, there is the size needed to the alloc
  mov rsi, rdx                    ;; length
  mov rax, 0x20000c5              ;; mmap
  xor rdi, rdi                    ;; start address
  mov rdx, (0x01|0x02)            ;; rwx
  mov r10, (0x1000|0x0001)        ;; flags
  xor r8, r8                      ;; file descriptor
  xor r9, r9                      ;; offset
  syscall ;; the adress should be in rax
  ret

free: ;; Arguments: rdi = pointer to allocated memory, rsi = size
  mov rax, 0x200000b     ; syscall number for munmap
  syscall                ; invoke the munmap system call
  ret

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
  
_big_or_little_endian:
  mov  ax, 0x1234        ; load up 1234 hex
  mov  [endianness], ax      ; store that word to memory
  mov  al, [endianness]      ; get first byte of that
  cmp  al, 0x12          ; 12 means big endian
  je   big
little:
  mov rax, 1
  xor rbx, rbx
  mov  qword [__little_endian], rax
  mov  qword [__big_endian], rbx
  ret
big:
  mov rax, 1
  xor rbx, rbx
  mov  qword [__little_endian], rbx
  mov  qword [__big_endian], rax
  ret


section .data
  chr_buffer:  times 10 db 0
  endianness: dw 0
  msg0: db "0x0", 0, 0, 0, 0, 0, 0, 0, 0
  msg1: db "Vrai", 0, 0, 0, 0, 0, 0, 0, 0
  msg2: db "Faux", 0, 0, 0, 0, 0, 0, 0, 0
  msg3: db "Le nombre de blocs mémoire alloués est supérieur à la taille de mémoire disponible", 0, 0, 0, 0, 0, 0, 0, 0
  msg4: db "N'a pas trouvé le pointeur demandé", 0, 0, 0, 0, 0, 0, 0, 0
  msg5: db "!copimem : L'argument 1 (la source) n'a pas été allouée avec 'mallom'", 0, 0, 0, 0, 0, 0, 0, 0
  msg6: db "!copimem : L'argument 2 (la destination) n'a pas été allouée avec 'mallom'", 0, 0, 0, 0, 0, 0, 0, 0
  msg7: db "!copimem : Les tailles des deux pointeurs ne sont pas égales", 0, 0, 0, 0, 0, 0, 0, 0
  msg8: db "Blocs alloués : addresse, taille, <ent>contenu", 0, 0, 0, 0, 0, 0, 0, 0
  msg9: db ", ", 0, 0, 0, 0, 0, 0, 0, 0
  msg10: db "Blocs vides : addresse, taille, <ent>contenu", 0, 0, 0, 0, 0, 0, 0, 0
  msg11: db "[ ]", 0, 0, 0, 0, 0, 0, 0, 0
  msg12: db "[ ", 0, 0, 0, 0, 0, 0, 0, 0
  msg13: db " ]", 0, 0, 0, 0, 0, 0, 0, 0
  msg14: db "!liste.index : L'index est plus grand que la taille de la liste", 0, 0, 0, 0, 0, 0, 0, 0
  msg15: db "!liste.pop : L'index est plus grand que la taille de la liste", 0, 0, 0, 0, 0, 0, 0, 0
  __big_endian: dq 0
  __little_endian: dq 0
  __stdin: dq 0
  __stdout: dq 0
  __icanon: dq 0
  __echo: dq 0
  __chr_tampon: dq 0
  __taille_mem: dq 0
  __memoire: dq 0
  __nul: dq 0
  __blocs_alloue: dq 0
  __blocs_libres: dq 0
  __section_mem: dq 0
  __section_libre: dq 0
  newline: db 10
extern _malloc
