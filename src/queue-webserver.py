def frame_collector(frame):
  from socket import socket, AF_INET, SOCK_DGRAM
  from io import BytesIO

  udpSock = socket(AF_INET, SOCK_DGRAM)
  udpSock.bind(('192.168.145.200',39876))
  isWriting = False
  while 1:
    dataRecv, CLNT_ADDR = udpSock.recvfrom(2048)

    if not isWriting:
      if dataRecv[:2] == b'\xff\xd8':
        isWriting = True
        buf = BytesIO()

    if isWriting:
      buf.write(dataRecv)
      if dataRecv[-2:] == b'\xff\xd9':
        isWriting = False
        frame.put(buf.getvalue())

def flask_service(frame):
  from flask import Flask, Response
  from time import time, sleep, time_ns
  from sys import float_info

  app = Flask(__name__)

  def frame_consumer():
    while 1:
      t0 = time()
      yield b'--frame\r\nContent-Type: image/jpeg\r\n\r\n' + frame.get() + b'\r\n'
      try: print(1/(time() - t0))
      except: pass

  @app.route('/mjpeg')
  def mjpeg():
    return Response(frame_consumer(),mimetype='multipart/x-mixed-replace; boundary=frame')
  
  @app.route('/')
  def index():
    return """
    """

  app.run(host='192.168.145.200', threaded=True)

if __name__ == '__main__':
  from multiprocessing import Process, Queue
  from time import sleep

  frame = Queue()

  p1 = Process(target=frame_collector, args=(frame,))
  p2 = Process(target=flask_service, args=(frame,))

  p1.start()
  p2.start()

  while 1:
    try: sleep(5)
    except: break
  
  p1.terminate()
  p2.terminate()
  print('All process terminated!')