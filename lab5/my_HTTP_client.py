import http.client
import sys


conn = http.client.HTTPConnection("127.0.0.1")

while True:
    cmd = input('input command (exaples: GET example.txt; POST example.txt 123: ')
    cmd = cmd.split()

    headers = {"Content-type": "application/json", "Accept": "text/plain"}

    # request command to server
    if cmd[0] == 'exit': 
        break

    elif cmd[0] == "GET":
        conn.request(cmd[0], cmd[1])

    elif cmd[0] == "POST":
        conn.request(cmd[0], cmd[1], cmd[2], headers)


    rsp = conn.getresponse()

    # print server response and data
    print('HTTP 1.0' if rsp.version==10 else 'HTTP 1.1',
         rsp.status, 
         rsp.reason
         )
    print(rsp)
    data_received = str(rsp.read()).replace("b'", "")
    print(data_received)

conn.close()