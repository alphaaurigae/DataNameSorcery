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
$ bin/DataNameSorcery --host <host_file> --target <target_file> [--def | --json | --xml]

--host <host_domain_file>` == IPv4, IPv6, or URL \n

--target <dns_file>` == IPv4, IPv6, or URL \n

--def == default output

--json == JSON output 

--xml == XML output**
```


### Default console
```
$ bin/datanamesorcery --host input_sample/host --target input_sample/target --def | sort | tr -d '\r' | sed 's/[[:space:]]*$//'
1.1.1.1 -> one.one.one.one
2001:4860:4860::8888 -> dns.google
8.8.8.8 -> dns.google
Invalid input: 1.1.i.1 -> Unable to resolve
Invalid input: 200V:4860:4860::8888 -> Unable to resolve
Invalid input: 208.67.2...222 -> Unable to resolve
Invalid input: 8.&.8.8 -> Unable to resolve

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