#!/bin/bash


BOLD='\033[1m'
BRIGHT_WHITE='\033[1;37m'
RESET='\033[0m' 

BUILD_DIR='build'
BIN_DIR='bin'
BIN_NAME='datanamesorcery'
INPUT_DIR='input_sample'
HOSTS_FILE='hosts' # Hosts to scan (Scan targets)
DNS_FILE='dns' # DNS servers for testing (Not the scan targets)



configure() {
    echo -e "${BOLD}${BRIGHT_WHITE}Configuring project with cmake...${RESET}"
    mkdir -p ${BUILD_DIR}
    cmake -S . -B ${BUILD_DIR} -DCMAKE_VERBOSE_MAKEFILE=ON
}

build() {
    echo -e "-e ${BOLD}${BRIGHT_WHITE}Building project...${RESET}"
    cmake --build ${BUILD_DIR} --target all -- -j$(nproc) --debug
}


./clean_cmake.sh

configure
build

echo -e "${BOLD}${BRIGHT_WHITE}Unittest shunit2${RESET}"
./shunit2_run.sh


echo -e "${BOLD}${BRIGHT_WHITE}Default output test${RESET}"
./test.sh


echo ""
echo -e "${BOLD}${BRIGHT_WHITE}See unit test results and check build success ... bin should be in $BIN_DIR e.g ${BIN_DIR}/${BIN_NAME}${RESET}"
echo ""
echo -e "${BOLD}${BRIGHT_WHITE}E.g:${RESET}"
echo ""
echo -e "${BOLD}${BRIGHT_WHITE}${BIN_DIR}/${BIN_NAME} -h${RESET}\""
echo ""
echo -e "${BOLD}${BRIGHT_WHITE}See output above for output of ./test.sh and shunit2_run.sh${RESET}\""
