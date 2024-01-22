# VideoUploader: Simplificando o Envio de Vídeos Longos

Bem-vindo ao VideoUploader, uma ferramenta em C++ que facilita o processo de upload de vídeos longos usando a metodologia de Chunked Transfer Encoding. Este projeto foi desenvolvido para contribuir com a comunidade, oferecendo uma solução eficiente e fácil de usar.

## Overview

O VideoUploader é uma aplicação em C++ que automatiza o envio de vídeos longos para uma API online. Ele utiliza a transferência de chunks para garantir um upload estável e eficiente, além de oferecer recursos como verificação de estabilidade do tamanho do arquivo e monitoramento de progresso.

## Pré-requisitos

Antes de começar, certifique-se de ter instalado:

- **C++ Compiler**: Certifique-se de ter um compilador C++ instalado em sua máquina.
- **Curl Library**: A biblioteca Curl é necessária para a comunicação HTTP. Instale conforme as instruções específicas do seu sistema.
- **Boost Library**: O projeto utiliza a Boost Filesystem Library para manipulação de arquivos. Certifique-se de tê-la instalada.

## Configuração

1. **Baixe o código-fonte**: Clone ou baixe este repositório para o seu ambiente local.

2. **Configuração do Ambiente**: Certifique-se de ter todas as bibliotecas necessárias instaladas conforme os pré-requisitos.

3. **Edite as Configurações**: Abra o arquivo `main.cpp` e ajuste as configurações no início do arquivo de acordo com suas necessidades.

```cpp
std::string pastaVideos = "videos a serem enviados";
std::string apiUrl = "url http";
std::string apiKey = "se necessario token aqui";
```

- `pastaVideos`: O caminho para a pasta que contém os vídeos a serem enviados.
- `apiUrl`: A URL da API para a qual os vídeos serão enviados.
- `apiKey`: A chave de autenticação, se necessária.

## Uso

1. **Compile o Código**: Utilize o compilador C++ para compilar o código.

```bash
g++ main.cpp -o VideoUploader -lcurl -lboost_filesystem -lboost_system
```

2. **Execute o Programa**: Execute o binário gerado.

```bash
./VideoUploader
```

O VideoUploader agora monitorará a pasta especificada, enviando automaticamente vídeos estáveis para a API.

## Contribuições

Sinta-se à vontade para contribuir para este projeto. Se você encontrar problemas ou tiver ideias para melhorias, abra uma issue ou envie um pull request. Juntos, podemos tornar o VideoUploader ainda mais poderoso e amigável.

Divirta-se utilizando o VideoUploader! 🚀