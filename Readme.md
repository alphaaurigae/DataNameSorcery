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


### Default console
```
$ bin/datanamesorcery --host input_sample/host --target input_sample/target --json | sort | tr -d '\r' | sed 's/[[:space:]]*$//'
{"ip":"1.1.1.1","result":"one.one.one.one"}
{"ip":"2001:4860:4860::8888","result":"dns.google"}
{"ip":"8.8.8.8","result":"dns.google"}
{"ip":"Invalid input: 1.1.i.1","result":"Unable to resolve"}
{"ip":"Invalid input: 200V:4860:4860::8888","result":"Unable to resolve"}
{"ip":"Invalid input: 208.67.2...222","result":"Unable to resolve"}
{"ip":"Invalid input: 8.&.8.8","result":"Unable to resolve"}
```


### Json
```
$ bin/DataNameSorcery -host input_sample/host --target input_sample/dns --json
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
$ bin/datanamesorcery --host input_sample/host --target input_sample/target --xml | sort | tr -d '\r' | sed 's/[[:space:]]*$//'
<dns><ip>1.1.1.1</ip><result>one.one.one.one</result></dns>
<dns><ip>2001:4860:4860::8888</ip><result>dns.google</result></dns>
<dns><ip>8.8.8.8</ip><result>dns.google</result></dns>
<dns><ip>Invalid input: 1.1.i.1</ip><result>Unable to resolve</result></dns>
<dns><ip>Invalid input: 200V:4860:4860::8888</ip><result>Unable to resolve</result></dns>
<dns><ip>Invalid input: 208.67.2...222</ip><result>Unable to resolve</result></dns>
<dns><ip>Invalid input: 8.&.8.8</ip><result>Unable to resolve</result></dns>
```


## Todo

- cli11 just setup as BasED template, needs finetuning.
- Check xml
- check unittests
- ?