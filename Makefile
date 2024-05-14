all:run_program
config_file := config.txt

.PHONY: run_program
run_program:
	script/run_program.sh $(config_file)

.PHONY:verify_as
verify_as:
	script/verify_as.sh

.PHONY:test_function
test_function:
	script/test_function.sh

.PHONY:test_program
test_program:
	python3 script/test_program.py

.PHONY:bench_function
bench_function:
	script/run_bench.sh
