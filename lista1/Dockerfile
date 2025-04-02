FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    apt-transport-https \
    ca-certificates \
    gnupg \
    software-properties-common \
    wget \
    build-essential \
    git && \
    wget -qO - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ jammy main' && \
    apt-get update && \
    apt-get install -y cmake && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Kopiowanie plików projektu
COPY . /app

# Skopiowanie folderu data do katalogu nadrzędnego
RUN cp -r /app/data /data

# Budowanie projektu przy wymuszeniu standardu C++20
RUN cmake -DCMAKE_CXX_STANDARD=20 . && make

CMD ["./lista1"]
