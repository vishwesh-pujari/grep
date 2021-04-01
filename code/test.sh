echo "./grep \"random\" \"random text\""
./grep "random" "random text"

echo -e "\n./grep \"[a-z]\" \"abcd123abcd\""
./grep "[a-z]" "abcd123abcd"

echo -e "\n./grep --basic-regexp \"[0-9]\" \"abcd123abcd\""
./grep --basic-regexp "[0-9]" "abcd123abcd"

echo -e "\n./grep \"[A-Z]\" -G \"abcd123abcd\""
./grep "[A-Z]" -G "abcd123abcd"

echo -e "\n./grep -E \"[a-z]+\" \"abcd123abcd\""
./grep -E "[a-z]+" "abcd123abcd"

echo -e "\n./grep -G \"[a-z]+\" \"abcd123abcd+\""
./grep -G "[a-z]+" "abcd123abcd+"

echo -e "\n./grep -E --ignore-case \"[a-z]+\" \"abcd123ABCD\""
./grep -E --ignore-case "[a-z]+" "abcd123ABCD"

echo -e "\n./grep -F \"[a-z]+\" \"[a-z]+\""
./grep -F "[a-z]+" "[a-z]+"

echo -e "\n./grep -F -i \"[a-z]+\" \"[A-Z]+\""
./grep -F -i "[a-z]+" "[A-Z]+"

echo -e "\n./grep -F \"\" \"anything\""
./grep -F "" "anything"

echo -e "\n./grep -F -E \"[a-z]\" \"anything\""
./grep -F -E "[a-z]" "anything"
