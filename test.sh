#!/bin/bash


BIN_DIR='bin'
BIN_NAME='datanamesorcery'
INPUT_DIR='input_sample'
HOSTS_FILE='host' # Hosts to scan (Scan targets)
DNS_FILE='target' # DNS servers for testing (Not the scan targets)


echo ""
${BIN_DIR}/${BIN_NAME}
echo ""
echo ""
${BIN_DIR}/${BIN_NAME} --host ${INPUT_DIR}/${HOSTS_FILE} --target ${INPUT_DIR}/${DNS_FILE} --def
echo ""
${BIN_DIR}/${BIN_NAME} --host ${INPUT_DIR}/${HOSTS_FILE} --target ${INPUT_DIR}/${DNS_FILE} --json
echo ""
${BIN_DIR}/${BIN_NAME} --host ${INPUT_DIR}/${HOSTS_FILE} --target ${INPUT_DIR}/${DNS_FILE} --xml
echo ""