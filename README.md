
# Basic HTTP client

A basic http client that parses a URL, sends GET and HEAD requests and parses and saves the response.

Used as library in my [HTTPS version](https://github.com/blr-ophon/min_httpsclient).

Programmed entirely in C. Uses linux sockets API for TCP connection with the server.

## Building

Clone the project
```bash
  git clone https://github.com/blr-ophon/min_httpclient
```
Compile using:

```bash
  cd min_httpclient
  make
```

## Usage

```bash
  ./minhttp <url>
```
Then specify the request method.

When the server responds with a file, it is saved in the responses folder. 



