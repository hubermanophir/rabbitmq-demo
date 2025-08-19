# Dockerfile for the consumer
FROM ubuntu:22.04

# Install dependencies (same as producer)
RUN apt-get update && \
    apt-get install -y build-essential cmake git librabbitmq-dev && \
    rm -rf /var/lib/apt/lists/*

# Copy source code
COPY consumer.c /app/consumer.c
WORKDIR /app

# Compile the C program
RUN gcc -o consumer consumer.c -lrabbitmq

# Run the program
CMD ["./consumer"]
