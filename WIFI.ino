#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

#define WIFI_SSID "ssid" 
#define WIFI_PASSWORD "pass"
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "riegointeligente2610@gmail.com"
#define AUTHOR_PASSWORD "pass"
#define RECIPIENT_EMAIL "miloaizam@gmail.com"

SMTPSession smtp;
ESP_Mail_Session session;  // Declaración global
String historialRiegos;

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  
  smtp.debug(1);
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  Serial.println("D1 Mini listo para recibir datos de riego.");
}

void loop() {
  if (Serial.available()) {
    String datosRiego = Serial.readStringUntil('\n');
    historialRiegos += datosRiego + "\n";
  }

  static unsigned long lastEmailTime = 0;
  if (millis() - lastEmailTime >= 2 * 60 * 1000) {  // 2 minutos para pruebas
    enviarCorreo(historialRiegos);
    historialRiegos = "";  // Reinicia el historial tras enviar el correo
    lastEmailTime = millis();
  }
}

void enviarCorreo(const String &contenido) {
  SMTP_Message message;
  message.sender.name = "RiegoInteligente ESP8266";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Registro de Riegos";
  message.addRecipient("RIEGO INTELIGENTE", RECIPIENT_EMAIL);
  
  message.text.content = contenido.c_str();
  Serial.println(contenido);
  
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  if (!smtp.connect(&session))
    return;
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error enviando correo: " + smtp.errorReason());
  else
    Serial.println("Correo enviado correctamente.");
}
