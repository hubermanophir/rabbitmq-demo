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

  amqp_channel_open(conn, 1);
  amqp_get_rpc_reply(conn);

  // Declare queue
  amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue_name), 0, 0, 0, 0,
                     amqp_empty_table);
  amqp_get_rpc_reply(conn);

  // Send message
  const char *message = "Hello from C Docker app!";
  amqp_bytes_t message_bytes = amqp_cstring_bytes(message);

  // connection
  // channel: 1
  // exchange: "" (default)
  // routing key: "hello"
  // mandatory: 0 (do not return unroutable messages)
  // immediate: 0 (do not return if not deliverable immediately)
  // properties: NULL (no special properties)
  // body: message_bytes
  amqp_basic_publish(conn, 1, amqp_cstring_bytes(""),
                     amqp_cstring_bytes(queue_name), 0, 0, NULL, message_bytes);

  printf("Message sent: %s\n", message);

  // Close
  amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
  amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
  amqp_destroy_connection(conn);

  return 0;
}
