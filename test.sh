#!/usr/bin/env bash

# --- Configuration ---
HOST="localhost"
PORT="6667"
TIMEOUT=2 # Seconds to wait for server response

# --- Colors for Output ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# --- Global Counters ---
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
FAILED_LIST=()

# ==============================================================================
# UTILITY FUNCTIONS
# ==============================================================================

# Check if netcat is installed
if ! command -v nc &> /dev/null; then
    echo -e "${RED}Error: 'nc' (netcat) is not installed.${NC}"
    exit 1
fi

print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}Starting IRC Tests against $HOST:$PORT${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# Usage: run_test "Test Name" "Input String" "Regex Expected in Output"
run_test() {
    local test_name="$1"
    local input_data="$2"
    local expected_regex="$3"

    ((TOTAL_TESTS++))

    echo -n "Running: $test_name ... "

    # We use echo -e to handle \r\n (CRLF) for IRC protocol compliance.
    # We pipe into nc.
    # -w $TIMEOUT: Stops listening after inactivity.
    response=$(echo -e "$input_data" | nc -w "$TIMEOUT" "$HOST" "$PORT" 2>&1)

    # Check if the response matches the expected regex
    if [[ "$response" =~ $expected_regex ]]; then
        echo -e "${GREEN}[PASS]${NC}"
        ((PASSED_TESTS++))
    else
        echo -e "${RED}[FAIL]${NC}"
        echo -e "  ${YELLOW}Input sent:${NC} ${input_data//[$'\r\n']/\\n}"
        echo -e "  ${YELLOW}Expected matches:${NC} $expected_regex"
        echo -e "  ${YELLOW}Actual response (truncated):${NC}"
        echo "$response" | head -n 5 | sed 's/^/    /'
        
        ((FAILED_TESTS++))
        FAILED_LIST+=("$test_name")
    fi
}

print_summary() {
    echo -e "\n${BLUE}========================================${NC}"
    echo -e "${BLUE}TEST SUMMARY${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "Total Tests:  $TOTAL_TESTS"
    echo -e "Passed:       ${GREEN}$PASSED_TESTS${NC}"
    echo -e "Failed:       ${RED}$FAILED_TESTS${NC}"

    if [ ${#FAILED_LIST[@]} -ne 0 ]; then
        echo -e "\n${RED}Failed Tests:${NC}"
        for test in "${FAILED_LIST[@]}"; do
            echo -e " - $test"
        done
        exit 1
    else
        echo -e "\n${GREEN}All tests passed!${NC}"
        exit 0
    fi
}

# ==============================================================================
# TEST SUITE
# ==============================================================================

print_header

# ------------------------------------------------------------------------------
# Test 1: Basic Connection & Handshake
# Explanation: Send NICK and USER. Expect a 001 (Welcome) numeric code.
# ------------------------------------------------------------------------------
INPUT="NICK tester1\r\nUSER tester1 0 * :Test User\r\n"
EXPECT="001" 
run_test "Handshake (Welcome Message)" "$INPUT" "$EXPECT"

# ------------------------------------------------------------------------------
# Test 2: PING / PONG
# Explanation: Send a PING with a token. Expect a PONG with the same token.
# ------------------------------------------------------------------------------
INPUT="PING :12345ABC\r\n"
EXPECT="PONG :12345ABC"
run_test "Ping/Pong Response" "$INPUT" "$EXPECT"

# ------------------------------------------------------------------------------
# Test 3: Joining a Channel
# Explanation: Complete handshake, then JOIN. Expect JOIN confirmation or names list.
# ------------------------------------------------------------------------------
INPUT="NICK tester2\r\nUSER tester2 0 * :Test User\r\nJOIN #general\r\n"
EXPECT="JOIN :#general"
run_test "Join Channel #general" "$INPUT" "$EXPECT"

# ------------------------------------------------------------------------------
# Test 4: Invalid Command Handling
# Explanation: Send garbage command. Expect 421 (Unknown Command) error.
# ------------------------------------------------------------------------------
INPUT="NICK tester3\r\nUSER tester3 0 * :Test User\r\nBLARGWARG\r\n"
EXPECT="421"
run_test "Error handling (Unknown Command)" "$INPUT" "$EXPECT"

# ------------------------------------------------------------------------------
# Test 5: Nickname Collision (Optional logic depending on server speed)
# Explanation: Attempt to use a reserved or taken nick (if server is persistent).
# Note: Since tests run sequentially, 'tester1' might still be timed out, 
# but let's test a generic bad handshake or just a version check.
# ------------------------------------------------------------------------------
INPUT="VERSION\r\n"
EXPECT="VERSION" # Or specific server version regex
run_test "Version Check" "$INPUT" "$EXPECT"

# ==============================================================================
# FINALIZE
# ==============================================================================

print_summary
