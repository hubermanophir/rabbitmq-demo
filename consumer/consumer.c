#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  const char *hostname = "rabbitmq"; // Docker container name
  int port = 5672;
  const char *queue_name = "hello";

  amqp_connection_state_t conn = amqp_new_connection();
  amqp_socket_t *socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    fprintf(stderr, "Could not create TCP socket\n");
    return 1;
  }

  // Retry loop until RabbitMQ is ready
  int retries = 10;
  while (amqp_socket_open(socket, hostname, port) && retries--) {
    fprintf(stderr, "RabbitMQ not ready, retrying in 2s...\n");
    sleep(2);
  }

  if (retries <= 0) {
    fprintf(stderr, "Could not open TCP socket to RabbitMQ\n");
    return 1;
  }

  // Login
  if (amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest",
                 "guest")
          .reply_type != AMQP_RESPONSE_NORMAL) {
    fprintf(stderr, "Failed to login\n");
    return 1;
  }

  // Open channel AND check for errors
  amqp_channel_open(conn, 1);
  if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
    fprintf(stderr, "Error: Failed to open channel\n");
    return 1;
  }

  // Declare queue AND check for errors
  amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue_name), 0, 0, 0, 0,
                     amqp_empty_table);
  if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
    fprintf(stderr, "Error: Failed to declare queue\n");
    return 1;
  }

  // Start consuming messages AND check for errors
  amqp_basic_consume(conn, 1, amqp_cstring_bytes(queue_name), amqp_empty_bytes,
                     0, 1, 0, amqp_empty_table);
  if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
    fprintf(stderr, "Error: Failed to start consuming\n");
    return 1;
  }

  printf("Waiting for messages...\n");
  fflush(stdout); // Flush after the initial message

  // Loop forever, waiting for messages
  while (1) {
    amqp_envelope_t envelope;
    amqp_consume_message(conn, &envelope, NULL, 0);
    printf("Received message: %.*s\n", (int)envelope.message.body.len,
           (char *)envelope.message.body.bytes);
    fflush(stdout); // <-- ADDED THIS LINE to force output to the logs
    amqp_destroy_envelope(&envelope);
  }

  return 0; // This part is never reached in this simple example
}
