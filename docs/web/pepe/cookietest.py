import os
import http.cookies

def main():
    # Get cookies from environment variables
    cookies = http.cookies.SimpleCookie()
    for key, value in os.environ.items():
        if key.startswith("HTTP_COOKIE_"):
            cookie_name = key[len("HTTP_COOKIE_"):]
            cookies[cookie_name] = value

    # Print out the cookies
    print("Received cookies:")
    for key, morsel in cookies.items():
        print(f"{key}: {morsel.value}")

    # Construct response body
    body = "Check the console for printed cookies."

    # Construct response headers
    headers = [
        "HTTP/1.1 200 OK",
        "Content-Type: text/html",
        f"Content-Length: {len(body)}"
    ]

    # Add Set-Cookie headers
    for key, morsel in cookies.items():
        headers.append(f"Set-Cookie: {key}={morsel.value}")

    # Add a blank line to separate headers from the body
    headers.append("")
    headers.append(body)

    # Join headers and body to form the response
    response = "\r\n".join(headers)
    print(response)

if __name__ == "__main__":
    main()
