	lw 0 1 21	Load 10 to reg 1 (numeric)
	lw 0 2 neg1	load -2 to reg 2 (symbolic)
	lw 0 3 zero	load 0 to reg 3 (symbolic)
	lw 0 4 two	load 2 to reg 4 (symbolic)
	lw 0 5 one	load 1 to reg 5 (symbolic)
loop	add 5 3 3	reg3 = reg5 + reg3
	beq 3 1 1	break
	beq 0 0 loop goto loop
	lw 0 3 zero
loop2	add 1 2 1	reg1 = reg1 + reg2
	beq 3 1 1	break
	beq 0 0 loop2 goto loop2
	add 4 1 4	reg4 = reg4 + reg1
	lw 0 3 offset
	jalr 3 4	jump to halt
	add 3 5 3 reg3 = reg3 + reg5 (this code will be dismissed)
done halt 
zero .fill 0 
one .fill 1  
two .fill 2
neg1 .fill -2
ten .fill 10
offset .fill 16
