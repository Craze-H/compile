FROM gcc:10
WORKDIR /app/
COPY test.c ./
RUN gcc test.c -o main
RUN chmod +x main
