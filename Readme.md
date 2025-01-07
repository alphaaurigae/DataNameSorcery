# DataNameSorcery

> ... reverse DNS scanner.

- Output as default console, json and xml. 
- Domain names, ipv4 and ipv6 as input.

> Still here? Continue if youre mindlessly brave or the like but no warranty given ...


## Build

- Build OS: "Ubuntu 24.04"

... cmake ...
(g++/clang - Cmakelists "option(USE_CLANG" ON/OFF - default clang)

`./build_cmake.sh` - build
`./clean_cmake.sh` - tidy build


## Deps
```
cli11-dev | https://github.com/CLIUtils/CLI11
libuv1-dev | https://libuv.org | https://github.com/libuv/libuv
libpoco-dev | https://pocoproject.org | https://github.com/pocoproject/poco
libxerces-c-dev | https://xerces.apache.org/xerces-c | https://github.com/apache/xerces-c
```

## Run
```
$ bin/DataNameSorcery -hst <host_domain_file> -dns <dns_file> {-def | -json | -xml}

-hst <host_domain_file>` == a file with ipV4 or ipv6 to reverse-dns scan

-dns <dns_file>` == a file with dns servers in ipV4, ipv6, or domain format

-def == use default output

-json == use JSON output 

-xml == use XML output**
```


### Default output
```
$ bin/DataNameSorcery -hst input_sample/hosts -dns input_sample/dns -def
8.8.8.8 -> dns.google
1.1.1.1 -> one.one.one.one
2001:4860:4860::8888 -> dns.google
208.67.222.222 -> dns.sse.cisco.com
Invalid input: 8.&.8.8 -> Unable to resolve
Invalid input: 1.1.i.1 -> Unable to resolve
Invalid input: 200V:4860:4860::8888 -> Unable to resolve
Invalid input: 208.67.2...222 -> Unable to resolve
```


### Json
```
$ bin/DataNameSorcery -hst input_sample/hosts -dns input_sample/dns -json
{"ip":"2001:4860:4860::8888","result":"dns.google"}
{"ip":"1.1.1.1","result":"one.one.one.one"}
{"ip":"208.67.222.222","result":"dns.opendns.com"}
{"ip":"8.8.8.8","result":"dns.google"}
{"ip":"Invalid input: 8.&.8.8","result":"Unable to resolve"}
{"ip":"Invalid input: 200V:4860:4860::8888","result":"Unable to resolve"}
{"ip":"Invalid input: 1.1.i.1","result":"Unable to resolve"}
{"ip":"Invalid input: 208.67.2...222","result":"Unable to resolve"}
```


### XML
```
$ bin/DataNameSorcery -hst input_sample/hosts -dns input_sample/dns -xml
<?xml version="1.0" encoding="UTF-8"?>
<ReverseDnsResults>
</ReverseDnsResults>
<?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>8.8.8.8</IP><ResultText>dns.google</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>1.1.1.1</IP><ResultText>one.one.one.one</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>208.67.222.222</IP><ResultText>dns.opendns.com</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>2001:4860:4860::8888</IP><ResultText>dns.google</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>Invalid input: 200V:4860:4860::8888</IP><ResultText>Unable to resolve</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>Invalid input: 8.&amp;.8.8</IP><ResultText>Unable to resolve</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>Invalid input: 1.1.i.1</IP><ResultText>Unable to resolve</ResultText></Result><?xml version="1.0" encoding="UTF-8" standalone="no" ?><Result><IP>Invalid input: 208.67.2...222</IP><ResultText>Unable to resolve</ResultText></Result></ReverseDnsResults>
```


## Todo

?
