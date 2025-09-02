# RabbitMQ + Docker
This demo is showing how to run a simple example of a producer and consumer using RabbitMQ

## Setup
clone repo
```bash
  git clone https://github.com/hubermanophir/rabbitmq-demo.git
```
Make sure you have docker installed on your machine
Easiest way to download docker desktop:

https://www.docker.com/products/docker-desktop/


After you cloned the project

```bash
cd consumer
```
and run:
```bash
docker build -t consumer .
```

after that go to 

```bash
cd producer
```

and build it:
```bash
docker build -t producer .
```

Great! now we have the images for both the producer and consumer ready to run



## Running The Demo
First run the command:
To create a bridge network connection between the docker images

```bash
docker network create rabbit-net
```

Build and run rabbitmq dashboard
```bash
docker run -d --rm --network rabbit-net --name rabbitmq -p 15672:15672 -p 5672:5672 -p 3000:3000 rabbitmq:3-management
```

Build and run wireshark gui
```bash
docker run -d --rm --name wireshark-gui --network container:rabbitmq --cap-add=NET_ADMIN --cap-add=NET_RAW lscr.io/linuxserver/wireshark:latest
```


Now you can play with it to see different things:

To run the consumer:
```bash
docker run --rm --network rabbit-net --name consumer consumer
```

To run the producer:
```bash
docker run --rm --network rabbit-net --name producer producer
```

Running the producer will send a message to the queue and then finish, The consumer will keep running and listening to the queue

You can go to `localhost:15672` for the rabbitmq dashboard
login using username and passwword guest


Go to localhost:3000 for the wireshark GUI

Go to eth0

In the wireshark we want to see only requests on port 5672
In the filter type

```bash
tcp.port == 5672
```

And you will be able to see all the tcp/amqp requests

## Cleanup
```bash
docker stop consumer rabbitmq wireshark-gui
```

```bash
docker network rm rabbit-net
```
