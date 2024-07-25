import http.client

connection = http.client.HTTPConnection('127.100.100.2:4343')
connection.putrequest('POST', '/endpoint')
connection.putheader('Transfer-Encoding', 'chunked')
connection.endheaders()

# Manually chunked data
chunks = [
    'This is the first chunk',
    'And this is the second one',
    'Finally, this is the last chunk'
]

for chunk in chunks:
    # Send each chunk size in hexadecimal + CRLF + chunk data + CRLF
    connection.send(hex(len(chunk))[2:].encode() + b'\r\n')
    connection.send(chunk.encode() + b'\r\n')

# Send zero-length chunk to indicate the end
connection.send(b'0\r\n\r\n')

response = connection.getresponse()
print(response.status, response.reason)
print(response.read().decode())

connection.close()