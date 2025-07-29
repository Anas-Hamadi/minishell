#!/bin/bash

echo "=== Exit Status Code Comprehensive Test ==="
echo ""

# Test script for minishell exit codes
cat << 'EOF' | ./minishell
echo "=== Testing Built-in Commands ==="
echo "1. echo (success)"
echo hello
echo "Exit code: $?"

echo "2. pwd (success)"
pwd
echo "Exit code: $?"

echo "3. cd (error - directory doesn't exist)"
cd /nonexistent_directory_test
echo "Exit code: $?"

echo "4. cd (success)"
cd /tmp
echo "Exit code: $?"
cd -

echo "5. export (success)"
export TEST_VAR=test_value
echo "Exit code: $?"

echo "6. env (success)"
env | grep TEST_VAR
echo "Exit code: $?"

echo "7. unset (success)"
unset TEST_VAR
echo "Exit code: $?"

echo "8. unset (error - no arguments)"
unset
echo "Exit code: $?"

echo ""
echo "=== Testing External Commands ==="
echo "9. true command"
true
echo "Exit code: $?"

echo "10. false command"
false
echo "Exit code: $?"

echo "11. Command not found"
nonexistentcommand123
echo "Exit code: $?"

echo "12. ls (success)"
ls /dev/null
echo "Exit code: $?"

echo "13. ls (error - file doesn't exist)"
ls /nonexistent_file_test
echo "Exit code: $?"

echo ""
echo "=== Testing Pipes ==="
echo "14. echo | cat (both succeed)"
echo hello | cat
echo "Exit code: $?"

echo "15. true | false (last command fails)"
true | false
echo "Exit code: $?"

echo "16. false | true (last command succeeds)"
false | true
echo "Exit code: $?"

echo "17. echo | nonexistentcmd (last command fails)"
echo hello | nonexistentcmd123
echo "Exit code: $?"

echo "18. Complex pipe: true | false | true"
true | false | true
echo "Exit code: $?"

echo ""
echo "=== Testing Heredoc with Exit Codes ==="
echo "19. cat with heredoc containing \$?"
false
cat << END
Previous command exit code was: $?
END
echo "Current exit code: $?"

echo ""
echo "=== Test Complete ==="
exit
EOF
