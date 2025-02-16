FROM gcc:11.3.0
COPY . /shmem-test
WORKDIR /shmem-test
RUN make

CMD ["sh", "-c", "./prog1 | ./prog2"]
