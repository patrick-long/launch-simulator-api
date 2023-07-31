<h1 align="center">Launch Simulator API</h1>

A Web API for the Launch Simulator project created using C++

## Getting Started

Getting started is quite simple for Windows users. Additional compatibility for Linux and UNIX will be implemented in the future in a Dockerized setting.

### Clone the repo

```sh
git clone https://github.com/patrick-long/launch-simulator-api.git
```

---

### Building and Running on Windows

1. Run the server.sh script

```sh
./server.sh
```

---

### Building and Running Docker container

1. Make sure that Docker is fully installed on your system and running
2. Navigate to the project folder `launch-simulaor-api/` and build the Docker container:

```sh
docker-compose build
```

3. After the container has successfully built, run the container:

```sh
docker-compose up
```

4. Whenever you are ready to exit the container, in another terminal window:

```sh
docker-compose down
```
