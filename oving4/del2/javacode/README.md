# Hvordan sette opp greia

## The link to the code
``[Java TLS] - http://java-buddy.blogspot.no/2016/07/java-example-of-ssl-server-and-client.html``


## Keytool
``
keytool -genkeypair -alias signFiles -keyalg RSA -keystore examplestore -storepass DITT_PASSORD -validity 365 -keysize 2048
``

## Kompiler koden
``
javac JavaSSLServer.java JavaSSLClient.java
``

## Aktiver 

### Serveren

``
java -Djavax.net.ssl.keyStore=examplestore -Djavax.net.ssl.keyStorePassword=DITT_PASSORD JavaSSLServer
``

### Klienten 

``
java -Djavax.net.ssl.trustStore=examplestore -Djavax.net.ssl.trustStorePassword=DITT_PASSORD JavaSSLClient
``

## Inspisere

``
keytool -list -v -keystore examplestore -storepass DITT_PASSORD
``