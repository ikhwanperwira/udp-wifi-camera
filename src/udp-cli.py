from socket import socket, AF_INET, SOCK_DGRAM
from threading import Thread

SERV_IPV4, SERV_PORT = ('192.168.43.150', 39876)
udpSock = socket(AF_INET, SOCK_DGRAM)
udpSock.bind((SERV_IPV4,SERV_PORT))

dataRecv = ""
CLNT_ADDR = None

def receiveData():
  global CLNT_ADDR
  global dataRecv
  while 1:
    dataRecv, CLNT_ADDR = udpSock.recvfrom(1357)
    print(dataRecv.decode(), flush=True, end='')

def sendData():
  global CLNT_ADDR
  while 1:
    dataSend = input() + '\n'
    udpSock.sendto(dataSend.encode(),CLNT_ADDR)

if __name__ == '__main__':
  recvThread = Thread(target=receiveData)
  sendThread = Thread(target=sendData)
  recvThread.start()
  sendThread.start()
  recvThread.join(0)
  sendThread.join(0)
