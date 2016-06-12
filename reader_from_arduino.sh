#!/bin/bash

exec 3<>/dev/tcp/192.168.1.12/80;echo -e "GET /objects/?object=tempSensor01&op=m&m=tempChanged&t=$1 HTTP/1.0\r\nConnection: close\r\nHost: 192.168.1.12\r\n\r\n">&3;cat<&3
exec 3<>/dev/tcp/192.168.1.12/80;echo -e "GET /objects/?object=humSensor01&op=m&m=humChanged&h=$2 HTTP/1.0\r\nConnection: close\r\nHost: 192.168.1.12\r\n\r\n">&3;cat<&3
