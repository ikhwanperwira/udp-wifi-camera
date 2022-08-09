import socket, threading
SERV_IPV4, SERV_PORT = ("192.168.145.200", 39876)
udpSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udpSock.bind((SERV_IPV4,SERV_PORT))

dataRecv = ""
CLNT_ADDR = None

def receiveData():
  global CLNT_ADDR
  global dataRecv
  while 1:
    dataRecv, CLNT_ADDR = udpSock.recvfrom(1357)
    print(dataRecv, CLNT_ADDR)

recvThread = threading.Thread(target=receiveData)
recvThread.start()
recvThread.join(0)

while 1:
  dataSend = (input() + "\n").encode()
  print(dataSend)
  udpSock.sendto(dataSend,CLNT_ADDR)