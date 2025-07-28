#!/bin/bash

echo "=== Testing Heredoc Signal Handling ==="
echo "Testing if Ctrl-C properly interrupts heredoc and returns to prompt"

echo 'Testing heredoc signal handling...'
echo 'cat << EOF'
echo 'first line'
echo 'second line'  
echo 'EOF'
echo ''
echo 'echo "After heredoc test"'
echo 'exit'
