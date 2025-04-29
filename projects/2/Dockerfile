FROM gcc:latest

WORKDIR /app

COPY hello.cpp .

RUN g++ -o hello hello.cpp

ENTRYPOINT ["./hello"]

