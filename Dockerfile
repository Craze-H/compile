FROM gcc:10
WORKDIR /app/
COPY test.c getword.h ./
RUN gcc test.c -o main
RUN chmod +x main
