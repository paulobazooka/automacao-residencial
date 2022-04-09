#include <GerenciadorWifi.h>


GerenciadorWifi::GerenciadorWifi(AsyncWebServer *s, DNSServer *dnsServer, IPAddress *IP, IPAddress *GATEWAY, IPAddress *MASK_SUBNET, Dispositivo *dispositivo)
{
    this->asyncWebServer = s;
    this->dnsServer = dnsServer;
    this->IP = IP;
    this->GATEWAY = GATEWAY;
    this->MASK_SUBNET = MASK_SUBNET;
    if (dispositivo != nullptr)
      this->dispositivo = dispositivo;
}

GerenciadorWifi::GerenciadorWifi(AsyncWebServer *s, DNSServer *dnsServer, IPAddress *IP, IPAddress *GATEWAY, IPAddress *MASK_SUBNET, Dispositivo *dispositivo, float *temperatura, float *umidade)
{
    this->asyncWebServer = s;
    this->dnsServer = dnsServer;
    this->IP = IP;
    this->GATEWAY = GATEWAY;
    this->MASK_SUBNET = MASK_SUBNET;
    this->temperatura = temperatura;
    this->umidade = umidade;
    if (dispositivo != nullptr)
      this->dispositivo = dispositivo;
}

GerenciadorWifi::GerenciadorWifi(AsyncWebServer *s, DNSServer *dnsServer, IPAddress *IP, IPAddress *GATEWAY, IPAddress *MASK_SUBNET, Dispositivo *dispositivo, Credenciais *credenciais, float *temperatura, float *umidade)
{
    this->asyncWebServer = s;
    this->dnsServer = dnsServer;
    this->IP = IP;
    this->GATEWAY = GATEWAY;
    this->MASK_SUBNET = MASK_SUBNET;
    this->temperatura = temperatura;
    this->umidade = umidade;
    if (dispositivo != nullptr)
      this->dispositivo = dispositivo;
    if (credenciais != nullptr)
      this->credenciais = credenciais;
}

GerenciadorWifi::~GerenciadorWifi()
{
}

boolean GerenciadorWifi::obterModoOperacao()
{
    if(WiFi.getMode() == WIFI_STA)
        return MODO_STATION;
    else
        return MODO_ACCESS_POINT;
}

bool GerenciadorWifi::conectado() { return WiFi.status() == WL_CONNECTED; }

void GerenciadorWifi::conectarWiFi(String ssid, String pass)
{
  this->imprimirLog("Conectando na rede ssid:" + ssid + " senha:" + pass);
  WiFi.begin(ssid, pass);
  delay(1000);
  for (size_t i = 0; i < TENTATIVAS_CONEXAO; i++)
  {
    if (!WiFi.isConnected())
    {
      delay(600);
      this->imprimirLog("Tentativa de conexão " + String(i + 1));
    } 
    else
      break;
  }

  if (WiFi.isConnected())
    this->imprimirLog("Conectado na rede " + ssid + " com o IP: " + WiFi.localIP().toString() + " e MAC: " + WiFi.macAddress());
  else 
    this->imprimirLog("ERRO ao conectar na rede ssid:" + ssid + " senha:" + pass);
}

void GerenciadorWifi::capturarRequisicoes()
{
  this->dnsServer->processNextRequest();
}

void GerenciadorWifi::modoStation()
{
  if(WiFi.mode(WIFI_STA)){
    if (this->dnsServer != nullptr)
      this->dnsServer->stop();
    this->imprimirLog("Modo Station Ativado!");
  }
  else  
    this->imprimirLog("ERRO ao ativar o modo Station!");
}

void GerenciadorWifi::modoAccessPoint()
{
  if(WiFi.mode(WIFI_AP_STA)){
    if(WiFi.softAPConfig(*this->IP, *this->IP, IPAddress(255, 255, 255, 0))){
      WiFi.disconnect();
       if(WiFi.softAP(this->HOSTNAME, this->PASS)){      
        this->dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
        if( this->dnsServer->start(this->DNS_PORT, "*", *this->IP)){
          this->imprimirLog("Servidor DNS ativado! " + WiFi.softAPIP().toString());
        } else {
          this->imprimirLog("ERRO ao ativar Servidor DNS!");
        }
      } else {
        this->imprimirLog("ERRO ao configurar o modo Access Point! SSID: " + this->HOSTNAME + " PASS: " + this->PASS);
      }
    } else{
      this->imprimirLog("ERRO ao configurar servidor Access Point!");
    }   
  }
  else {
    this->imprimirLog("ERRO ao ativar o modo Access Point!");
  }      
}

IPAddress GerenciadorWifi::localIp() { return WiFi.localIP(); }

String GerenciadorWifi::getIP(){
  if (WiFi.isConnected() && WiFi.getMode() == WIFI_STA)
    return WiFi.localIP().toString();
  else
    return WiFi.softAPIP().toString();
}

String GerenciadorWifi::getStatus(){
  if (WiFi.isConnected() && WiFi.getMode() == WIFI_STA)
    return "   Acesse no browser o IP: " + WiFi.localIP().toString() + "/botoes para controlar o dispositivo";
  else
    return this->MENSAGEM_CONFIGURACAO_WIFI;
}

