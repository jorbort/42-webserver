#!/usr/bin/env python3
import cgi

print("Content-Type: text/html\n")
print("<html><head><title>CGI Example</title></head>")
print("<body>")
print("<h1>CGI Script Output</h1>")

form = cgi.FieldStorage()
if form:
    for key in form.keys():
        print(f"<p>{key}: {form.getvalue(key)}</p>")
else:
    print("<p>No parameters were passed.</p>")

print("</body></html>")
