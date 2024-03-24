	# This is an assembly hello world program

	# The data section holding the program's constants
data:
	ascii @msg "Hello, World!\n"

	# The code section 
code:
	movui u0, @msg 
	outb u0
	hlt
