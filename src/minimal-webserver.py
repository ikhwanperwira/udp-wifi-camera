from multiprocessing import Pipe

def frame_collector(clients):
  import io
  import socket
  SERV_IPV4, SERV_PORT = ('192.168.145.200', 39876)
  udpSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  udpSock.bind((SERV_IPV4,SERV_PORT))
  isWriting = False
  LOCK_CLNT_ADDR = None
  # LOCK_CLNT_ADDR = ('192.168.145.105',9999)
  try:
    while 1:
      dataRecv, CLNT_ADDR = udpSock.recvfrom(2048)

      if LOCK_CLNT_ADDR == CLNT_ADDR:
        if not isWriting:
          if dataRecv[:2] == b'\xff\xd8': # Start of JPEG
            isWriting = True
            buf = io.BytesIO()

        if isWriting:
          buf.write(dataRecv)
          if dataRecv[-2:] == b'\xff\xd9': # End of JPEG
            isWriting = False
            buf.seek(0)
            frame = buf.read()
            d = dict(clients) # Make copy into local dict
            for handle, sender in d.items():
              try:
                sender.send(frame)
              except: # Client has gone away?
                del clients[handle]
      else:
        if dataRecv == b'SYN\n':
          udpSock.sendto(b'ACK\n',CLNT_ADDR)
          udpSock.sendto(b'$cam init\n',CLNT_ADDR)
          udpSock.sendto(b'$cam stream\n',CLNT_ADDR)
          LOCK_CLNT_ADDR = CLNT_ADDR
  except KeyboardInterrupt: # ctrl-C
    pass

def flask_service(clients): # process 2
  from flask import Flask, Response
  from time import time
  app = Flask(__name__)

  def frame_consumer():
    receiver, sender = Pipe(False)
    clients[sender._handle] = sender
    while True:
      t0 = time()
      yield b'--frame\r\nContent-Type: image/jpeg\r\n\r\n' + receiver.recv() + b'\r\n'
      print(1/ (time() - t0))

  @app.route('/mjpeg')
  def mjpeg():
    return Response(frame_consumer(),mimetype='multipart/x-mixed-replace; boundary=frame')

  @app.route('/')
  def index():
    return """
    """

  app.run(host='192.168.145.200', threaded=True)

if __name__ == '__main__':
  from multiprocessing import Process, Manager

  with Manager() as manager:
     clients = manager.dict()

     p1 = Process(target=frame_collector, args=(clients,))
     p2 = Process(target=flask_service, args=(clients,))
     p1.start()
     p2.start()

     try:
       p2.join() # block while flask_service is running
     except: # ctrl-C
       p1.terminate()
       p2.terminate()
     finally:
       print('Ending up.')