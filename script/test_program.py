import subprocess

def verify_output(command, expected_output):
    try:
        output = subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT).decode()
        data_is_valid = output == expected_output
        return data_is_valid, output
    except subprocess.CalledProcessError as e:
        return False, str(e)

def verify_program_output(tests):
    passed = 0
    failed_command = []
    for test_command, expected_output in tests:
        print(f"[==========] Running {len(tests)} test(s)")
        print(f"[ RUN      ] {test_command}")
        test_result, obtained = verify_output(test_command, expected_output)
        if test_result:
            print(f"[       OK ] {test_command}")
            passed += 1
        else:
            print(f"[  ERROR   ] --- {expected_output} != {obtained}")
            print(f"[  FAILED  ] {test_command}")
            failed_command.append(test_command)
    print(f"[==========] {len(tests)} test(s) run.")
    print(f"[  PASSED  ] {passed} test(s).")
    if len(failed_command):
        print(f"[  FAILED  ] {len(failed_command)} test(s), listed below:")
        for command in failed_command:
            print(f"[  FAILED  ] {command}")

list_command = [
    ("vm/bin/vm config_file/hello.conf", "Hello, World!\n"),
    ("vm/bin/vm config_file/fibonacci.conf", "2\n3\n5\n8\n13\n21\n34\n55\n89\n144\n"),
    ("vm/bin/vm config_file/dotprod.conf", "0.000000\n"),
    ("vm/bin/vm config_file/dotprod_u64.conf", "0\n"),
    ("vm/bin/vm config_file/dotprod_s64.conf", "0\n"),
    ("vm/bin/vm config_file/dotprod_g64.conf", "0.000000\n"),
    ("vm/bin/vm config_file/dotprod.conf", "0.000000\n"),
    ("vm/bin/vm config_file/reduce_sum_u64.conf", "256000\n"),
]

if __name__ == "__main__":
    verify_program_output(list_command)