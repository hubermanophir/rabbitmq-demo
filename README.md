Run commands:
docker network create rabbit-net

Build & Run Rabbitmq:
docker run -d --rm --network rabbit-net --name rabbitmq -p 15672:15672 rabbitmq:3-management

Can access the management in localhost:15672

under consumer folder:
docker build -t c-consumer .

under producer folder:
docker build -t c-consumer .

Run consumer:
docker run  --rm --network rabbit-net --name consumer c-consumer

Run producer:
docker run --rm --network rabbit-net --name producer c-producer


Cleanup:
docker stop consumer rabbitmq 
docker network rm rabbit-net