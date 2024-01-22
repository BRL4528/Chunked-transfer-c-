#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <boost/filesystem.hpp>
#include <curl/curl.h>
#include <fstream>
#include <sstream>

namespace fs = boost::filesystem;

// Função para verificar a conexão com a Internet
bool verificarConexaoInternet()
{
    // Lógica de verificação da conexão com a Internet
    // Retorne true se estiver conectado, false caso contrário
    return true; // Substitua por sua própria lógica de verificação
}

// Função para enviar o vídeo para a API online
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t total_size = size * nmemb;
    output->append((char *)contents, total_size);
    return total_size;
}

int progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    // Exemplo simples de exibição do progresso
    std::cout << "Progresso: " << ulnow * 100 / ultotal << "% concluído\r" << std::flush;

    // Retorne 0 para continuar a transferência, ou um valor diferente de 0 para cancelar
    return 0;
}

bool isFileSizeStable(const std::string &filePath, size_t consecutiveChecks)
{
    std::ifstream fileStream(filePath, std::ios::binary);
    if (!fileStream)
    {
        std::cerr << "Erro ao abrir o arquivo de vídeo." << std::endl;
        return false;
    }

    // Obter o tamanho inicial do arquivo
    fileStream.seekg(0, std::ios::end);
    size_t initialSize = fileStream.tellg();
    fileStream.close();

    // Aguardar e verificar consecutivamente se o tamanho do arquivo permanece estável
    for (size_t i = 0; i < consecutiveChecks; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Aguardar 5 segundos entre as verificações

        fileStream.open(filePath, std::ios::binary);
        if (!fileStream)
        {
            std::cerr << "Erro ao abrir o arquivo de vídeo." << std::endl;
            return false;
        }

        fileStream.seekg(0, std::ios::end);
        size_t currentSize = fileStream.tellg();
        fileStream.close();

        // Verificar se o tamanho do arquivo permanece o mesmo
        if (currentSize != initialSize)
        {
            // Tamanho do arquivo ainda está mudando, continue verificando
            initialSize = currentSize;
        }
        else
        {
            // Tamanho do arquivo permanece estável
            return true;
        }
    }

    return false; // O tamanho do arquivo ainda está mudando após várias verificações
}

bool enviarVideo(const std::string &filePath, const std::string &apiUrl, const std::string &apiKey, const std::string &httpMethod)
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        std::ifstream fileStream(filePath, std::ios::binary);

        if (!fileStream)
        {
            std::cerr << "Erro ao abrir o arquivo de vídeo." << std::endl;
            return false;
        }

        // Obter apenas o nome do arquivo (sem extensão)
        std::string fileName = fs::path(filePath).stem().string();

        // Configurar a URL da API
        std::string url = apiUrl + "SUA ROTA AQUI" + fileName;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Configurar o cabeçalho da requisição
        struct curl_slist *headers = NULL;
        // headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        headers = curl_slist_append(headers, "Transfer-Encoding: chunked"); // Usar transferência de chunks
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

         // Configurar o método HTTP (POST ou PATCH)
        if (httpMethod == "PATCH")
        {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        }


        // Configurar a função de escrita de dados
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Configurar o formulário multipart
        curl_httppost *post = NULL;
        curl_httppost *last = NULL;

        curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", CURLFORM_FILE, filePath.c_str(), CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

        // Configurar a função de retorno de chamada para lidar com a resposta da API
        std::string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // Executar a solicitação
        res = curl_easy_perform(curl);

        // Verificar se a solicitação foi bem-sucedida
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            std::cout << "Upload bem-sucedido. Resposta da API: " << response_data << std::endl;
        }

        // Limpar
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_formfree(post);

        curl_global_cleanup();

        // Retornar true se a solicitação for bem-sucedida, false caso contrário
        return res == CURLE_OK;
    }

    // Retornar false em caso de falha
    return false;
}

int main()
{
    // Caminho para a pasta de vídeos
    std::string pastaVideos = "videos a serem enviados";
    std::string apiUrl = "url http";                                                                                                                                                                       // Substitua pela URL da sua API
    std::string apiKey = "se necessario token aqui"; // Substitua pela chave da sua API
    // std::string videoId = "7042301e-458c-4837-8942-b481199bf141";                                                                                                                                                        // Substitua pelo ID do seu vídeo

    // Loop principal
    while (true)
    {
        // Verificar se há novos vídeos na pasta
        for (const auto &entry : boost::filesystem::directory_iterator(pastaVideos))
        {
            if (boost::filesystem::is_regular_file(entry))
            {
                std::cout << "Novo vídeo detectado: " << entry.path() << std::endl;

                if (isFileSizeStable(entry.path().string(), 3))
                {
                    // Aguardar até que haja uma conexão com a Internet
                    while (!verificarConexaoInternet())
                    {
                        std::cout << "Aguardando conexão com a Internet..." << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(10)); // Esperar 10 segundos antes de verificar novamente
                    }

                    // Enviar o vídeo para a API online
                    if (enviarVideo(entry.path().string(), apiUrl, apiKey, "PATCH"))
                    {
                        std::cout << "Vídeo enviado com sucesso." << std::endl;

                        // Remover o vídeo local após o envio bem-sucedido
                        fs::remove(entry.path());
                        std::cout << "Vídeo local removido." << std::endl;
                    }
                    else
                    {
                        std::cerr << "Erro ao enviar o vídeo." << std::endl;
                    }
                }
                else
                {
                    std::cout << "Aguardando estabilização do tamanho do arquivo..." << std::endl;
                }
            }
        }

        // Aguardar antes de verificar novamente (intervalo em segundos)
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }

    return 0;
}
