#!/bin/bash

# Set up environment variables for testing
BIN_DIR="bin"
BIN_NAME="datanamesorcery"
INPUT_DIR="input_sample"
HOSTS_FILE="hosts"
DNS_FILE="dns"


# Test the exact output for the -hst format
test_hst_output() {
    output="$(bin/datanamesorcery -hst input_sample/hosts -dns input_sample/dns -def | sort | tr -d '\r' | sed 's/[[:space:]]*$//')"
    echo "Output:"
    echo "$output"

    # Temporarily hardcode the expected output for testing
    expected_output="1.1.1.1 -> one.one.one.one
2001:4860:4860::8888 -> dns.google
8.8.8.8 -> dns.google
Invalid input: 1.1.i.1 -> Unable to resolve
Invalid input: 200V:4860:4860::8888 -> Unable to resolve
Invalid input: 208.67.2...222 -> Unable to resolve
Invalid input: 8.&.8.8 -> Unable to resolve"
    
    echo "Expected Output:"
    echo "$expected_output"

    assertEquals "Default Output Test Failed" "$expected_output" "$output"
}

test_json_output() {
    output="$(bin/datanamesorcery -hst input_sample/hosts -dns input_sample/dns -json | sort | tr -d '\r' | sed 's/[[:space:]]*$//')"
    echo "Output:"
    echo "$output"

    # Temporarily hardcode the expected output for testing
    expected_output='{"ip":"1.1.1.1","result":"one.one.one.one"}
{"ip":"2001:4860:4860::8888","result":"dns.google"}
{"ip":"8.8.8.8","result":"dns.google"}
{"ip":"Invalid input: 1.1.i.1","result":"Unable to resolve"}
{"ip":"Invalid input: 200V:4860:4860::8888","result":"Unable to resolve"}
{"ip":"Invalid input: 208.67.2...222","result":"Unable to resolve"}
{"ip":"Invalid input: 8.&.8.8","result":"Unable to resolve"}'

    
    echo "Expected Output:"
    echo "$expected_output"

    assertEquals "JSON Output Test Failed" "$expected_output" "$output"
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