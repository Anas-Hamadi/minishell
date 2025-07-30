#!/bin/bash

echo "=== Parse Error Handling Comprehensive Test ==="
echo ""

# Test script for minishell parse error handling
cat << 'EOF' | ./minishell
echo "=== Testing Invalid Operators ==="
echo "1. Testing <<<" 
<<<
echo "Exit code: $?"

echo "2. Testing <<< with argument"
<<< word
echo "Exit code: $?"

echo "3. Testing ||"
||
echo "Exit code: $?"

echo "4. Testing &&"
&&
echo "Exit code: $?"

echo "5. Testing multiple pipes"
|||
echo "Exit code: $?"

echo ""
echo "=== Testing Invalid Characters ==="
echo "6. Testing semicolon"
;
echo "Exit code: $?"

echo "7. Testing single &"
&
echo "Exit code: $?"

echo "8. Testing parentheses"
(
echo "Exit code: $?"

echo "9. Testing )"
)
echo "Exit code: $?"

echo "10. Testing *"
*
echo "Exit code: $?"

echo ""
echo "=== Testing Redirection Errors ==="
echo "11. Testing < without filename"
<
echo "Exit code: $?"

echo "12. Testing > without filename"
>
echo "Exit code: $?"

echo "13. Testing >> without filename"
>>
echo "Exit code: $?"

echo "14. Testing << without delimiter"
<<
echo "Exit code: $?"

echo ""
echo "=== Testing Valid Syntax (should work) ==="
echo "15. Valid pipe"
echo hello | cat
echo "Exit code: $?"

echo "16. Valid redirection"
echo test > /tmp/test_file
cat /tmp/test_file
rm /tmp/test_file
echo "Exit code: $?"

echo "17. Valid heredoc"
cat << END
This is a valid heredoc
END
echo "Exit code: $?"

echo ""
echo "=== Test Complete ==="
exit
EOF
