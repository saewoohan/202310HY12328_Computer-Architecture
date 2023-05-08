	lw 0 1 10	laod 5 to reg1 (numeric)
	lw 0 2 ten	load 10 to reg3 (symbolic)
	nor 1 2 3	reg3 = reg1 nor reg2 ( ~(2 | 10) ) 
	sw 0 3 ten 	store reg3 to mem9 (symbolic)
	sw 0 3 7	store reg3 to mem7 (numeric)
    lw 0 4 offset    load 8 to reg4 (symbolic)
	jalr 4 3    jump to halt
    add 1 2 3   reg3 = reg1 + reg2 (this code will be dismissed)
done halt 
ten .fill 10 
five .fill 5
offset .fill 8
