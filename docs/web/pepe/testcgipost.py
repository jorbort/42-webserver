#!/usr/bin/env python3
import os
import sys
import html

def main():
    # Read the request body
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    request_body = sys.stdin.read(content_length)

    # Debugging output
    print("Content-Type: text/html")
    print()  # End of headers
    print("<html><head><title>CGI Script Output</title></head><body>")
    print("<h1>CGI Script Output</h1>")
    print("<h2>Debugging Output</h2>")
    print("<pre>")
    print(f"CONTENT_LENGTH: {content_length}")
    print(f"Request Body: {request_body}")
    print("</pre>")

    # Parse the request body
    form_data = {}
    if request_body:
        for pair in request_body.split('&'):
            key, value = pair.split('=')
            form_data[key] = html.escape(value)

    # Print the HTML response
    print("<h2>Environment Variables</h2>")
    print("<pre>")
    for key, value in os.environ.items():
        print(f"{key}: {value}")
    print("</pre>")

    # Print request body
    print("<h2>Request Body</h2>")
    print("<pre>")
    for key, value in form_data.items():
        print(f"{key}: {value}")
    print("</pre>")

    print("</body></html>")

if __name__ == "__main__":
    main()