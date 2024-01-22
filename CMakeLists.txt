cmake_minimum_required(VERSION 3.10)

project(upload_video)

# Adicione seus arquivos-fonte aqui
set(SOURCE_FILES
    main.cpp
    # Outros arquivos necessários
)

# Especifique o nome do executável
add_executable(main ${SOURCE_FILES})

# Se você estiver usando recursos específicos do C++11 ou superior, adicione estas linhas
set_property(TARGET main PROPERTY CXX_STANDARD 11)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)

# Adicione quaisquer bibliotecas ou flags de compilação necessários
# target_link_libraries(seu_programa suabiblioteca)

# Se estiver usando threads (por exemplo, std::thread), adicione
set(BOOST_ROOT /usr/include/boost)
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)

target_link_libraries(main PRIVATE Boost::filesystem Boost::system CURL::libcurl Threads::Threads)
