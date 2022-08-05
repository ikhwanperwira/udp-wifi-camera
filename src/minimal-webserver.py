def frame_collector(frame): # process 1
  from io import BytesIO
  from socket import socket, AF_INET, SOCK_DGRAM
  SERV_IPV4, SERV_PORT = ('192.168.43.150', 39876)
  udpSock = socket(AF_INET, SOCK_DGRAM)
  udpSock.bind((SERV_IPV4,SERV_PORT))
  isWriting = False
  while 1:
    dataRecv, CLNT_ADDR = udpSock.recvfrom(65507)
      
    if not isWriting:
      if dataRecv[:2] == b'\xff\xd8': # Start of JPEG
        isWriting = True
        buf = BytesIO()

    if isWriting:
      buf.write(dataRecv)
      if dataRecv[-2:] == b'\xff\xd9': # End of JPEG
        isWriting = False
        buf.seek(0)
        frame.put(buf.read())
        # open('test/pics/'+str(time())+'.jpg','wb').write(buf.read())

def flask_service(frame): # process 2
  from flask import Flask, Response
  from time import sleep
  app = Flask(__name__)

  def frame_consumer():
    while 1:
      yield b'--frame\r\nContent-Type: image/jpeg\r\n\r\n' + frame.get() + b'\r\n'
      sleep(1/60)

  @app.route('/mjpeg')
  def mjpeg():
    return Response(frame_consumer(), mimetype='multipart/x-mixed-replace; boundary=frame')

  # @app.route('/')
  # def index():
  #   return """
  #   <body style="background: blue;">
  #     <div style="width: 800px; margin: 0px auto;">
  #       <img src="/mjpeg">
  #     </div>
  #   </body>
  #   """

  app.run(host='192.168.43.150', threaded=True)

if __name__ == '__main__':
  from multiprocessing import Process, Queue
  from time import sleep

  frame = Queue()

  p1 = Process(target=frame_collector, args=(frame,))
  p2 = Process(target=flask_service, args=(frame,))

  p1.start()
  p2.start()
  print('All processes started!')
  while 1:
    try:
      sleep(5)
    except:
      break
  p1.terminate()
  p2.terminate()
  print('All processes terminated!')
