.data

.text
        j main
main:
        addi $sp $sp -24
        sw $fp 16($sp)
        sw $ra 20($sp)
        move $fp $sp


        li $t1 3
        sw $t1 0($fp)


        li $t1 1
        sw $t1 4($fp)

        j label_1

label_3:

        lw $t1 4($fp)
        li $t2 2
        mult $t1 $t2
        mflo $t3
        sw $t3 8($fp)

        lw $t1 8($fp)
        sw $t1 4($fp)

        lw $t1 0($fp)
        li $t2 1
        sub $t3 $t1 $t2
        sw $t3 12($fp)

        lw $t1 12($fp)
        sw $t1 0($fp)

label_1:

        lw $t1 0($fp)
        li $t2 0
        sub $t3 $t1 $t2
        bgez $t3 label_3

        lw $a0 4($fp)
        li $v0 1
        syscall

        addi $a0 $0 '\n'
        li $v0 11
        syscall

        move $sp $fp
        lw $fp 16($sp)
        lw $ra 20($sp)
        addi $sp $sp 24

        li $v0 10
        syscall