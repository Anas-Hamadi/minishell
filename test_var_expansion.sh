#!/bin/bash

echo "=== Comprehensive Variable Expansion Test ==="
echo ""

# Test script for minishell variable expansion
cat << 'EOF' | ./minishell
echo "=== Testing Basic \$? Expansion ==="
echo "1. Initial exit code"
echo "Exit code: $?"

echo "2. After successful command"
true
echo "Exit code: $?"

echo "3. After failed command"
false
echo "Exit code: $?"

echo "4. After command not found"
nonexistentcmd
echo "Exit code: $?"

echo ""
echo "=== Testing Environment Variable Expansion ==="
echo "5. Setting and expanding custom variable"
export MY_TEST_VAR=hello_world
echo "MY_TEST_VAR: $MY_TEST_VAR"

echo "6. Expanding HOME variable"
echo "HOME: $HOME"

echo "7. Expanding PATH (first 50 chars)"
echo "$PATH" | cut -c1-50

echo "8. Expanding undefined variable"
echo "UNDEFINED_VAR: '$UNDEFINED_VAR'"

echo ""
echo "=== Testing \$? in Different Contexts ==="
echo "9. \$? in single quotes (should not expand)"
false
echo 'Exit code: $?'

echo "10. \$? in double quotes (should expand)"
false
echo "Exit code: $?"

echo "11. \$? mixed with other text"
false
echo "The last command returned $? as exit code"

echo "12. Multiple \$? in same command"
false
echo "Exit: $? and again: $?"

echo ""
echo "=== Testing \$? in Pipes ==="
echo "13. \$? after pipe success"
echo hello | cat
echo "Exit code: $?"

echo "14. \$? after pipe failure"
true | false
echo "Exit code: $?"

echo "15. \$? in pipe itself"
false
echo "Exit was $?" | cat

echo ""
echo "=== Testing \$? in Heredocs ==="
echo "16. \$? expansion in heredoc"
false
cat << END
The previous command failed with exit code: $?
This should show 1.
END

echo "17. \$? with quotes in heredoc"
true
cat << 'LITERAL'
This should show literal: $?
LITERAL

echo ""
echo "=== Testing Complex Variable Scenarios ==="
echo "18. Variable expansion with special characters"
export SPECIAL_VAR="hello world with spaces"
echo "SPECIAL: '$SPECIAL_VAR'"

echo "19. Multiple variables in one command"
export VAR1=first
export VAR2=second
echo "$VAR1 and $VAR2 together"

echo "20. \$? combined with environment variables"
false
echo "Exit: $? and HOME: $HOME"

echo "21. Variable expansion in command arguments"
export CMD=echo
export ARG=test
$CMD $ARG

echo "22. Testing \$? persistence"
false
echo "First check: $?"
echo "Second check: $?" 

echo ""
echo "=== Testing Edge Cases ==="
echo "23. Empty variable"
export EMPTY_VAR=""
echo "Empty: '$EMPTY_VAR'"

echo "24. Variable with numbers"
export VAR123=value123
echo "Numbered: $VAR123"

echo "25. Variable with underscores"
export MY_LONG_VAR_NAME=underscore_test
echo "Underscore: $MY_LONG_VAR_NAME"

echo "26. Invalid variable patterns"
echo "Invalid \$: $"
echo "Invalid \$1: $1"
echo "Invalid \${}: ${}"

echo ""
echo "=== Test Complete ==="
exit
EOF