String GerenciadorWifi::processor(const String &var)
{
  this->imprimirLog("Processando requisição: " + var);

  if (var == "IP")
    return this->getIP();
  if (var == "RELE1")
    return "cozinha"; 
  if (var == "RELE2")
  return "sala";
  if (var == "RELE3")
    return "corredor";
  if (var == "RELE4")
    return "garagem";
  if (var == "RELE5")
    return "fundos";
  if (var == "RELE6")
    return "quarto1";
  if (var == "RELE7")
    return "quarto2";
  if (var == "RELE8")
    return "quarto3";
  if (var == "modelo"){
    if (this->dispositivo != nullptr)
      return this->dispositivo->getModelo();
    else
      return String();
  }
  if (var == "versao_firmware"){
    if (this->dispositivo != nullptr)
      return this->dispositivo->getFirmware();
    else
      return String();
  }
  if (var == "versao_interface"){
    if (this->dispositivo != nullptr)
      return this->dispositivo->getInterface();
    else
      return String();
  }  
  
  return String();
}

void GerenciadorWifi::configurarServidor()
{
  this->imprimirLog("Configurando o servidor HTTP para receber requisições");
  
  // Rota GET em /
  this->asyncWebServer->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    this->imprimirLog("Requisição GET em /");
    request->send(LittleFS, "/html/index.html", String(), false, [this](const String &var) -> String { return processor(var); });
  });

  // Rota GET em /sensores/
  this->asyncWebServer->on("/sensores", HTTP_GET, [this](AsyncWebServerRequest *request) {
    this->imprimirLog("Requisição GET em /sensores");
    AsyncJsonResponse * response = new AsyncJsonResponse();
    JsonVariant& root = response->getRoot();
    root["temperatura"] = String(*this->temperatura, 1);
    root["umidade"] = String(*this->umidade, 1);
    this->imprimirLog("Enviando resposta: " + root.as<String>());
    response->setLength();
    response->setCode(200);
    request->send(response);
   
  });

  // Rota GET em /botoes
  this->asyncWebServer->on("/botoes", HTTP_GET, [this](AsyncWebServerRequest *request) {
    this->imprimirLog("Requisição GET em /botoes");
    request->send(LittleFS, "/html/botoes.html", String(), false, [this](const String &var) -> String { return processor(var); });
  });

  // Rota POST em /rele
  this->asyncWebServer->on("/rele", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [this](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

    if (request->hasArg("id")){
      String rele = request->arg("id");
      if (eventoRele != nullptr)
          this->eventoRele(rele.toInt());
    }  

     request->send(200);
  });


  // Rota POST em /wifi
  this->asyncWebServer->on("/wifi", HTTP_POST, [this](AsyncWebServerRequest *request) {
    this->imprimirLog("Requisição POST recebida em /wifi");
    AsyncWebParameter *s = request->getParam("ssid", true);
    AsyncWebParameter *p = request->getParam("pass", true);
    if (s != nullptr && p != nullptr) {
      this->eventoWifi(s->value(), p->value());
    } else {
      request->send(LittleFS, "/html/index.html", String(), false, [this](const String &var) -> String { return processor(var); });
    }
  });

   // Rota para carregar o arquivo style.css
  this->asyncWebServer->on("/style/index.css", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style/index.css", "text/css");
  });

  // Rota para carregar o arquivo botoes.css
  this->asyncWebServer->on("/style/botoes.css", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style/botoes.css", "text/css");
  });

  // Rota para carregar o arquivio favicon.ico
  this->asyncWebServer->on("/image/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/image/favicon.ico", "image/x-icon");
  });

  // Rota para carregar o arquivio gota.png
  this->asyncWebServer->on("/image/gota.png", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/image/gota.png", "image/png");
  });

  // Rota para carregar o arquivio termometro.png
  this->asyncWebServer->on("/image/termometro.png", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/image/termometro.png", "image/png");
  });

  // Rota para carregar o arquivio configuracoes.png
  this->asyncWebServer->on("/image/configuracao.png", HTTP_GET, [this](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/image/configuracao.png", "image/png");
  });

  this->asyncWebServer->serveStatic("/", LittleFS, "/image/").setCacheControl("max-age=86400");
  this->asyncWebServer->serveStatic("/", LittleFS, "/style/").setCacheControl("max-age=86400");

  this->asyncWebServer->begin();  // Inicia o servidor
}

void GerenciadorWifi::callbackReconfiguracaoWifi(void (*alterarConfiguracaoWifi) (String ssid, String pass)){
  this->eventoWifi = alterarConfiguracaoWifi;
}

void GerenciadorWifi::callbackEstadoRele(void (*alterarEstadoRele) (int rele)){
  this->eventoRele = alterarEstadoRele;
}

void GerenciadorWifi::setCallbackLog(void (*callBackLogWifi) (String mensagem)){
    this->log = callBackLogWifi;
}

void GerenciadorWifi::imprimirLog(String msg){
  if (this->log != nullptr)
      this->log(msg);
}

void GerenciadorWifi::scanearRedes(){
    uint8_t redes = WiFi.scanNetworks();
    this->imprimirLog("-------------" + String(redes) + " redes encontradas! ------------");
    for(uint8_t i = 0; i < redes; i++){
      this->imprimirLog("SSID: " + WiFi.SSID(i) +
                        "\tRSSI: " + WiFi.RSSI(i) +
                        "\tENCRY: " + WiFi.encryptionType(i) +
                        "\tBSSIDstr: " + WiFi.BSSIDstr(i) +
                        "\tCanal: " + WiFi.channel(i));
    }
    this->imprimirLog("-----------------------------------------------");
}