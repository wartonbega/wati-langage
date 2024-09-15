

print_func = """
memcopy_stack:
  ;; the source is in rsi, destination in rdi, size in rdx (modifies rsi, rdi)
  test rdx, rdx
  jz memcopy_stack_end
  xor rax, rax
  mov al, byte [rsi]
  mov byte [rdi], al
  dec rdx
  dec rsi
  dec rdi
  jmp memcopy_stack
memcopy_stack_end:
  ret

memcopy:
  ;; the source is in rsi, destination in rdi, size in rdx
  push rsi
  push rdi
  push rax
  push rdx
memcopy_begin:
  test rdx, rdx
  jz memcopy_end
  xor rax, rax
  mov al, byte [rsi]
  mov byte [rdi], al
  dec rdx
  inc rsi
  inc rdi
  jmp memcopy_begin
memcopy_end:
  pop rdx
  pop rax
  pop rdi
  pop rsi
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
  
multisize_reg_check: ;; L'adresse est dans rax
  mov qword [MRC_save_r9 ], r9
  mov qword [MRC_save_r10], r10
  mov qword [MRC_save_r11], r11
  mov qword [MRC_save_r12], r12
  mov qword [MRC_save_rdx], rdx
  mov qword [MRC_save_rsi], rsi
  mov qword [MRC_save_rdi], rdi
  
  mov r11, qword [multisize_rax_size]
  mov r12, multisize_rax
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_rbx_size]
  mov r12, multisize_rbx
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_rcx_size]
  mov r12, multisize_rcx
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_rdx_size]
  mov r12, multisize_rdx
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_rsi_size]
  mov r12, multisize_rsi
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_rdi_size]
  mov r12, multisize_rdi
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_r8_size]
  mov r12, multisize_r8
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_r9_size]
  mov r12, multisize_r9
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_r10_size]
  mov r12, multisize_r10
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_r11_size]
  mov r12, multisize_r11
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_r12_size]
  mov r12, multisize_r12
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_r13_size]
  mov r12, multisize_r13
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_r14_size]
  mov r12, multisize_r14
  cmp rax, r12
  je multisize_reg_check_true
  
  mov r11, qword [multisize_r15_size]
  mov r12, multisize_r15
  cmp rax, r12
  je multisize_reg_check_true


multisize_reg_check_true:
  pop r10 ;; l'adresse de départ
  
  ;; dans rax la source
  mov rdx, r11  ;; la taille
  
  mov rsi, rax  ;; la source
  sub rsp, r11  ;; on augmente la taille du stack
  mov rdi, rsp  ;; la destination

  mov rax, rsp
  call initiate_data_zero
  call memcopy

  push r10 ;; adrresse de départ
  
  mov r9 , qword [MRC_save_r9 ]
  mov r10, qword [MRC_save_r10]
  mov r11, qword [MRC_save_r11]
  mov r12, qword [MRC_save_r12]
  mov rdx, qword [MRC_save_rdx]
  mov rsi, qword [MRC_save_rsi]
  mov rdi, qword [MRC_save_rdi]
  
  ret

initiate_data_zero: ;; rax : pointer, rdx : size
  push rdx
  push rax
initiate_data_zero_begin:
  mov byte [rax], 0
  dec rdx 
  inc rax
  cmp rdx, 0
  je initiate_data_zero_end
  jmp initiate_data_zero_begin
initiate_data_zero_end:
  pop rax
  pop rdx
  ret
"""