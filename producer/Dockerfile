# Dockerfile
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && \
    apt-get install -y build-essential cmake git librabbitmq-dev curl && \
    rm -rf /var/lib/apt/lists/*

# Copy source code
COPY producer.c /app/producer.c
WORKDIR /app

# Compile the C program
RUN gcc -o producer producer.c -lrabbitmq

# Run the program
CMD ["./producer"]
