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
COPY medium_client.cpp ./

RUN mkdir -p bins/
RUN g++ medium_client.cpp -static -o bins/medium_client

FROM ubuntu:latest

WORKDIR /opt/cs

# Copy the binary from the build stage
COPY --from=build /src/bins/medium_client ./

#CMD ["./medium_client"] 
#CMD ["/bin/bash"]
ENTRYPOINT [ "./medium_client" ]
