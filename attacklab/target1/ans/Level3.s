movq    $0x6044d2, %rax
movq    $0x6044e8, %rdx
movq    $1, (%rdx)
movq    $0x3, (%rax)
movl    $1, %eax
pushq   $0x401916
retq
