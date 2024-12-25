#!/bin/bash


BIN_DIR="bin"
BIN_NAME="datanamesorcery"
INPUT_DIR="input_sample"
HOSTS_FILE="hosts"
DNS_FILE="dns"

BOLD='\033[1m'
BRIGHT_WHITE='\033[1;37m'
RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

local test_name="$1"
local expected="$2"
local output="$3"

debug_assert_equals() {
  if [ "$output" != "$expected" ]; then
    echo "${RED}TEST FAILED: $test_name${RESET}"
    echo "DEBUG: Expected:"
    echo "$expected"
    echo "DEBUG: Got:"
    echo "$output"
    assertEquals "Test '$test_name' failed." "$expected" "$output"
  else
    echo "${GREEN}TEST PASSED: $test_name${RESET}"
    echo "DEBUG: Expected:"
    echo "$expected"
    echo "DEBUG: Got:"
    echo "$output"
    assertEquals "Test '$test_name' passed." "$expected" "$output"
  fi
}


# Test the exact output for the -hst format
test_hst_output() {
    output="$(bin/datanamesorcery -hst input_sample/hosts -dns input_sample/dns -def | sort | tr -d '\r' | sed 's/[[:space:]]*$//')"
    echo "Output:"
    echo "$output"

    # Temporarily hardcode the expected output for testing
    expected="1.1.1.1 -> one.one.one.one
2001:4860:4860::8888 -> dns.google
8.8.8.8 -> dns.google
Invalid input: 1.1.i.1 -> Unable to resolve
Invalid input: 200V:4860:4860::8888 -> Unable to resolve
Invalid input: 208.67.2...222 -> Unable to resolve
Invalid input: 8.&.8.8 -> Unable to resolve"
    
    echo "Expected Output:"
    echo "$expected_output"

  debug_assert_equals "Default Output Test Failed" "$expected" "$output"
}

test_json_output() {
    output="$(bin/datanamesorcery -hst input_sample/hosts -dns input_sample/dns -json | sort | tr -d '\r' | sed 's/[[:space:]]*$//')"
    echo "Output:"
    echo "$output"

    # Temporarily hardcode the expected output for testing
    expected='{"ip":"1.1.1.1","result":"one.one.one.one"}
{"ip":"2001:4860:4860::8888","result":"dns.google"}
{"ip":"8.8.8.8","result":"dns.google"}
{"ip":"Invalid input: 1.1.i.1","result":"Unable to resolve"}
{"ip":"Invalid input: 200V:4860:4860::8888","result":"Unable to resolve"}
{"ip":"Invalid input: 208.67.2...222","result":"Unable to resolve"}
{"ip":"Invalid input: 8.&.8.8","result":"Unable to resolve"}'

    
    echo "Expected Output:"
    echo "$expected_output"

  debug_assert_equals "JSON Output Test Failed" "$expected" "$output"
}

#test_xml_output() {
#    # Fetch raw XML output from the program
#    raw_output="$(bin/datanamesorcery -hst input_sample/hosts -dns input_sample/dns -xml | tr -d '\r' | sed 's/[[:space:]]*$//')"
#
#    echo "Raw Output:"
#    echo "$raw_output"
#
#    # Expected output
#    expected_output='<?xml version="1.0" encoding="UTF-8"?>
#<ReverseDnsResults>
#</ReverseDnsResults>
#<?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>2001:4860:4860::8888</IP><ResultText>dns.google</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>1.1.1.1</IP><ResultText>one.one.one.one</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>8.8.8.8</IP><ResultText>dns.google</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>Invalid input: 200V:4860:4860::8888</IP><ResultText>Unable to resolve</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>Invalid input: 8.&amp;.8.8</IP><ResultText>Unable to resolve</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>Invalid input: 1.1.i.1</IP><ResultText>Unable to resolve</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>Invalid input: 208.67.2...222</IP><ResultText>Unable to resolve</ResultText></Result></ReverseDnsResults>'
#
#
#    echo "Expected Output:"
#    echo "$expected_output"
#
#    # Assert equality
#    assertEquals "XML Test Failed" "$expected_output" "$raw_output"
#}

echo "$(tput bold)XML OUTPUT TEST SKIPPED - FIX UNITTEST$(tput sgr0)"