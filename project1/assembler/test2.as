	lw	0	1	1000000		offsetfileds don't fit in 16 bits
	lw	0	2	four	load reg2 with 4 (symbolic address)
	lw	0	3	ten		load reg3 with ten (symbolic address)
	add	1	2	3		reg3 = reg1 + reg2 (4+8)
    sw	0	3	eight	store mem8 with reg3 (symbolic address)
    sw	0	3	1		store mem1 with reg3 (numeric address)
done	halt
eight .fill 8 
four .fill 4
ten .fill 10
