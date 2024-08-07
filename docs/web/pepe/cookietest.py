import os

def main():
    print("Received cookies:")
    for key, value in os.environ.items():
        if key.startswith("HTTP_COOKIE"):
            cookie_name = key[len("HTTP_COOKIE"):]
            print(f"{cookie_name}: {value}")

if __name__ == "__main__":
    main()