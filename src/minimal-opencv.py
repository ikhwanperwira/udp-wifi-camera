from cv2 import imshow, imdecode, waitKey, destroyAllWindows, resize, INTER_LINEAR
from numpy import frombuffer, uint8
from io import BytesIO
from socket import socket, AF_INET, SOCK_DGRAM

SERV_IPV4, SERV_PORT = ('192.168.43.150', 8888)
udpSock = socket(AF_INET, SOCK_DGRAM)
udpSock.bind((SERV_IPV4,SERV_PORT))
isWriting = False
while 1:
  dataRecv, CLNT_ADDR = udpSock.recvfrom(65536)
    
  if not isWriting:
    if dataRecv[:2] == b'\xff\xd8': # Start of JPEG
      isWriting = True
      buf = BytesIO()

  if isWriting:
    buf.write(dataRecv)
    if dataRecv[-2:] == b'\xff\xd9': # End of JPEG
      isWriting = False
      buf.seek(0)
      try: imshow('foo',resize( imdecode(frombuffer(buf.read(),uint8), 1),(640,480), INTER_LINEAR))
      except: pass

  if waitKey(1) == ord('q'):
    break

destroyAllWindows()