/* Functions regarding server behavior and handling data send to server */

String get_content_type(String filename)
{
  if      (filename.endsWith(".html"))   return "text/html";
  else if (filename.endsWith(".css"))    return "text/css";
  else if (filename.endsWith(".js"))     return "application/javascript";
  else if (filename.endsWith(".ico"))    return "image/x-icon";
  else if (filename.endsWith(".json"))   return "application/json";
  else if (filename.endsWith(".png"))    return "image/png";
  else if (filename.endsWith(".gz"))     return "application/x-gzip";
  return "text/plain";
}

bool handle_file_read(String path)
{
  Serial.println("handle_file_read: " + path);
  if (path.endsWith("/")) path += "index.html";
  
  String content_type = get_content_type(path);
  String path_with_gz = path + ".gz";

  if (LittleFS.exists(path) || LittleFS.exists(path_with_gz))
  {
    if (LittleFS.exists(path_with_gz))
      path += ".gz";
 
    File f = LittleFS.open(path, "r");
    size_t sent = server.streamFile(f, content_type);
    f.close();
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false;

}
