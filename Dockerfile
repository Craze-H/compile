FROM gcc:10.2
WORKDIR /app/
COPY test.cpp getword.h ./
RUN g++ test.cpp -o test
RUN chmod +x test
