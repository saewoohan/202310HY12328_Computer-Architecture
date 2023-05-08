	lw 0 1 8	load 8 to reg1 (numeric)
	lw 0 2 five	load five to reg2 (symbolic)
	lw 0 3 ten	load 10 to reg3 (symbolic)
	add 1 3 3	reg3 = reg1 + reg3
    sw	0	3	neg0	error use of undefined labels
    sw	0	3	1		store mem1 with reg3 (symbolic address)
	noop
done halt
ten .fill 10
five .fill 5
eight .fill 8
