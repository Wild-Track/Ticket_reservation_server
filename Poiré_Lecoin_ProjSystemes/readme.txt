C'est un projet système ou le but est de réaliser une application qui gère des places de concert entre un client et un serveur. 
Nous avons décidé de réaliser cette application en multi-thread et permettons donc de gérer plusieurs connexions à la fois.
Des structures sont créées pour gérer les places et les réservations.
Puis des mutex qui lock et unlock l'accès à ces informations permettent une synchronisation des structures.

Pour compiler le server.c "gcc -pthread -o a.out server.c"
Pour compiler le client.c "gcc -o a.out client.c"