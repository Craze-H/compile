FROM gcc:10.2
WORKDIR /app/
COPY test.cpp ./
RUN g++ test.cpp -o test
RUN chmod +x test
