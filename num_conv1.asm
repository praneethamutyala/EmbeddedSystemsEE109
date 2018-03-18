###############################################################################
## Author:  _____Sai Praneetha Mutyala_____________
## Date:    _______11/4/2016___________
## Purpose: Convert a number in some base to its decimal equivalent
##
###############################################################################
	.data
imsg1:	.asciiz	"Enter a number in another base using at most 10 digits: "
imsg2:	.asciiz	"Enter the base number: "
omsg1:	.asciiz	"Decimal equivalent: "
newln:	.asciiz "\n"
num:	.space	12   
base:	.word	0
ans:	.word	0
len:	.word 	0

	.text
START:	
				
	li	$v0, 4		# Print the 1st prompt
	la	$a0, imsg1
	syscall

	li	$v0, 8		# Get the digits from the user
	la	$a0, num	# Setup a pointer to the destination array
	li	$a1, 12		# there will be a newline
	syscall			# and a null char at the end

	li	$v0, 4		# Print the 2nd prompt
	la	$a0, imsg2
	syscall
	
	li	$v0, 5		# read integer
	syscall	
	la	$a1, base
	sw	$v0, 0($a1)	# store base to memory variable
	

	la 	$a0, num  	# Load the address of num
	la	$a1, base	# Load the address of the base
	lw	$t0, 0($a1)	# load the base number

## Add your code here	
## To convert to decimal we start a sum with the most-significant digit.
##  Then for each subsequent digit we multiplying the sum by the base 
##  and add the next digit
 gets:
 	la 	$s1, num 	#sets base address of the array to s1
 loop:
 	jal 	getc		#jump to getc subroutine
 	lb 	$t0, base	#loads the char from char buffer into $t0 stripping null
 	sb	$t0, 0($s1)	#stores the char in the nth element of array
 	lb	$t1, newln	#load newline char into t1
 	beq 	$t0, $t1, done  #if end of string, then jump to done
 	addi	$s1, $s1, 1   	#incremets the base address of the array
 	j 	loop		#jump to start of the loop to read
 	
 getc:
 	li 	$v0, 8		#call the code for reading the string	
 	la 	$a0, base	#loads the address of char for reading in
 	li 	$a1, 2		#length of the string is 1 byte char and 1 byte for null
 	syscall			# stores the char byte from input buffer into char
 	jr	$ra		#jump-register to calling function
 	
 readInt:
 	j	gets		#letting $s1 to be the top address of the array and $s0 be the counter digits
 done:				#lettig s2 to be the sum total
 	addi 	$s1, $s1, -1	#repositioning the array pointer to last character before the newline
 	la	$s0, num 	#set the base address of the array to s0 for use as counter
 	addi	$s0, $s0, -1	#repositioning the base array to read the leftmost character in the string 
 	add	$s2, $0, $0	#initialising sum to be zero
 	li 	$t0, 10		#sets $t0 to be 10 used for decimal conversion
 	li	$t3, 1
 	lb	$t1, 0($s1)	#loads char from array into $t1
 	blt	$t1, 48, error	#checking error for if char is not a digit (ascii <0)
 	bgt	$t1, 57, error	#checking error for if char is not a digit (ascii > 9)	
 	addi	$t1, $t1, -48	#converts $t1 ascii value to the decimal value
 	add	$s2, $s2, $t1	#add decimal value to t1 to give the total sum
 	addi	$s1, $s1, -1	#decrements the array address
 	j	print 
 	
 lp: #loops for all the digist preceeding the LSB
 	andi	$t0, $t0, 0x0F 	#where $t0 contains the ascii digit now and hence, it will have the int of it
 	lbu	$t1, ($s1)
 	beq 	$t1, $s0, FIN	#exit if the beginning of the string is attached
 	blt	$t1, 48, error	#hecking error for if char is not a digit (ascii<0)
 	bgt	$t1, 57, error	#checking error for if char is not a digit (ascii>9)	
 	addi	$t1, $t1, -48	#converts $t1 ascii value to the decimal value
 	mul	$s2, $s2, $t0	#sum *= 10
 	add	$s2, $s2, $t1	#sum += array[s1]-'0'
 	addi	$s1, $s1, 1	#increment array address
 	j	 lp		#jump to the start of the loop
 	
 
 error: #if non digit chars are entered, readInt returns 0
 	add 	$s2, $0, $0
 	j	FIN
 	
 FIN:
 	li 	$v0, 1
 	add	$a0, $s2, $0
 	syscall 
 	
 	
# When done, print the converted number using syscall service 1
print: #for printing the answer 
	li 	$v0, 4
	la	$a0, omsg1
	li	$v0, 8
	la	$a0, omsg1
	li 	$v0, 1
	la	$a0, ans
	jal 	done
	syscall


# Do not alter code below this point
	li	$v0, 4		# print newline
	la	$a0, newln
	jal 	done 
	syscall
# Quit
	li	$v0, 10
	syscall


