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

@test "quotes are still handled properly" {
    run ./dsh <<EOF
echo "hello there everyone"
EOF
    stripped_output=$(echo "$output" | tr -d '\n')
    expected_output="hello there everyonedsh3> dsh3> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "count words in a piped echo command using wc -w" {
    run "./dsh" <<EOF
echo "hello there everyone" | wc -w
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="3dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "use the tr command to change all e's in a piped echo command into q's" {
    run "./dsh" <<EOF
echo "hello there everyone" | tr e q
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="hqllothqrqqvqryonqdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "count all c source code files using ls grep, and wc" {
    run "./dsh" <<EOF
ls -l|grep ".c"|wc -l
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="3dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "use the head command to only print the first 3 lines out of 5" {
    run "./dsh" <<EOF
seq 1 5 | head -n 3
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="123dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "use the tail command to only print the last 3 lines out of 5" {
    run "./dsh" <<EOF
seq 1 5 | tail -n 3
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="345dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "use the sort command to reverse a list of numbers" {
    run "./dsh" <<EOF
seq 1 5 | sort -nr
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="54321dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "use cut to trim echo's output" {
    run "./dsh" <<EOF
echo "hello there everyone" | cut -d" " -f1
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="hellodsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}