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

@test "cd with quoted directory" {
    current=$(pwd)

    cd /tmp
    mkdir -p dsh-test

    run "${current}/dsh" <<EOF
cd "dsh-test"
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="/tmp/dsh-testdsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "dragon works as expected" {
    run "./dsh" <<EOF
dragon
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>$(cat "drgn.txt" | tr -d '[:space:]')dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "count lines in a file using wc -l" {
    run "./dsh" <<EOF
wc -l drgn.txt
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="37drgn.txtdsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "exit immediately" {
    run "./dsh" <<EOF
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "able to create a file using touch" {
    current=$(pwd)

    cd /tmp

    run "${current}/dsh" <<EOF
touch temp
[ -e temp ]
echo $?
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="0dsh2>dsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "options work with echo -e (newlines are not in stripped output)" {
    run "./dsh" <<EOF
echo -e "line 1\nline 2\nline 3"
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="line1line2line3dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "attempting to read a file that doesn't exist fails" {
    run "./dsh" <<EOF
cat thisFileNoExisto
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="cat:thisFileNoExisto:Nosuchfileordirectorydsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "attempting to cd to a directory that doesn't exist fails" {
    run "./dsh" <<EOF
cd whereAmI
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="dsh2>dsh:cd:whereAmI:Nosuchfileordirectorydsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}