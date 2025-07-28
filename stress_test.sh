#!/bin/bash

echo "=== Memory Stress Test for Minishell ==="
echo "Testing dynamic allocation with various input sizes..."

# Test with very long variable names (bash supports them)
export VERY_LONG_VARIABLE_NAME_THAT_EXCEEDS_TRADITIONAL_LIMITS_BUT_SHOULD_WORK_FINE_WITH_DYNAMIC_ALLOCATION="test_value"

# Test with very long command lines
LONG_CMD='echo "'
for i in {1..500}; do
    LONG_CMD+="A"
done
LONG_CMD+='"'

echo "Testing with long variable expansion..."
echo 'echo $VERY_LONG_VARIABLE_NAME_THAT_EXCEEDS_TRADITIONAL_LIMITS_BUT_SHOULD_WORK_FINE_WITH_DYNAMIC_ALLOCATION' | valgrind --tool=memcheck --leak-check=full --suppressions=ignore_readline.supp ./minishell 2>&1 | grep -E "(ERROR|definitely lost|possibly lost)"

echo "Testing with long command lines..."
echo "$LONG_CMD" | valgrind --tool=memcheck --leak-check=full --suppressions=ignore_readline.supp ./minishell 2>&1 | grep -E "(ERROR|definitely lost|possibly lost)"

echo "Testing with nested quotes and expansions..."
echo 'echo "Hello $USER and $VERY_LONG_VARIABLE_NAME_THAT_EXCEEDS_TRADITIONAL_LIMITS_BUT_SHOULD_WORK_FINE_WITH_DYNAMIC_ALLOCATION"' | valgrind --tool=memcheck --leak-check=full --suppressions=ignore_readline.supp ./minishell 2>&1 | grep -E "(ERROR|definitely lost|possibly lost)"

echo "Testing with heredoc..."
(echo 'cat << EOF'; echo 'Line 1 with $USER'; echo 'Line 2 with long text'; echo 'EOF') | valgrind --tool=memcheck --leak-check=full --suppressions=ignore_readline.supp ./minishell 2>&1 | grep -E "(ERROR|definitely lost|possibly lost)"

echo "=== Stress test complete ==="
