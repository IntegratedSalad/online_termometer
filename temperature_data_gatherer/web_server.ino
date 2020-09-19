void handle_root()
{
  server.send(200, "text/plain", ":DD XDD");
}

void handle_not_found()
{
  server.send(404, "text/plain", "404: Not Found");
}
