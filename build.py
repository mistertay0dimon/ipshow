import platform
import os

def main():
    system = platform.system()

    if system == "Windows":
        os.system("gcc src/main.c -o bin/ipshow.exe -liphlpapi -lws2_32")
    else:
        os.system("gcc src/main.c -o bin/ipshow -lm")

if __name__ == "__main__":
    main()