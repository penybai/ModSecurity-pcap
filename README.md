# 功能测试

以`pcap connector`连接`ModSecurity`，测试其功能：

编译安装`libyajl`库以使得`Modsecurity`支持J`SON`输出：
```
apt-get install libyajl-dev
```

编译安装`ModSecurity`库：
```
下载modsecurity-v3.0.6.tar.gz
tar -zxvf modsecurity-v3.0.6.tar.gz
cd modsecurity-v3.0.6/
./configure --with-yajl
make -j 10
make install
安装后目录在/usr/local/modsecurity
```

编译`Modsecurity-pcap`：

```
官方版版本过老，不支持3.0.6版本，使用修改版
git clone https://github.com/penybai/ModSecurity-pcap
cd ModSecurity-pcap/
g++ -o pcap pcap.cc -lpcap -I/usr/local/modsecurity/include -L/usr/local/modsecurity/lib -lmodsecurity -std=c++11
```

运行`ModSecurity-pcap`:

```
将libmodsecurity.so所在路径加入运行时配置，在/etc/ld.so.conf加入/usr/local/modsecurity/lib，并执行ldconfig命令

执行pcap命令检测指定的HTTP协议pcap文件，终端打印输出的（或/var/log/modsec_audit.log）即为审计日志：
root@NetOS:~/ModSecurity-pcap# ./pcap ./0830http1.pcap
Phase: 0 (0 rules)
Phase: 1 (0 rules)
Phase: 2 (2 rules)
    Rule ID: 200000--0x13caab0
    Rule ID: 200001--0x13cb560
Phase: 3 (4 rules)
    Rule ID: 200002--0x13cc9f0
    Rule ID: 200003--0x13d0a00
    Rule ID: 200004--0x13d14c0
    Rule ID: 200005--0x13d2410
Phase: 4 (0 rules)
Phase: 5 (0 rules)
Phase: 6 (0 rules)
Phase: 7 (0 rules)
Server log callback is not set -- [client 10.174.123.107] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930598"] [ref "v163443,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930598"]
Server log callback is not set -- [client 10.176.81.250] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930598"] [ref "v643222,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930598"]
Server log callback is not set -- [client 10.177.18.7] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930598"] [ref "v257449,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930598"]
Server log callback is not set -- [client 10.177.52.111] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930599"] [ref "v822511,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930599"]
Server log callback is not set -- [client 10.172.123.111] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.1.67"] [uri "/AgentUpdate/server.htc H"] [unique_id "1649930600"] [ref "v325,1"] [hostname "10.172.1.67"] [uri "/AgentUpdate/server.htc H"] [unique_id "1649930600"]
Server log callback is not set -- [client 10.176.44.149] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930600"] [ref "v48030,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930600"]
Server log callback is not set -- [client 10.176.193.146] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930600"] [ref "v55026,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930600"]
Server log callback is not set -- [client 10.177.51.15] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930601"] [ref "v21458,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930601"]
Server log callback is not set -- [client 10.174.86.102] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930601"] [ref "v30926,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930601"]
Server log callback is not set -- [client 10.175.109.241] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930602"] [ref "v244915,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930602"]
Server log callback is not set -- [client 10.172.124.167] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.1.67"] [uri "/AgentUpdate/server.htc H"] [unique_id "1649930602"] [ref "v23700,1"] [hostname "10.172.1.67"] [uri "/AgentUpdate/server.htc H"] [unique_id "1649930602"]
Server log callback is not set -- [client 10.172.124.247] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.1.67"] [uri "/AgentUpdate/server.htc H"] [unique_id "1649930610"] [ref "v1667547,1"] [hostname "10.172.1.67"] [uri "/AgentUpdate/server.htc H"] [unique_id "1649930610"]
Server log callback is not set -- [client 10.175.81.93] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930611"] [ref "v1335666,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930611"]
Server log callback is not set -- [client 10.176.26.100] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930612"] [ref "v84246,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930612"]
Server log callback is not set -- [client 10.175.74.205] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930612"] [ref "v11325,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930612"]
Server log callback is not set -- [client 10.174.89.99] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930612"] [ref "v55962,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930612"]
Server log callback is not set -- [client 10.172.124.10] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.1.67"] [uri "/AgentUpdate/server.htc H"] [unique_id "1649930612"] [ref "v228,1"] [hostname "10.172.1.67"] [uri "/AgentUpdate/server.htc H"] [unique_id "1649930612"]
Server log callback is not set -- [client 10.174.96.153] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930613"] [ref "v1703122,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930613"]
Server log callback is not set -- [client 10.174.108.202] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930613"] [ref "v13406,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930613"]
Server log callback is not set -- [client 10.176.17.65] ModSecurity: Warning. Matched "Operator `Eq' with parameter `0' against variable `REQBODY_ERROR' (Value: `1' ) [file "basic_rules.conf"] [line "65"] [id "200002"] [rev ""] [msg "Failed to parse request body."] [data "XML parsing error: XML: Failed parsing document."] [severity "2"] [ver ""] [maturity "0"] [accuracy "0"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930613"] [ref "v9296,1"] [hostname "10.172.19.199"] [uri "/htmis/service/clientWebService"] [unique_id "1649930613"]