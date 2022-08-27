from socket import socket, AF_INET, SOCK_DGRAM
from threading import Thread
from sys import stdin, stdout
SERV_IPV4, SERV_PORT = ("192.168.43.150", 39876)
udpSock = socket(AF_INET, SOCK_DGRAM)
udpSock.bind((SERV_IPV4,SERV_PORT))

dataRecv = ""
CLNT_ADDR = None

def receiveData():
  global CLNT_ADDR
  global dataRecv
  while 1:
    dataRecv, CLNT_ADDR = udpSock.recvfrom(1357)
    stdout.write(dataRecv.decode())

recvThread = Thread(target=receiveData)
recvThread.start()
recvThread.join(0)

while 1:
    dataSend = (stdin.readline()).encode()
    udpSock.sendto(dataSend,CLNT_ADDR)
