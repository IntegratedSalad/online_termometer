
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packet_buffer[NTP_PACKET_SIZE]; // buffer to hold incoming & outgoing packets

time_t get_ntp_time()
{
  IPAddress ntp_server_ip;

  while (udp.parsePacket() > 0);
  WiFi.hostByName(ntp_server_name, ntp_server_ip);
  send_ntp_packet(ntp_server_ip);
  uint32_t begin_wait = millis();

  while (millis() - begin_wait < 1500)
  {
    int size = udp.parsePacket();

    if (size >= NTP_PACKET_SIZE)
    {
      udp.read(packet_buffer, NTP_PACKET_SIZE); // read packet into the buffer
      unsigned long seconds_since_1900;

      // convert four bytes starting at location 40 to a long integer
      
      seconds_since_1900  = (unsigned long)packet_buffer[40] << 24;
      seconds_since_1900 |= (unsigned long)packet_buffer[41] << 16;
      seconds_since_1900 |= (unsigned long)packet_buffer[42] << 8;
      seconds_since_1900 |= (unsigned long)packet_buffer[43];
      return seconds_since_1900 - 2208988800UL + time_zone * SECS_PER_HOUR;
    }
  }

  Serial.println("No NTP Response.");
  return 0;
  
}

// send an NTP request to the time server at the given address
void send_ntp_packet(IPAddress &address)
{
  memset(packet_buffer, 0, NTP_PACKET_SIZE);

  // Initialize values needed to form NTP request

  packet_buffer[0] = 0b11100011; // LI, Version, Mode
  packet_buffer[1] = 0;
  packet_buffer[2] = 6;
  packet_buffer[3] = 0xEC;

  packet_buffer[12] = 49;
  packet_buffer[13] = 0x4E;
  packet_buffer[14] = 49;
  packet_buffer[15] = 52;

  // Sending a packet requesting a timestamp:

  udp.beginPacket(address, 123);
  udp.write(packet_buffer, NTP_PACKET_SIZE);
  udp.endPacket();
  
}
