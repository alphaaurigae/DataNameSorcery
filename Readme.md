# DataNameSorcery
>... find some name for a DNS app thats never been used before, is funny and not too long.... well this has DNS in it and an evil touch anagram so theres that.

> Well then... this is a reverse DNS scanner, which can print the output as default, json and xml. It accepts domain names, ipv4 and ipv6 as input. **

> I tried to make the code a minimal secure but im a c++ noob and havent spent much time on it, so dont count on it for large scale production ^^

> There is minimal input validation. The program only accepts valid IPs / domains with character limit (lib poco). 

> Comments in the input files preceeded by // are ignored too.**


> Still here? Continue if youre mindlessly brave or the like but no warranty given ...


## BUILD
There is a makefile and a CMakeLists.txt (was bored, yeah)
### MAKE
`make` (lol)
> Creates a bin dir in proj root for the bin
### CMAKE
`mkdir -p build`
`cd build`
`cmake ..`
`cmake --build .`
> Bin dir in build dir


## USAGE
**Usage: ./reverse_dns2 -hst <host_domain_file> -dns <dns_file> {-def | -json | -xml}

`-hst <host_domain_file>` == a file with ipV4 or ipv6 to reverse-dns scan

`-dns <dns_file>` == a file with dns servers in ipV4, ipv6, or domain format

`-def` == use default output

`-json` == use JSON output

`-xml` == use XML output**

test all setup: `make && cd bin && ./datanamesorcery -hst ../input_sample/hosts -dns ../input_sample/dns -def && ./datanamesorcery -hst ../input_sample/hosts -dns ../input_sample/dns -json && ./datanamesorcery -hst ../input_sample/hosts -dns ../input_sample/dns -xml`


## Tests with valid and invalid input

### Default output
`$ ./datanamesorcery -hst ../input_sample/hosts -dns ../input_sample/dns -def

8.8.8.8 -> dns.google

1.1.1.1 -> one.one.one.one

208.67.222.222 -> dns.sse.cisco.com

2001:4860:4860::8888 -> dns.google

Invalid input: 200V:4860:4860::8888 -> Unable to resolve

Invalid input: 8.&.8.8 -> Unable to resolve

Invalid input: 1.1.i.1 -> Unable to resolve

Invalid input: 208.67.2...222 -> Unable to resolve

 `


### Json output with https://github.com/nlohmann/json
`$ ./datanamesorcery -hst ../input_sample/hosts -dns ../input_sample/dns -json

{"ip":"8.8.8.8","result":"dns.google"}

{"ip":"1.1.1.1","result":"one.one.one.one"}

{"ip":"2001:4860:4860::8888","result":"dns.google"}

{"ip":"208.67.222.222","result":"dns.umbrella.com"}

{"ip":"Invalid input: 200V:4860:4860::8888","result":"Unable to resolve"}

{"ip":"Invalid input: 8.&.8.8","result":"Unable to resolve"}

{"ip":"Invalid input: 1.1.i.1","result":"Unable to resolve"}

{"ip":"Invalid input: 208.67.2...222","result":"Unable to resolve"}
`



### XML output with lib https://github.com/apache/xerces-c
 `$ ./datanamesorcery -hst ../input_sample/hosts -dns ../input_sample/dns -xml

<?xml version="1.0" encoding="UTF-8"?>

<ReverseDnsResults>

</ReverseDnsResults>

  <Result>

    <IP>8.8.8.8</IP>

    <ResultText>dns.google</ResultText>

  </Result>

  <Result>

    <IP>1.1.1.1</IP>

    <ResultText>one.one.one.one</ResultText>

  </Result>

  <Result>

    <IP>2001:4860:4860::8888</IP>

    <ResultText>dns.google</ResultText>

  </Result>

  <Result>

    <IP>208.67.222.222</IP>

    <ResultText>dns.umbrella.com</ResultText>

  </Result>

  <Result>

    <IP>Invalid input: 200V:4860:4860::8888</IP>

    <ResultText>Unable to resolve</ResultText>

  </Result>

  <Result>

    <IP>Invalid input: 8.&.8.8</IP>

    <ResultText>Unable to resolve</ResultText>

  </Result>

  <Result>

    <IP>Invalid input: 208.67.2...222</IP>

    <ResultText>Unable to resolve</ResultText>

  </Result>

  <Result>

    <IP>Invalid input: 1.1.i.1</IP>

    <ResultText>Unable to resolve</ResultText>

  </Result>

</ReverseDnsResults> `

