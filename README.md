# Projeto de Automação Residencial
O presente projeto consiste em automatizar de maneira simples o acendimento de lâmpadas em alguns cômodos da casa através de uma página html disponibilizadas por um webserver com NodeMCU8266;

# Funções
* Gerenciamento de conexão com o roteador doméstico
* Monitoramento da temperatura e umidade relatica do ar com display LCD 16x2
* Controle de acendimento de lâmpadas pelo portal do webserver local
* Monitoramento da temperatura e umidade pelo site local do webserver

# Materiais Utilizados
* NodeMCU8266
* MCP23017 (extensor de GPIO 16 portas)
* Display LCD 16x2 com interface I2C
* Sensor de temperatura e umidade DHT22
* Fonte 5V 6W

# Ferramentas Utilizadas
Para desenvolvimento do projeto utilizou-se o editor de texto [VSCode](https://code.visualstudio.com/download) juntamente com o plugin [PlatformIO](https://platformio.org/install/ide?install=vscode). Tomou-se como base a plataforma arduino.

# Instruções de Upload de arquivos para o NodeMCU8266
[Clique aqui](data/README.md) para mais informações