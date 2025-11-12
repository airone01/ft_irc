# ft_irc

> An implementation of the IRC protocol in C++98, following RFC 1459.

## Recommended client

- HexChat (GUI)
- irssi (TUI)

## Implementation details

### EventHandler

This has a header only and no implementation because it's meant to be inherited from. It's a minimal abstract interface for objects that want to receive epoll events. Any object registered with Reactor (the registerer, see below) must implement this interface.
It has a `virtual void handleEvent(uint32_t events) = 0;` method, in which events is the epoll event mask (`EPOLLIN`, `EPOLLOUT`, `EPOLLERR`, etc).
To use it, make a inherit it and implement handleEvent.
Then pass the object pointer to `Reactor::addFd`, meaning `Reactor` will call this `handleEvent` implementation when the associated fd is ready.

### Socket

This is a [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) wrapper around the POSIX socket file descriptor. It can perform create/bind/listen/accept (and it has basic helpers). Use it to prepare the server socket or to manage raw fds.
Call `Socket::createAndBind(addr, port)` to create a TCP socket and bind it to the given address and port.
Then call `Socket::listen(backlog)` to listen on the socket (with backlog set).
Then call `Socket::accept()` to accept a single connection.
You also have `setNonBlocking`, and more methods which are self-explanatory.

### Listener

This is basically the high-level "server". It's a wrapper that binds a listening socket and reacts to `EPOLLIN` to accept new clients (which is the whole point of ft_irc imo), the registers them with the Reactor and adds them to a `ConnectionManager` instance.
To use it, create it. It will then stores the address, port, `Reactor` and `ConnectionManager`.
Then use `start()` which will do all the binding, marking as non-blocking,
registering fds, etc...
Then `handleEvent` will loop on `accept()` pretty much forever or until it receives a `EAGAIN`, configure the accepted socket to be non-blocking once again, register it with ConnectionManager and add it to the `Reactor`.
Do note that in the current implementation (git blame me), the connection message callback is unset.

### Connection

This is meant to represent a client connection. It has the client fd and the
read/write buffers and it has methods for reading/writing.

Details you need to know here:
- It inherits `EventHandler` so that `Reactor` will call `Connection::handleEvent` when the peer's fd is ready.
- `handleEvent` will check for fatal flags to make sure it can continue. Stuff like `EPOLLHUP`, `EPOLERR`, etc. and call `handleRead()` or `handleWrite()` for `EPOLLIN/EPOLLOUT` respectively.
- `handleRead` will read until `EAGAIN` and append that to m_readBuf. If a `MessageCallback` is set, the callback is invoked with the acculumated bytes.
- `send` will append to the write buffer. But if it was empty before the call to send, `Connection` will modify epoll interest to include `EPOLLOUT` so `Reactor` will notify when the socket becomes writable.
- `handleWrite` will try to write as much of the write buffer as possible. When empty, it removes the `EPOLLOUT` interest (we don't need it anymore).
- `close` will deregister the fd from epoll, remove the connection from the
  manager and close the fd.

Basically in usage:

> When we will have a dispatcher, it will set a `MessageCallback` to receive
> bytes and will produce responses by calling send on the `Connection`.
> The `Connection` will manage non-blocking I/O and epoll interactions (that's
> what it was made for), and the callback will handle application protocol
> logic (IRC protocol parsing for us).

### Reactor

This encapsulates `epoll(7)` and provides a simple event loop that dispatches events to `Eventhandler` instances associated will file descriptors.
Use it by creating an instance in the main thread/function, register listening socket(s) (the "server") and client `Connection`(s) with `addFd`. When `run()` is called, `Reactor` will call handleEvent on the corresponding handlers.
The is also an optional `eventfd` to wake `epoll_wait` when `stop()` is called.

### ConnectionManager

`ConnectionManager` tracks active `Connection` object (sorted by fd?) so that higher-level code can list, sweep, or close connections as required (i.e. on server shutdown).
Methods are pretty straightforward (`add`, `remove`, `closeAll`, `sweepIdle`, `size`).

## Standard usage of the "lib"

This concerns files in the `src/net/` directory.

- Create a `Reactor` instance.
- Create a `ConnectionManager` (and `TimerManager` if you need timeouts - we do).
- Create a listening socket (w/ `Listener`).
- For each accepted client:
  - create `Connection(clientFd, &reactor, &connMgr)`
  - set the connection's `MessageCallback` (dispatcher) that handles protocol and calls `Connection::send()` to queue responses
  - register the client fd with `reactor->addFd(clientFd, EPOLLIN | EPOLLRDHUP | ..., connectionPointer)`
  - `add()` the `Connection` to `ConnectionManager`
- Call `reactor.run()` to start the event loop.
- When you need to stop: `reactor.stop()` then close connections and cleanup.

