#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

setup() {
    # Start the server in the background
    ./dsh -s & 
    SERVER_PID=$!
    sleep 1 
}



@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

# local mode tests
@test "local: quotes are still handled properly" {
    run ./dsh <<EOF
echo "hello there everyone"
EOF
    stripped_output=$(echo "$output" | tr -d '\n')
    expected_output="hello there everyonelocal modedsh4> dsh4> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "local: count words in a piped echo command using wc -w" {
    run "./dsh" <<EOF
echo "hello there everyone" | wc -w
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="3localmodedsh4>dsh4>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

#client-server tests

@test "client: client can connect to server and receive a response" {
    output=$(echo "echo hello" | ./dsh -c)
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>hellodsh4>cmdloopreturned0"

    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "client: count words in a piped echo command using wc -w in client" {
    output=$(echo "echo "hello there everyone" | wc -w" | ./dsh -c)
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>3dsh4>cmdloopreturned0"

    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "client: quotes are still handled properly" {
    output=$(echo "echo "hello there everyone"" | ./dsh -c)
    stripped_output=$(echo "$output" | tr -d '\n')
    expected_output="socket client mode:  addr:127.0.0.1:1234dsh4> hello there everyonedsh4> cmd loop returned 0"

    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "client: stop-server command works properly" {
    output=$(echo "stop-server" | ./dsh -c)
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>socketservermode:addr:0.0.0.0:1234->Single-ThreadedModecmdloopreturned0"

    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "client: exit command works properly" {
    output=$(echo "exit" | ./dsh -c)
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>socketservermode:addr:0.0.0.0:1234->Single-ThreadedModecmdloopreturned0"

    kill $SERVER_PID

    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]

}

@test "client: built-in works in a pipe" {
    output=$(echo "dragon | wc -l" | ./dsh -c)
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>40dsh4>cmdloopreturned0"

    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}