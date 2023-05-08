	lw	0	1	eight	load reg1 with 8 (symbolic address)
	lw	0	2	four	load reg2 with 4 (symbolic address)
	lw	0	3	ten		load reg3 with 10 (symbolic address)
	add	1	2	3		reg3 = reg1 + reg2 (4+8)
    sw	0	3	eight	store mem8 with reg3 (symbolic address)
	nor	1	2	3		reg3 = ~(reg1 | reg2)
    sw	0	3	ten		store mem10 with reg3 (symbolic address)
	lw	0	4	16		load reg4 with mem3 (numeric address)
	lw 	0	1	zero 	load reg1 with 0 (symbolic address)
	lw	0	5	sub1	load reg5 with -1 (symbolic address)
loop add 1	5	1		reg1 = reg1 - reg5
	beq 4	1	1		break
	beq 0 	0 	loop 	goto loop
done	halt
eight .fill 8
four .fill 4
ten .fill 10
zero .fill 0
sub1 .fill -1
