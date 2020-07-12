Account manager
===============

I) Installation
---------------

OS Comptatible: Windows 10
Executé account_manager.exe.
 
II) Langue disponible
---------------------

Actuellement lla langue d'affichage dépend de la langue de votre système d'exploitation.
- Anglais (défaut)
- Français

III) Utilisation
----------------

1) Compte
----------

Un compte est un ensemble transaction, que ce soit des dépenses, ou des revenues.
Pour créé un compte cliqué sur le menu "Compte", et "Nouveau compte".
Indiqué le nom du compte, le montant initial présent sur le compte, et la date d'ouverture du compte.
Quand une nouvelle est ajouté avant la première date présente, la date et le montant initial sera automatiquement ajusté. Le total du compte à l'ancienne date d'ouverture n'aura pas changé.
Vous pouvez supprimé un compte en cliquant sur le menu "Compte" et "Supprimer le compte"
Quand aucun compte n'existe, la fenêtre de création de compte s'ouvrira automatiquement.
Vous pouvez changé le compte affiché grâce à la liste en haut à droite.

2) Transaction
--------------

Une transaction est soit un revenu, soit une dépense pour le compte actif.
Pour ajouté une transaction, cliqué sur le menu "Compte courant" et "Ajouter transaction".
Indiqué l'intituler de la transaction, son montant, sa date, et si il s'agit d'une dépense ou d'un revenu.
Un revenu augmentera le total du compte, tandis qu'une dépense le dimunuera? 
Vous pouvez supprimer un transaction. Sélectionné une transaction dans la liste des transactions, cliqué sur le menu "Compte courant", et cliqué sur "Supprimer transaction".
Pour édité un transaction, cliqué sur la transaction désiré. Vous pourrez modifié l'intitulé, le montant, et assigné une catégorie à la transaction (voir section 4)

3) Liste de transactions
------------------------

Cette liste affiche toutes les transactions des dates sélectionné. Si aucune date n'est sélectionné, toutes les transactions du compte seront affiché.
Quand une transaction est sélectionné, une ligne rouge signifie une dépense, et une ligne verte un revenu. Quand un * apparait à côté d'un symbole +/-, ce la signifie qu'il s'agit d'une transaction prévisionnelle, ie une transaction ajouté à une date postérieur à la date courante.
Quand une transaction estimé devient antérieur au jour courant, une fenêtre de validation apparait. Cette fenêtre permet de validé ou de supprimé une transaction estimé. Si vous validé une transaction, celle ci deviendra non estimé.  

4) Catégories
-------------

Vous pouvez ajouté une nouvelle catégorie en cliquant sur le dernier élément de la liste des catégories. Une catégories est lié soit aux revenues, soit au dépenses pour un seul compte.

5) Calendrier
-------------

Le calendrier fournit une prévisualisation de détails du compte actif.
Pour chaque jour possédant au moins une transaction, un rectangle vert ou rouge apparaitra en haut de la case du jour. Un rectangle vert correspond à un total journalier positif, tandis qu'un rectangle rouge correpond à un total journalier négatif.
Pour chaque jour compris entre la première et la dernière transactions, un rectangle vert ou rouge apparaitra uen bas de la case du jour. Un rectangle vert correspond à un total journalier positif, tandis qu'un rectangle rouge correpond à un total journalier négatif.
Vous pouvez sélectionné plusieurs date en maintenant la touche shift. Quand une ou plusieurs date sont sélectionné, la liste des transactions n'affiche que les transactions pour les dates sélectionné.


6) Graphique
------------

L'onglet graphique fournit une visualisation de l'évolution du compte dans le temps. La partie bleu correspond au transaction validé, tandis que la partie verte correspond au transaction prévu.
Par défaut la vue montre l'évolution sur un mois, vous pouvez changé cette vue avec la molette de la souris pour passé sur une vue sur 1 an, ou une vue sur toutes la durée d'existence du compte.
L'onglet graphique permet également de visualer la répartition des transactions en fonction de leur catégorie, répartie entre dépense et revenu.

7) Récurrence
------------

Une récurrence est une transaction qui se répéte de façon réguliére. 
Une récurrence est définie par une transaction de référence, le type de la transaction (dépense ou revenu), la périodicité de la récurrence.
Une fois ces éléments de référence réglé, vous pouvez généré les transactions entre 2 dates, et faire une récurrences finie, ou définir une récurrence infinie, qui généreras de façon régulière les transactions.  

8) Budget
---------

La fonctionnalité de budget permet de gérer un budget sur une période de temps. Un budget est lié à une catégorie. 
Pour un budget vous devez ajouter une ou plusieurs cible. Une cible se définit par une date de début, une valeur et une période de temps. 
Une itération du budget se reproduit toute les péiode de temps. 

9) Dépense commune
------------------

Une zone de dépense commune permet de définir un groupe de dépense ou de revenue commun entre plusieurs participants.
Cela permet in fine de répartir équitablement les dépenses et revenue entre les différents particiapants, de donné la liste des différentes transactions à effectuer pour équilibré le tout.

10) Transfert
-------------

Dans cette fenêtre vous pouvez faire un transfert entre deux comptes.
Ceci créera une dépense dans le compte débiteur, et un revenue dans le comtpe recepteur.

IV) Options
-----------

La page d'options permet de régler un certains nombres de paramétres.
-La langue de l'application (Français ou anglais)
-L'activation de certaines fonctionnalitées
-Le support de sauvegarde principale, et , si activé, d'archive
-L'auto archivation à la sortie de l'application

V) Zone de notification
-----------------------

L'application peut être réduite à la zone de notification. 
Depuis la zone de notifation vous pouvez:
-Ajouter une transaction
-Cacher ou montrer l'application
-Quitter l'application

VI) Raccourcis
--------------

CTRL+Q: Quitter l'application
CTRL+N: Créer un nouveau compte
CTRL+SHIFT+D: Supprimer le compte courant
CTRL+A: Ajouter une nouvelle transaction
CTRL+D: Supprimer la transaction courante
SHIFT+ clique gauche une date du calendrier: Sélection de plusieurs dates 
F: Ouvrir le fenêtre de transfert
