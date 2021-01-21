.section .text
.global start
start:

call multiply

ebreak

multiply:

li x1, 0x80000000
li x2, 0x81
sb x2, 0(x1)

ret
