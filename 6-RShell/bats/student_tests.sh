#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "grep dshlib.c" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
ls | grep dshlib.c
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dshlib.ccmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "grep .c wc" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
ls | grep .c | wc
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:12344442cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "echo" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo hi
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234hicmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "echo pipe" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo hi|echo hii
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234hiicmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "cd" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
cd hi
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234Error:Invaliddirectorycmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "cd cd pipe" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
cd hi| cd
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234Error:InvaliddirectoryError:Invalidcdcommandcmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "cd echo cd invalid echo" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
cd |echo hi |cd invalid| echo byee
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234Error:InvalidcdcommandError:Invaliddirectorybyeecmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "invalid command" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
hi
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234execvp:Nosuchfileordirectorysocketservermode:addr:0.0.0.0:1234->Single-ThreadedModecmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "echoing max pipe" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo 1 | echo 2 | echo 3 | echo 4| echo 5| echo 6| echo 7 | echo 8
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:12348cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "exit" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
exit
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "stop-server" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "quotes" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo "hello bye"
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234hellobyecmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "valid invalid" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo "hello bye" | hi 
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234execvp:Nosuchfileordirectorysocketservermode:addr:0.0.0.0:1234->Single-ThreadedModecmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "valid invalid valid" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo "hello bye" | hi | echo hi
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234hicmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "space" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
 
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}
