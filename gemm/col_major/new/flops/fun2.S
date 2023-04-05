// func2.S 每次循环做 160 次乘法，每条指令处理 16 个 float，共 10 条。
.text
.align 5
.global func2

func2:
    ptrue p0.s
.loop2:
    fmla z0.s, p0/m, z0.s, z0.s
    fmla z1.s, p0/m, z1.s, z1.s
    fmla z2.s, p0/m, z2.s, z2.s
    fmla z3.s, p0/m, z3.s, z3.s
              
    fmla z4.s, p0/m, z4.s, z4.s
    fmla z5.s, p0/m, z5.s, z5.s
    fmla z6.s, p0/m, z6.s, z6.s
    fmla z7.s, p0/m, z7.s, z7.s
             
    fmla z8.s, p0/m, z8.s, z8.s
    fmla z9.s, p0/m, z9.s, z9.s

    subs x0, x0, #1
    bne .loop2
    ret
