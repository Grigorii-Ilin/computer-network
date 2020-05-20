from http.server import BaseHTTPRequestHandler, HTTPServer


class MyHTTPRequestHandler(BaseHTTPRequestHandler):
    file_location = 'resourse.txt'  # file location

    f = open(file_location, 'r')  # open requested file
    file_content = f.read()

    def set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html;charset=utf-8')
        self.send_header('Content-version', '1.1')
        self.send_header('Host','ru.mysite.org')
        self.end_headers()

    # handle GET command
    def do_GET(self):
        try:
            if self.path.endswith('.txt'):
                self.set_response()

                self.wfile.write(bytes('\n'+self.file_content, "utf8"))
                return

        except IOError:
            self.send_error(404, 'file not found')

    def do_POST(self):
        try:
            content_length = int(self.headers['Content-Length'])  # Gets the size of data
            post_data = self.rfile.read(content_length)  # Gets the data itself

            self.file_content += post_data.decode('utf-8')

            self.set_response()
            self.wfile.write(bytes('\n'+self.file_content, "utf8"))
            return

        except IOError:
            self.send_error(404, 'file not found')




print('http server is starting...')

# ip and port of server
server_address = ('127.0.0.1', 80)
h = HTTPServer(server_address, MyHTTPRequestHandler)
print('http server is running...')
h.serve_forever()

