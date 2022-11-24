
/*https://gitlab.com/ahmaddidiks/base-firmware/-/tree/esp32*/
static const char *rootCACertificate PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFgDCCBSegAwIBAgIQCAaZPmrE100zBdEGNDJiGTAKBggqhkjOPQQDAjBKMQsw
CQYDVQQGEwJVUzEZMBcGA1UEChMQQ2xvdWRmbGFyZSwgSW5jLjEgMB4GA1UEAxMX
Q2xvdWRmbGFyZSBJbmMgRUNDIENBLTMwHhcNMjIxMTAxMDAwMDAwWhcNMjMwMTMw
MjM1OTU5WjBqMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQG
A1UEBxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQQ2xvdWRmbGFyZSwgSW5jLjET
MBEGA1UEAxMKZ2l0bGFiLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABMUy
km4OwQrXvy5vIRuHTp14EdggqTzX35khszTzMF2CqVfiKoLNYsXDvlS5ajI2eKtQ
NNgeN/SgN+yGfGOA2kWjggPNMIIDyTAfBgNVHSMEGDAWgBSlzjfq67B1DpRniLRF
+tkkEIeWHzAdBgNVHQ4EFgQUwziP9QAvARMhPwZSWtshBiGlh8YwgZQGA1UdEQSB
jDCBiYIUY3VzdG9tZXJzLmdpdGxhYi5jb22CDmthcy5naXRsYWIuY29tghplbWFp
bC5jdXN0b21lcnMuZ2l0bGFiLmNvbYIKZ2l0bGFiLmNvbYIPY2hlZi5naXRsYWIu
Y29tghNyZWdpc3RyeS5naXRsYWIuY29tghNwYWNrYWdlcy5naXRsYWIuY29tMA4G
A1UdDwEB/wQEAwIHgDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwewYD
VR0fBHQwcjA3oDWgM4YxaHR0cDovL2NybDMuZGlnaWNlcnQuY29tL0Nsb3VkZmxh
cmVJbmNFQ0NDQS0zLmNybDA3oDWgM4YxaHR0cDovL2NybDQuZGlnaWNlcnQuY29t
L0Nsb3VkZmxhcmVJbmNFQ0NDQS0zLmNybDA+BgNVHSAENzA1MDMGBmeBDAECAjAp
MCcGCCsGAQUFBwIBFhtodHRwOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwdgYIKwYB
BQUHAQEEajBoMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5jb20w
QAYIKwYBBQUHMAKGNGh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9DbG91ZGZs
YXJlSW5jRUNDQ0EtMy5jcnQwDAYDVR0TAQH/BAIwADCCAXwGCisGAQQB1nkCBAIE
ggFsBIIBaAFmAHYA6D7Q2j71BjUy51covIlryQPTy9ERa+zraeF3fW0GvW4AAAGE
MPHZGgAABAMARzBFAiEA8wvuB2PiVXJGdlYu48U5roWqlQX1VZduIavzLhUNRuQC
IEL1gmBL+QtxOSLdHo+NCTcCAUxVY//OBoAsD5kEddFzAHUAs3N3B+GEUPhjhtYF
qdwRCUp5LbFnDAuH3PADDnk2pZoAAAGEMPHZXgAABAMARjBEAiB9OD3ZknmeipcL
hTZcDrR3No1xhfQal1PaA82x6jUnkgIgBMI+eFr1aToFuqUYoBMwF7OEExN5uvoO
ZiUuIZKNEgUAdQC3Pvsk35xNunXyOcW6WPRsXfxCz3qfNcSeHQmBJe20mQAAAYQw
8dkvAAAEAwBGMEQCIF445YvGza/exv6s6IwgjMCuPV9FbrDy3u23Z9hYBUoRAiB3
Y5/BfqJB7bMgMTRCSmMqm33+Tee8lHDT5EHzRSauEzAKBggqhkjOPQQDAgNHADBE
AiBl1aSUtSIw+PzhZFXXKhmEBTFSrcmmHHDdeeD5qQZq0gIgVTyWbt8lAPm8YKQG
YnYOXCV24bwb9fKRWp4NmPfpb7A=
-----END CERTIFICATE-----
)EOF";

void firmwareUpdate(const char*ssid, const char *pass, const char *firmwareUrl, const char*fsUrl)
{
  // firmware URL con not be empty
  if (strcmp(firmwareUrl, "") == 0)
    ESP.restart();

  
  // connect WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
      i++;
      Serial.print(".");
      if (i == 10) ESP.restart();
      delay(500);
  }
  Serial.println("Connected To Wifi");
  
  //setting up WiFi client
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  
   // update fs if needed
  if (strcmp(fsUrl, "") != 0)
  {
    Serial.println("updating flash");
    t_httpUpdate_return _fs = httpUpdate.updateSpiffs(client, fsUrl, "");
    Serial.println("update flash done");
  }
  
  // update firmware
  t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl);
}