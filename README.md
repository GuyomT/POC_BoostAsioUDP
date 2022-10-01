# POC_BoostAsioUDP
POC for R-Type project of a simple UDP server/client using boost asio

```bash
g++ udpClient.cpp -o udpClient -lboost_system -lpthread
g++ udpServer.cpp -o udpServer -lboost_system -lpthread

./udpServer
./udpClient localhost
nc -4u -w1 localhost 8000
```
