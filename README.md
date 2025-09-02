# 🍷 Vinheria Agnello - Monitoramento com ESP32

Este projeto é uma **evolução do sistema de monitoramento da Vinheria Agnello**, antes feito com Arduino, agora atualizado com **ESP32** para maior conectividade e desempenho.

O sistema monitora **temperatura, umidade e luminosidade** do estoque de vinhos, garantindo condições ideais para conservação.  
Além disso, o ESP32 envia os dados coletados para uma **máquina virtual (VM) no Azure com Ubuntu Linux** através de um IP público, e pode receber comandos para acender ou desligar um **LED indicador**.

---

## 📡 Funcionalidades

- 📊 Leitura de **temperatura** e **umidade** com o sensor **DHT22**  
- 💡 Medição da **luminosidade** com o módulo **LDR**  
- 🔴 Controle de **LED remoto** (ligar/desligar via resposta da VM no Azure)  
- ☁️ Envio dos dados para um **servidor em nuvem (Azure VM - Ubuntu Linux)**  
- 🌍 Comunicação bidirecional: ESP32 envia medições e recebe comandos  

---

## 🔧 Tecnologias e Componentes

- **ESP32** – Microcontrolador com Wi-Fi integrado  
- **Sensor DHT22** – Leitura de temperatura e umidade  
- **Módulo LDR** – Medição de luminosidade  
- **LED** – Indicador de controle remoto  
- **Azure VM (Ubuntu Linux)** – Servidor para armazenamento e resposta  
- **C/C++ (Arduino IDE)** – Linguagem de programação  

---

## 🖥️ Estrutura do Projeto

- `sketch.ino` → Código principal do ESP32  
- Comunicação com servidor via **HTTP**  
- Tratamento dos sensores e acionamento do LED  

---

## ▶️ Demonstração

📽️ Vídeo simulando o projeto no YouTube: [https://youtube.com/](https://youtu.be/Sb0oKQqAEeU)  
🔗 Simulação no Wokwi: [https://wokwi.com/projects/440854623145093121](https://wokwi.com/projects/440854623145093121)

---

## 🚀 Como Executar

1. Clone este repositório:
   ```bash
   git clone https://github.com/marcossilva11/cp4-edgeComputing.git
   ```
2. Abra o projeto no **Arduino IDE**.
3. Instale as bibliotecas necessárias.
4. Configure as credenciais de **Wi-Fi** e o **IP da VM** no código.
5. Carregue o código para o **ESP32**.
6. Acompanhe a saída no **Serial Monitor** e observe o LED respondendo aos comandos.

---

## 👨‍💻 Integrantes do Grupo

* Daniel Laureano da Luz - RM565899
* Gabriel Del Sole Speck - RM562513
* Marcos Antonio da Silva - RM566451
* Paulo Gustavo Pessoa da Silva - RM566194
* Rone Cruz Santos - RM566558
