#!/usr/bin/env python3

import cgi
import cgitb
import os

# Enable debugging
cgitb.enable()

print("Content-Type: text/html")    # HTML is following
print()                             # blank line, end of headers

# Parse form data
form = cgi.FieldStorage()

# Retrieve form fields
name = form.getvalue('name')
email = form.getvalue('email')

# Output the form data
print("<html>")
print("<head>")
print("<title>CGI Form Data</title>")
print("</head>")
print("<body>")
print("<h1>Form Data Received</h1>")
print(f"<p>Name: {name}</p>")
print(f"<p>Email: {email}</p>")
print("</body>")
print("</html>")