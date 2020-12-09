

.data
	zwh_str0: .ascii "a_A\0"

.text
	j main
main:
	addi $sp $sp -32
	sw $fp 24($sp)
	sw $ra 28($sp)
	move $fp $sp

	li $a0 18373610
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	li $t1 15
	li $t2 3
	add $t3 $t1 $t2
	sw $t3 0($fp)

	lw $a0 0($fp)
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	li $t1 15
	li $t2 3
	sub $t3 $t1 $t2
	sw $t3 4($fp)

	lw $a0 4($fp)
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	li $t1 15
	li $t2 3
	mult $t1 $t2
	mflo $t3
	sw $t3 8($fp)

	lw $a0 8($fp)
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	li $t1 15
	li $t2 3
	div $t1 $t2
	mflo $t3
	sw $t3 12($fp)

	lw $a0 12($fp)
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	li $t1 15
	li $t2 3
	add $t3 $t1 $t2
	sw $t3 16($fp)

	lw $a0 16($fp)
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	la $a0 zwh_str0
	li $v0 4
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	li $a0 3
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	li $a0 15
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	li $t1 15
	sub $t3 $0 $t1
	sw $t3 20($fp)

	lw $a0 20($fp)
	li $v0 1
	syscall

	addi $a0 $0 '\n'
	li $v0 11
	syscall

	move $sp $fp
	lw $fp 24($sp)
	lw $ra 28($sp)
	addi $sp $sp 32

	li $v0 10
	syscall
