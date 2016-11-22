# http_framework
This is client framework that can be request to http server and response to get the data. Besides that the data can be decoded to xml or json format.

g++ -I./include src/CS_REQHND.cpp src/HttpRequest.cpp -fPIC -shared -o libhttp.so
(g++ -I/usr/local/include/ -I./include -L/usr/local/lib/ -lcurl src/CS_REQHND.cpp src/HttpRequest.cpp -fPIC -shared -o libhttp.so)