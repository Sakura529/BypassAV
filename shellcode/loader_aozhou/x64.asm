.code

GetPEB64 proc
    xor rax, rax
    mov rax, gs:[60h]
    ret
GetPEB64 endp

GetNtdllAddr64 proc
    call GetPEB64
    mov rax, [rax+18h]
    mov rax, [rax+30h]
	mov rax, [rax+10h]
    ret
GetNtdllAddr64 endp


GetKernel32Addr64 proc
    call GetPEB64
    mov rax, [rax+18h]
    mov rax, [rax+30h]
	mov rax, [rax]
	mov rax, [rax]
	mov rax, [rax+10h]
	ret
GetKernel32Addr64 endp
end