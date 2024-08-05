#!/usr/bin/env python3

import os
import cgi
import cgitb

cgitb.enable()  # Enable CGI error reporting

print("Content-Type: text/html")  # HTML is following
print()  # Blank line, end of headers

print("<html><head><title>CGI Script Output</title></head><body>")
print("<h1>CGI Script Output</h1>")

# Print environment variables
print("<h2>Environment Variables</h2>")
print("<pre>")
for key, value in os.environ.items():
    print(f"{key}: {value}")
print("</pre>")

# Print request body
print("<h2>Request Body</h2>")
form = cgi.FieldStorage()
print("<pre>")
for field in form.keys():
    print(f"{field}: {form.getvalue(field)}")
print("</pre>")

print("</body></html>")