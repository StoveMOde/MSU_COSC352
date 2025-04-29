FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y g++ curl libcurl4-openssl-dev

WORKDIR /app

COPY extract_tables.cpp .

RUN g++ extract_tables.cpp -o extract_tables -lcurl

CMD ["./extract_tables"]
