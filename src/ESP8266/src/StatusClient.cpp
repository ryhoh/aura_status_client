#include "StatusClient.h"

StatusClientOption postStatusToBoard(String name)
{
  const char *host = "aurastatusboard.herokuapp.com";
  const char *endpoint = "/api/heartbeat";
  /* "api_password_here" */
  const char *api_password = "vhAiHAHKYfGslYiNFXKxzxllrL08SQOu0KqbdWeBUVti6HOJ5KetsUNcRMIw4SdM";
  
  static time_t last_time = INT16_MIN;
  const time_t current_time = time(NULL);
  if (current_time < INT16_MIN)  // Using local time
    return StatusClientOption {
      "NTP not synchronized",
      StatusClientResult::FAILURE_NTP_NOT_SYNC
    };
  
  const struct tm *current_tm = localtime(&current_time);
  // Suppress from 0 AM to 6 AM.
  if (current_tm->tm_hour < 6 || current_time - last_time < INTERVAL_S)
    return StatusClientOption {
      String("Skipped (next is ") + String(INTERVAL_S - current_time + last_time) + "s later)",
      StatusClientResult::SKIP
    };
  last_time = current_time;

  WiFiClientSecure client;
  client.setInsecure();
  // const char *fingerprint = "82:6F:E8:01:13:78:A9:2A:76:2B:B6:9F:2F:8C:EB:11:D2:4E:2E:4D:9F:66:FA:71:E8:C7:88:55:FB:C2:BB:AB";
  // client.allowSelfSignedCerts();
  // client.setFingerprint(fingerprint);

  if (!client.connect(String("https://") + host, 443))
    return StatusClientOption {
      "Connection for StatusBoard cannot be established",
      StatusClientResult::FAILURE_CONNECTION
    };
  
  const String payload = 
    String("name=") + name + "&password=" + api_password;
  
  client.print(
    String("POST https://") + host + endpoint + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "User-Agent: Aura Machine-Butterfly (ESP8266)\r\n" +
    "Connection: close\r\n" +
    "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n" +
    "Content-Length: " + payload.length() + "\r\n" +
    "\r\n" +
    payload + "\r\n"
  );
  client.flush();

  // receive and skip header
  const String buf = client.readString();
  const size_t msg_start = buf.indexOf("\r\n\r\n") + 4;

  return StatusClientOption {
    buf.substring(msg_start),
    StatusClientResult::SUCCESS
  };
}
