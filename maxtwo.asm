
###############################################################################
## Author:  Mark Redekopp
## Date:    2014-Nov-11
## Purpose: Finds the max of 2 input numbers
##
###############################################################################
	.data
NUMS:	.word   0,0
MAX:	.word   0   # memory location to place the result into
IMSG:	.asciiz "Enter 2 integers hitting 'Enter' after each one.\n"
OMSG:	.asciiz "The maximum number is:\n"
	.text
START:	la 	$a2, NUMS  	# Load the address of NUMS array
	la	$a3, MAX	# Load the address of MAX for later use
	
	li	$v0, 4		# Print the prompt
	la	$a0, IMSG
	syscall
	
	li	$v0, 5		# Get the first number
	syscall
	sw	$v0, 0($a2)	# store the first number
	
	li	$v0, 5		# Get the second nuber
	syscall
	sw	$v0, 4($a2)	# store the second number

	lw	$t0, 0($a2)	# load the numbers into registers
	lw	$t1, 4($a2)

## Now implement:
# if (A >= B)
#   max = A
# else 
#   max = B

	# C1 is IF
C1:	blt	$t0,$t1,C2	# Skip to ELSE if A < B
	sw	$t0,0($a3)	# A is MAX				
	b 	CDONE		
	# C2 is ELSE
C2:	sw	$t1,0($a3)	# ELSE B is MAX

# Now print out the max
CDONE:	li	$v0, 4		# print output prompt
	la	$a0, OMSG
	syscall
	li	$v0, 1		# load MAX and print it
	lw	$a0, 0($a3)
	syscall

# Quit
	li	$v0, 10
	syscall
