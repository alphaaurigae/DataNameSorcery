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
libpoko-dev - https://github.com/pocoproject/poco
libxerces-c-dev - https://github.com/apache/xerces-c
nlohmann-json3-dev - https://github.com/nlohmann/json
libuv1-dev - https://github.com/libuv/libuv

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


# Todo

```
Address Sanitizer Output: Detected multiple direct and indirect memory leaks during unit tests and program execution.

 Reproduce: 
"run ./build_cmake.sh (clang or g++) (clang++-18/g++-13)
sanitizers uncommented (cmakelists)
'
    set(COMMON_SANITIZERS
#"address"
#"leak"
#"pointer-compare"
'

see DnsResolver.h "class DnsResolver"

==426656==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 320 byte(s) in 2 object(s) allocated from:
    #0 0x5e956808ca71 in operator new(unsigned long) (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x10aa71) (BuildId: 23cf557bd0e8c57231f5fe8b394f160172f123ef)
    #1 0x5e9568097d8a in DnsResolver::asyncReverseDnsLookup(...) (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x115d8a)
    #2 0x5e9568091baa in DnsResolver::reverseDnsScannerAsync(...) (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x10fbaa)
    #3 0x5e956808f0d0 in main (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x10d0d0)
    #4 0x71513302a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #5 0x71513302a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #6 0x5e9567fb3604 in _start (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x31604)

Indirect leak of 800 byte(s) in 5 object(s) allocated from:
    #0 0x5e956808ca71 in operator new(unsigned long) (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x10aa71)
    #1 0x5e9568097d8a in DnsResolver::asyncReverseDnsLookup(...) (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x115d8a)
    #2 0x5e9568091baa in DnsResolver::reverseDnsScannerAsync(...) (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x10fbaa)
    #3 0x5e956808f0d0 in main (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x10d0d0)
    #4 0x71513302a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #5 0x71513302a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #6 0x5e9567fb3604 in _start (/home/mmmm/Desktop/proj/git/DataNameSorcery/bin/datanamesorcery+0x31604)

SUMMARY: AddressSanitizer: 1120 byte(s) leaked in 7 allocation(s).
```
