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

Now build & run rabbitmq instance:

```bash
docker run -d --rm --network rabbit-net --name rabbitmq -p 15672:15672 rabbitmq:3-management
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

## Cleanup
```bash
docker stop consumer rabbitmq
docker network rm rabbit-net
```
