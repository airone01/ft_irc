#!/usr/bin/env bash

# --- Configuration ---
HOST="localhost"
PORT="6667"
MESSAGE_COUNT=5000  # Number of commands to send
TIMEOUT=5           # Netcat timeout in seconds
COMMAND="PING :token" # Command to spam (e.g., PING :token or PRIVMSG #spam :msg)

# --- Colors ---
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

# ==============================================================================
# IRC Input Generation
# ==============================================================================

# Generates the full input stream, including handshake and spam messages.
generate_input() {
    # 1. Handshake: Essential for the server to enter the registered state.
    echo -e "CAP LS 302\r\n"
    echo -e "NICK speedtest\r\n"
    echo -e "USER tester 0 * :Speed Test User\r\n"
    
    # 2. Command Spam: Loop the chosen command.
    for ((i=1; i<=$MESSAGE_COUNT; i++)); do
        # Use COMMAND from config, append required CRLF termination
        echo -e "$COMMAND $i\r\n"
    done
    
    # 3. Cleanup: Quit the connection.
    echo -e "QUIT\r\n"
}

# ==============================================================================
# Execution
# ==============================================================================

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}      IRC Server Performance Test       ${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "${YELLOW}Host: ${NC}$HOST:$PORT"
echo -e "${YELLOW}Commands to Send: ${NC}$MESSAGE_COUNT (${COMMAND/PING :token/PING} command)"
echo -e "${YELLOW}Netcat Timeout: ${NC}$TIMEOUT seconds"
echo

# Store generated commands in a variable to measure only the execution time.
INPUT_DATA=$(generate_input)
# Calculate the size of the input data for reference
DATA_SIZE=$(echo "$INPUT_DATA" | wc -c)

echo -e "${BLUE}[INFO] Starting test...${NC}"

# Use time command to measure elapsed time (real)
START_TIME=$(date +%s.%N)

# Pipe all input data to netcat. Netcat will terminate after the timeout or QUIT.
RESPONSE=$(echo "$INPUT_DATA" | nc -w "$TIMEOUT" "$HOST" "$PORT" 2>&1)

END_TIME=$(date +%s.%N)

# Calculate duration
DURATION=$(echo "$END_TIME - $START_TIME" | bc)

# ==============================================================================
# Analysis and Summary
# ==============================================================================

echo -e "${BLUE}----------------------------------------${NC}"
echo -e "${BLUE}Test Results${NC}"
echo -e "${BLUE}----------------------------------------${NC}"

# Check for successful registration response (001)
if echo "$RESPONSE" | grep -q "001"; then
    echo -e "${GREEN}[PASS] Handshake successful (001 received).${NC}"
else
    echo -e "${RED}[FAIL] Handshake failed (001 not received). Aborting speed check.${NC}"
    exit 1
fi

# Determine if PONGs were received
PONG_COUNT=$(echo "$RESPONSE" | grep -c "PONG :token")

echo -e "Total test duration: ${GREEN}${DURATION} seconds${NC}"

# If running PING tests, calculate transactions per second (TPS)
if [[ "$COMMAND" == "PING :token" ]]; then
    # Subtract 3 commands for the handshake and 1 for QUIT
    TRANSACTIONS=$((MESSAGE_COUNT - 4)) 
    
    # Calculate transactions per second
    if (( $(echo "$DURATION > 0" | bc -l) )); then
        TPS=$(echo "$TRANSACTIONS / $DURATION" | bc -l)
        echo -e "Processed PING Commands: ${YELLOW}$TRANSACTIONS${NC}"
        echo -e "Transactions Per Second (TPS): ${GREEN}$(printf "%.2f" $TPS)${NC}"
    else
        echo -e "${RED}Duration too short to calculate accurate TPS.${NC}"
    fi
fi

# Print a small snippet of the response for debugging
echo -e "\n${BLUE}Response Snippet (First 5 Lines):${NC}"
echo "$RESPONSE" | head -n 5 | sed 's/^/    /'

echo -e "${BLUE}========================================${NC}"

if (( $(echo "$DURATION < 1.0" | bc -l) )); then
    echo -e "${GREEN}Server performance is good for ${MESSAGE_COUNT} commands.${NC}"
else
    echo -e "${YELLOW}Server processing is slower than ideal. Investigate Dispatcher/I/O loop.${NC}"
fi
