# IRC #

IRC significa Internet Relay Chat. É um protocolo de comunicação que permite que usuários se conectem a servidores de chat e participem de discussões em grupo (chamadas de "canais") ou em conversas privadas. O IRC foi criado no final dos anos 1980 e tornou-se uma das primeiras formas de comunicação em tempo real na internet.

# Socket #

Um socket é um ponto final de uma conexão de rede, permitindo a comunicação entre dois dispositivos em uma rede, como um servidor e um cliente. Em termos simples, é uma interface que permite que um programa de software envie e receba dados através de uma rede.
Características Principais:

    Endereço IP e Porta: Um socket é identificado por uma combinação de um endereço IP (que identifica o dispositivo na rede) e um número de porta (que identifica um processo específico no dispositivo).
    Protocolo de Transporte: Sockets geralmente utilizam protocolos como TCP (Transmission Control Protocol) ou UDP (User Datagram Protocol) para a comunicação. TCP oferece uma conexão confiável e orientada a fluxo, enquanto UDP é mais rápido, mas não garante a entrega dos pacotes.

Tipos de Sockets:

    Sockets de Fluxo (TCP): Utilizados para conexões orientadas a conexão, como a comunicação HTTP entre um navegador e um servidor web. Eles garantem que os dados sejam entregues de forma ordenada e sem perdas.

    Sockets de Datagramas (UDP): Usados para conexões sem conexão, onde a entrega dos pacotes não é garantida e pode ser desordenada. Exemplos incluem streaming de vídeo ao vivo ou jogos online, onde a velocidade é mais importante que a confiabilidade absoluta.


# NONBLOCK #

Modo Non-blocking (Não Bloqueante):

    Comportamento: No modo NONBLOCK, quando um programa tenta realizar uma operação de I/O (como ler ou escrever dados), a operação retorna imediatamente, mesmo que a operação não possa ser completada naquele momento. Se, por exemplo, um programa tenta ler dados de um socket que ainda não tem dados disponíveis, a operação retornará imediatamente com uma indicação de que não há dados disponíveis, em vez de fazer o programa esperar até que os dados estejam prontos.

    Uso em Sockets:
        Quando um socket está em modo non-blocking, operações como connect(), accept(), read(), ou write() retornam imediatamente. Se a operação não puder ser completada, a função geralmente retorna um erro ou um código especial (como EWOULDBLOCK ou EAGAIN no Unix) para indicar que a operação não pôde ser realizada no momento, mas que o programa pode tentar novamente mais tarde.