# Présentation

Ce logiciel est la partie cliente d'un systeme de logiciel chargé de s'occuper de la reconnaissance faciale sur un controleur de domaine. Il est destiné uniquement aux environnements LINUX.

# Dépendance

* libpam-dev
* libopencv-dev (2.4.1)
* liqt4-dev

# Installation

1. Pour l'installation de ce logiciel dans un premier temps, [telecharger la source du logiciel](https://github.com/binouossi/Face_authentication_client).

2. Creer un dossier de compilation dans la source telechargée, et s'y placé.  

	mkdir build  
	cd buid  

3. Executer les commandes:  

	cmake -D CMAKE_INSTALL_PREFIX=/usr ..  
	make  
	make install  

## Configuration

Avant toute utilisation du logiciel, il faut lui indiquer l'addresse réseau de son serveur. Ceci se fait grace à son fichier de configuration /etc/Face_client. Par default l'addresse localhost renseigner.

De meme il faut reconfigurer PAM pour qu'il utilise ce module. Pour cela, allez dans /etc/pam.d/\[logiciel "Le logiciel qui devrait utilser le module. Pour une configuration global, utiliser common-auth" \] pour y inserer la ligne:

	auth sufficient Face_client.so enableX

# Auteur
TACOLODJOU Emmanuel  
Maintainer  
tacolodjounini@gmail.com  

# LICENCE

Ce logiciel est protege par la licence GPLv3
