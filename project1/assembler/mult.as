        lw 0 2 mplier   load reg2 with 12328
        lw 0 3 mcand    load reg3 with 32766
        lw 0 4 maskb    load reg4 with -2
        lw 0 5 m_bit    load reg5 with maxbit
loop    nor 2 4 7       reg7 = ~(reg2 | reg4)
        beq 0 7 L1      branch L1 if reg0 == reg7
        beq 0 0 L2      branch L2 if reg0 == reg0
L1      add 1 3 1       reg1 = reg1 = reg34
L2      lw 0 7 sub1     load reg7 with -1
        add 5 7 5       reg5 = reg5 + reg7
        beq 0 5 done    branch done if reg0 == reg5
        add 3 3 3       left shift bit
        add 4 4 4       left shift bit
        lw 0 7 add1     load reg7 with 1
        add 4 7 4       reg4 = reg4 + reg7
        beq 0 0 loop    branch loop if reg0 == reg0
done    halt
mcand   .fill   32766
mplier  .fill   12328
maskb   .fill   -2      
add1    .fill   1
sub1    .fill   -1
m_bit  .fill   15      
