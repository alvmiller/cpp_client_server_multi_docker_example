#pull ubuntu
FROM ubuntu:latest AS build

#pull gcc
#FROM gcc:latest

#just in case,  you can install gcc and cmake
RUN apt-get update && apt-get -y install build-essential gcc cmake

WORKDIR /src

EXPOSE 7777

# add (copy) all from local folder to /usr/src
#ADD . /usr/src
COPY medium_server.cpp ./

RUN mkdir -p bins/
RUN g++ medium_server.cpp -static -o bins/medium_server

FROM ubuntu:latest

WORKDIR /opt/cs

# Copy the binary from the build stage
COPY --from=build /src/bins/medium_server ./

#CMD ["./medium_server"] 
#CMD ["/bin/bash"]
ENTRYPOINT [ "./medium_server" ]
