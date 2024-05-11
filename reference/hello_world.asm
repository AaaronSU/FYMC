	# This is an assembly hello world program

	# The data section holding the program's constants
data:
	ascii msg "Hello, World!\n"

	# The code section
code:
<<<<<<< HEAD
	movui u0, @msg 
=======
	movui u0, @msg
>>>>>>> a32ac885c0d21877b8f50c17b48c44458a59f483
	outb u0
	hlt
