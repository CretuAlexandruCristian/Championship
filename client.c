
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

void MeniuPrincipal()
{
printf("1.Login\n");
printf("2.Register\n");
printf("3.Exit\n");
}
void MeniuUser()
{
printf("1.Inregistrare campionat\n");
printf("2.Inscriere la campionat\n");
printf("3.Sign Out\n");
printf("4.Exit\n");

}
void MeniuAdmin()
{
printf("1.Inregistrare campionat\n");
printf("2.Vizualizare partide\n");
printf("3.Sign Out\n");
printf("4.Exit\n");
}
int powTwo(int x)
{
   //checks whether a number is zero or not
   if (x == 0)
      return 0;

   //true till x is not equal to 1
   while( x != 1)
   {
      //checks whether a number is divisible by 2
      if(x % 2 != 0)
         return 0;
         x /= 2;
   }
   return 1;
}

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;
int ID,nrmax,ziua,luna,an;
int idmeci,scor1,scor2;
char numejoc[100],reguli[100],modextragere[100];
char email[100];

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100];		// mesajul trimis
  char username[100]; // username trimis
  char password[100]; // parola trimisa 

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
int loginOK=0;
int registerOK=1;
while(1)
if(loginOK==0)
  /* citirea mesajului */
  {
  MeniuPrincipal();
    int key;
    bzero(&key,sizeof(int));
    printf("Introduceti comanda:");
    fflush(stdout);
    scanf("%d",&key);
    if(key==1)
    if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
  bzero (username, 100);
  printf ("[client]Introduceti username: ");
  fflush (stdout);
  read (0, username, 100);

  /* trimiterea mesajului la server */
  if (write (sd, username, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
   bzero (password, 100);
  printf ("[client]Introduceti parola: ");
  fflush (stdout);
  read (0, password, 100);

  /* trimiterea mesajului la server */
  if (write (sd, password, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
//bzero(&loginOK,sizeof(int));
  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd, &loginOK, sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  if(loginOK==0)
  printf ("Username/parola incorecta!\n");
  else printf("Bine ati revenit!\n");
  }
  else if (key==2)
  if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
        bzero (username, 100);
  printf ("[client]Introduceti username: ");
  fflush (stdout);
  read (0, username, 100);

  /* trimiterea mesajului la server */
  if (write (sd, username, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
   bzero (password, 100);
  printf ("[client]Introduceti parola: ");
  fflush (stdout);
  read (0, password, 100);

  /* trimiterea mesajului la server */
  if (write (sd, password, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
//bzero(&loginOK,sizeof(int));
  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd, &registerOK, sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  if(registerOK==0)
  printf ("Nu va puteti inregistra!\n");
  else
  printf ("V-ati inregistrat cu succes!\n");
    }
    else if (key==3)
  if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
      break;
    }
  }
  else if(loginOK==1)
  {
    MeniuUser();
    int key;
    bzero(&key,sizeof(int));
    printf("Introduceti comanda:");
    fflush(stdout);
    scanf("%d",&key);
    if(key==1)
    if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
    //   int ID=genID();
    //    bzero(&ID,sizeof(int));
    // fflush(stdout);
    //   if (write (sd, &ID, sizeof(int)) <= 0)
    // {
    //   perror ("[client]Eroare la write() spre server.\n");
    //   return errno;
    // }
    //printf("%d\n",ID);
    //char numejoc[100];
    bzero(numejoc,100);
    printf("Introduceti numele jocului:");
    fflush(stdout);
    scanf("%s",numejoc);
    if (write (sd, numejoc, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    //char reguli[100];
    bzero(reguli,100);
    printf("Introduceti regula:");
    fflush(stdout);
    scanf("%s",reguli);
    if (write (sd, reguli, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
   
    //char modextragere[100];
    bzero(modextragere,100);
    
    printf("Introduceti modul de extragere:");
    fflush(stdout);
    scanf("%s",modextragere);
    
    
    if (write (sd, modextragere, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    //int nrmax;
    nrmax=0;
    while(powTwo(nrmax)==0){
    bzero(&nrmax,sizeof(int));
    printf("Introduceti numarul de participanti(putere a lui 2):");
    fflush(stdout);
    scanf("%d",&nrmax);
    }
    if (write (sd, &nrmax, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

    
    bzero(&ziua,sizeof(int));
    printf("Introduceti ziua:");
    fflush(stdout);
    scanf("%d",&ziua);
    
    if (write (sd, &ziua, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    
    bzero(&luna,sizeof(int));
    printf("Introduceti luna:");
    fflush(stdout);
    scanf("%d",&luna);
    
    if (write (sd, &luna, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

    bzero(&an,sizeof(int));
    printf("Introduceti anul:");
    fflush(stdout);
    scanf("%d",&an);
    
    if (write (sd, &an, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    // int nrcurent=0;
    // bzero(&nrcurent,sizeof(int));
    // fflush(stdout);
    // if (write (sd, &nrcurent, sizeof(int)) <= 0)
    // {
    //   perror ("[client]Eroare la write() spre server.\n");
    //   return errno;
    // }
    // int full=0;
    // bzero(&full,sizeof(int));
    // fflush(stdout);
    // if (write (sd, &full, sizeof(int)) <= 0)
    // {
    //   perror ("[client]Eroare la write() spre server.\n");
    //   return errno;
    // }
    printf("\n");
    }
   if(key==2){
    
    if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
      char campionate[5000];
    bzero(campionate,5000);
    if (read (sd, campionate, 5000) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  printf ("\n%s", campionate);
  printf ("0.Inapoi\n");
    bzero(&ID,sizeof(int));
    
    printf("Introduceti campionatul la care doriti sa participati:");
    fflush(stdout);
    scanf("%d",&ID);
    if (write (sd, &ID, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
  if(ID!=0){
    bzero(email,100);
    printf("Introduceti mailul cu care doriti sa va inscrieti la campionat:");
    fflush(stdout);
    scanf("%s",email);
    if (write (sd, email, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    int inscris;
  if (read (sd, &inscris, sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    if(inscris==0) printf("V-ati inscris cu succes!\n");
    else printf("Sunteti inscris deja la acest campionat!\n");

    }
    }
   }
    if(key==3) {
      if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
        if (read (sd, &loginOK, sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  if(loginOK==0)
  printf ("Va rugam sa va inregistrati!\n");
      
    }



    }
    
    if(key==4) {
      if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
      break;
    }
    }


  }
  else if(loginOK==2){
      MeniuAdmin();
      int key;
    bzero(&key,sizeof(int));
    printf("Introduceti comanda:");
    fflush(stdout);
    scanf("%d",&key);
    if(key==1)
    if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
    //   int ID=genID();
    //    bzero(&ID,sizeof(int));
    // fflush(stdout);
    //   if (write (sd, &ID, sizeof(int)) <= 0)
    // {
    //   perror ("[client]Eroare la write() spre server.\n");
    //   return errno;
    // }
    //printf("%d\n",ID);
    //char numejoc[100];
    bzero(numejoc,100);
    printf("Introduceti numele jocului:");
    fflush(stdout);
    scanf("%s",numejoc);
    if (write (sd, numejoc, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    //char reguli[100];
    bzero(reguli,100);
    printf("Introduceti regula:");
    fflush(stdout);
    scanf("%s",reguli);
    if (write (sd, reguli, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
   
    //char modextragere[100];
    bzero(modextragere,100);
    
    printf("Introduceti modul de extragere:");
    fflush(stdout);
    scanf("%s",modextragere);
    
    
    if (write (sd, modextragere, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    nrmax=0;
    while(powTwo(nrmax)==0){
    bzero(&nrmax,sizeof(int));
    printf("Introduceti numarul de participanti(putere a lui 2):");
    fflush(stdout);
    scanf("%d",&nrmax);
    }
    if (write (sd, &nrmax, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

    bzero(&ziua,sizeof(int));
    
    printf("Introduceti ziua:");
    fflush(stdout);
    scanf("%d",&ziua);
    
    if (write (sd, &ziua, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

    bzero(&luna,sizeof(int));
    
    printf("Introduceti luna:");
    fflush(stdout);
    scanf("%d",&luna);
    
    if (write (sd, &luna, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }

    bzero(&an,sizeof(int));
    
    printf("Introduceti anul:");
    fflush(stdout);
    scanf("%d",&an);
    
    if (write (sd, &an, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    // int nrcurent=0;
    // bzero(&nrcurent,sizeof(int));
    // fflush(stdout);
    // if (write (sd, &nrcurent, sizeof(int)) <= 0)
    // {
    //   perror ("[client]Eroare la write() spre server.\n");
    //   return errno;
    // }
    // int full=0;
    // bzero(&full,sizeof(int));
    // fflush(stdout);
    // if (write (sd, &full, sizeof(int)) <= 0)
    // {
    //   perror ("[client]Eroare la write() spre server.\n");
    //   return errno;
    // }
    printf("\n");
    }
    if(key==2) {

    if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
      char campionateAdmin[1000];
    bzero(campionateAdmin,1000);
    if (read (sd, campionateAdmin, 1000) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  printf ("\n%s\n", campionateAdmin);
  printf("0.Inapoi\n");

    bzero(&ID,sizeof(int));  
    printf("Introduceti ID-ul meciului pe care doriti sa il modificati:");
    fflush(stdout);
    scanf("%d",&ID);
    if (write (sd, &ID, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    if(ID!=0){
    bzero(&scor1,sizeof(int));  
    printf("Introduceti scorul primului jucator:");
    fflush(stdout);
    scanf("%d",&scor1);
    if (write (sd, &scor1, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    bzero(&scor2,sizeof(int));  
    printf("Introduceti scorul celui de-al doilea jucator:");
    fflush(stdout);
    scanf("%d",&scor2);
    if (write (sd, &scor2, sizeof(int)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    }
    }
    }

    if(key==3) {
      if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
        if (read (sd, &loginOK, sizeof(int)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  if(loginOK==0)
  printf ("Va rugam sa va inregistrati!\n");
      
    }
    }
    if(key==4){
      if (write (sd, &key, sizeof(int)) <= 0)
    {
     
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    else{
      break;
    }

    }
  }
  close(sd);
  /* inchidem conexiunea, am terminat */
  
}