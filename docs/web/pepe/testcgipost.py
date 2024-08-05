#!/usr/bin/env python3

import os
import sys
import urllib.parse

print("Content-Type: text/html")
print()
print("<html><body>")
print("<h1>CGI Script Output</h1>")
print("<h2>Environment Variables</h2>")
for key, value in sorted(os.environ.items()):
    print(f"{key}: {value}<br>")

print("<h2>Request Body</h2>")
try:
    content_length = int(os.environ.get('HTTP_CONTENT_LENGTH', 0))
    input_data = sys.stdin.read(content_length)
    print(f"<pre>{input_data}</pre>")
except Exception as e:
    print(f"Error reading input: {e}")

print("<h2>Processed Form Data</h2>")
try:
    form_data = urllib.parse.parse_qs(input_data)
    name = form_data.get("name", ["None"])[0]
    email = form_data.get("email", ["None"])[0]
    print(f"Name: {name}<br>")
    print(f"Email: {email}<br>")
except Exception as e:
    print(f"Error processing form data: {e}")

print("</body></html>")
#fp=sys.stdin, environ=os.environ, keep_blank_values=True