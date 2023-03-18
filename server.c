

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite3.h"

/* portul folosit */
#define PORT 2024

/* codul de eroare returnat de anumite apeluri */
extern int errno;

char sir[256],insert[500],sql[500];
int ID,nrmax,nrcurent,ziua,luna,an;
int idmeci,scor1,scor2;
char numejoc[100],reguli[100],modextragere[100];
char username[100],email[100],password[100];
char user[100];
char *err;
sqlite3 *db2;
sqlite3_stmt *stmt;
int rc;

int getMaxID(){
    int val ;
    FILE * fp = fopen("IDgenerator.txt", "r") ;
    fscanf(fp, "%d", &val) ;
    fclose(fp);
    return val ;
}
int genID()
{
    int val ;
    FILE * fp = fopen("IDgenerator.txt", "r") ;
    if (!fp) {
        fp = fopen("IDgenerator.txt", "w") ;
        if (!fp) return -1 ; // fail
        fprintf(fp, "%d", 1) ;
        fclose(fp) ;
        return 1;
    }
    fscanf(fp, "%d", &val) ;
    val++;
    fclose(fp); // close file for read
    fp = fopen("IDgenerator.txt", "w") ; // reopen for write
    fprintf(fp, "%d", val) ;
    fclose(fp) ;
    return val ;
}

void revstr(char *str1)  
{  
    // declare variable  
    int i, len, temp;  
    len = strlen(str1); // use strlen() to get the length of str string  
      
    // use for loop to iterate the string   
    for (i = 0; i < len/2; i++)  
    {  
        // temp variable use to temporary hold the string  
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}  

char inttochar(int x)
{
    if(x==0) strcpy(sir,"0");
    else{
    strcpy(sir,"");
int aux=x;
      char c;
    while(aux){
        c=aux%10 + '0';
        strncat(sir,&c,1);
        aux=aux/10;
    }
    revstr(sir);
    }
}
void InregistrareCampionat()
{
    strcpy(insert,"INSERT INTO campionat VALUES(");
    inttochar(ID);
    strcat(insert,sir);
        strcat(insert,",'");
      strcat(insert,numejoc);
      strcat(insert,"','");
      strcat(insert,reguli);
      strcat(insert,"','");
      strcat(insert,modextragere);
      strcat(insert,"',");
      inttochar(nrmax);
      strcat(insert,sir);
     strcat(insert,",");
      
    inttochar(nrcurent);
    strcat(insert,sir);
    strcat(insert,",");
    inttochar(ziua);
    strcat(insert,sir);
    strcat(insert,",");
    inttochar(luna);
    strcat(insert,sir);
    strcat(insert,",");
    inttochar(an);
    strcat(insert,sir);
    strcat(insert,")");
    printf("%s\n",insert);
     rc=sqlite3_exec(db2,insert,NULL,NULL,&err);
      if(rc!=SQLITE_OK)
     printf("insert error:%s\n",err);
}
char campionate[5000];
char AfisareCampionate()
{
bzero(campionate,5000);
sqlite3_prepare_v2(db2,"SELECT * FROM campionat",-1,&stmt,0);
    int nr_max,nr_curent,day,month,year;
    const unsigned char *Nume_joc,*Reguli,*Modextragere;
    int i=0;
    strcpy(campionate,"");
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        ID=sqlite3_column_int(stmt,0);
        Nume_joc=sqlite3_column_text(stmt,1);
        Reguli=sqlite3_column_text(stmt,2);
        Modextragere=sqlite3_column_text(stmt,3);
        nr_max=sqlite3_column_int(stmt,4);
        nr_curent=sqlite3_column_int(stmt,5);
        day=sqlite3_column_int(stmt,6);
        month=sqlite3_column_int(stmt,7);
        year=sqlite3_column_int(stmt,8);
        // strcpy(sir,("%d.NUME JOC=%s-Reguli:%s-Mod Extragere:%s-Participanti:%d/%d-Full:%d\n",ID,Nume_joc,Reguli,Modextragere,nr_curent,nr_max,FULL));
        inttochar(ID);
        strcat(campionate,sir);
        strcat(campionate,".NUME JOC=");
        strcat(campionate,Nume_joc);
        strcat(campionate,"-Reguli:");
        strcat(campionate,Reguli);
        strcat(campionate,"-Mod Extragere:");
        strcat(campionate,Modextragere);
        
        strcat(campionate,"-Participanti:");
        inttochar(nr_curent);
        strcat(campionate,sir);
        strcat(campionate,"/");
        inttochar(nr_max);
        strcat(campionate,sir);
        strcat(campionate,"-");
        inttochar(day);
        strcat(campionate,sir);
        strcat(campionate,".");
        inttochar(month);
        strcat(campionate,sir);
        strcat(campionate,".");
        inttochar(year);
        strcat(campionate,sir);
        strcat(campionate,"\n");
        i++;
    }
    printf("%s",campionate);
    printf("NR campionate:%d\n",i);
    // print(sir);
    // return sir;

}
int inscris;
void InscriereLaCampionat()
{
    const unsigned char *userI,*emailI;
    int idI;
    inscris=0;
    char comanda[100]="SELECT * FROM inscrieri where id=";
    inttochar(ID);
    strcat(comanda,sir);
    sqlite3_prepare_v2(db2,comanda,-1,&stmt,0);
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        idI=sqlite3_column_int(stmt,0);
        userI=sqlite3_column_text(stmt,1);
        emailI=sqlite3_column_text(stmt,2);
        // printf("\nidI=%d ID=%d\n",idI,ID);
        if(strcmp(user,userI)==0 && idI==ID)
        inscris=1;
        printf("PERSOANE INSCRISE LA CAMPIONAT:%s %s %d\n",userI,emailI,idI);
    }
    // printf("\nINSCRIS=%d\n",inscris);

    if (inscris==0) {
    char sql[100]="UPDATE campionat SET nr_curent=nr_curent+1 WHERE id=";
    inttochar(ID);
    strcat(sql,sir);
    //
    rc=sqlite3_exec(db2,sql,NULL,NULL,&err);
    if(rc!=SQLITE_OK)
    printf("Update error:%s\n",err);
    printf("%s\n",sql);
    
    //strcpy(sql,"INSERT INTO inscrieri VALUES (25,'Alex','alex@yahoo.com')");
    strcpy(sql,"INSERT INTO inscrieri VALUES(");
    inttochar(ID);
    strcat(sql,sir);
    strcat(sql,",'");
    strcat(sql,user);
    strcat(sql,"','");
    strcat(sql,email);
    strcat(sql,"')");
    rc=sqlite3_exec(db2,sql,NULL,NULL,&err);
    
    if(rc!=SQLITE_OK)
    printf("insert inscrieri error:%s\n",err);
    printf("%s\n",sql);
    }
    else printf("Sunteti inscris deja la acest campionat!\n");

}

void verificareFull()
{
    sqlite3_prepare_v2(db2,"SELECT * FROM campionat",-1,&stmt,0);
    int nr_max,nr_curent,day,month,year;
    int ID;
    const unsigned char *Nume_joc,*Reguli,*Modextragere;
    int i=0;

    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        ID=sqlite3_column_int(stmt,0);
        Nume_joc=sqlite3_column_text(stmt,1);
        Reguli=sqlite3_column_text(stmt,2);
        Modextragere=sqlite3_column_text(stmt,3);
        nr_max=sqlite3_column_int(stmt,4);
        nr_curent=sqlite3_column_int(stmt,5);
        day=sqlite3_column_int(stmt,6);
        month=sqlite3_column_int(stmt,7);
        year=sqlite3_column_int(stmt,8);
        

        // i++;
        // printf("%d.NUME JOC=%s-Reguli:%s-Mod Extragere:%s-Participanti:%d/%d\n",ID,Nume_joc,Reguli,Modextragere,nr_curent,nr_max);
        if (nr_max<=nr_curent){
            char sql[300]="DELETE FROM campionat WHERE id=";
            inttochar(ID);
            strcat(sql,sir);
            int rc;
            rc=sqlite3_exec(db2,sql,NULL,NULL,&err);
            if(rc!=SQLITE_OK)
            printf("Delete campionat error:%s\n",err);
            else
            printf("Campionat cu ID=%d sters.\n",ID);
            // Din inscrieri
            const unsigned char *Susername,*Semail;
            strcpy(sql,"SELECT * FROM inscrieri WHERE id=");
            inttochar(ID);
            strcat(sql,sir);
            sqlite3_prepare_v2(db2,sql,-1,&stmt,0);
            char nume[100][100];
            char emails[100][100];
            int k=0;
            while(sqlite3_step(stmt)!=SQLITE_DONE)
            {
                ID=sqlite3_column_int(stmt,0);
                Susername=sqlite3_column_text(stmt,1);
                Semail=sqlite3_column_text(stmt,2);
                strcpy(nume[k],Susername);
                strcpy(emails[k],Semail);
                k++;
                // printf("PERSOANE INSCRISE LA CAMPIONAT:%s %s %d\n",Susername,Semail,ID);
            }
            for (int i=0;i<k-1;i++)
            {
                char aux[100]="";
                int j = rand()%(k-i) + i;
                strcpy(aux,nume[i]);
                strcpy(nume[i],nume[j]);
                strcpy(nume[j],aux);
                strcpy(aux,emails[i]);
                strcpy(emails[i],emails[j]);
                strcpy(emails[j],aux);
            
            }
            char runde[5000]="";
            char details[1000]="";
            strcat(runde,"Nume joc: ");
            strcat(runde,Nume_joc);
            strcat(runde,"\n");
            strcat(runde,"Reguli: ");
            strcat(runde,Reguli);
            strcat(runde,"\n");
            strcat(runde,"Mod extragere: ");
            strcat(runde,Modextragere);
            strcat(runde,"\n");
            // detalii
            strcpy(details,runde);
            strcat(runde,"Data: ");
            inttochar(day);
            strcat(runde,sir);
            strcat(runde,".");
            inttochar(month);
            strcat(runde,sir);
            strcat(runde,".");
            inttochar(year);
            strcat(runde,sir);
            strcat(runde,"\n");
            for (int i=0;i<k;i++)
            printf("%d-%s-%s\n",ID,nume[i],emails[i]);
            int hour=10;
            int minutes=0;
            for (int i=0;i<k;i=i+2)
            {
                strcat(runde,nume[i]);
                strcat(runde," vs ");
                strcat(runde,nume[i+1]);
                strcat(runde," - ");
                if (minutes==60)
                {minutes=0;
                hour+=1;
                }
                inttochar(hour);
                strcat(runde,sir);
                strcat(runde,":");
                if (minutes==0)
                strcat(runde,"00");
                else{
                inttochar(minutes);
                strcat(runde,sir);
                }
                strcat(runde,"\n");
                minutes+=30;
            }
            // printf("%d-%s-%s\n",ID,nume[i],emails[i]);
            for (int i=0;i<k;i++)
            {
                FILE *fpmail=fopen("mail.txt","w");
                fprintf(fpmail,"To: %s\n",emails[i]);
                fprintf(fpmail,"From: Championship\n");
                fprintf(fpmail,"Subject: Draw\n\n");
                fprintf(fpmail,"%s\n",runde);
                fclose(fpmail);
                system("sendmail -t < mail.txt");

            }
            // printf("%s",runde);
            int nrmeci=1;
            day=day+1;
                if(day>=29){
                    day=1;
                    month+=1;
                }
                if(month>=13){
                    month=1;
                    year=year+1;
                }
            for (int i=0;i<k;i=i+2)
            {
                strcpy(sql,"INSERT INTO admin VALUES(");
                inttochar(ID);
                strcat(sql,sir);
                strcat(sql,"0");
                inttochar(nrmeci);
                strcat(sql,sir);
                strcat(sql,",'");
                strcat(sql,nume[i]);
                strcat(sql,"','");
                strcat(sql,nume[i+1]);
                strcat(sql,"',0,0,0,'");
                strcat(sql,emails[i]);
                strcat(sql,"','");
                strcat(sql,emails[i+1]);
                strcat(sql,"','");
                strcat(sql,details);
                strcat(sql,"',");
                inttochar(ID);
                strcat(sql,sir);
                strcat(sql,",");
                
                inttochar(day);
                strcat(sql,sir);
                strcat(sql,",");
                inttochar(month);
                strcat(sql,sir);
                strcat(sql,",");
                inttochar(year);
                strcat(sql,sir);
                strcat(sql,")");
                rc=sqlite3_exec(db2,sql,NULL,NULL,&err);
                if(rc!=SQLITE_OK)
                printf("insert inscrieri error:%s\n",err);
                printf("%s\n",sql);
                nrmeci++;
            }
            break;
        }

    }
}

char campionateAdmin[1000];
void sendToAdmin(){
    sqlite3_prepare_v2(db2,"SELECT * FROM admin WHERE jucat=0",-1,&stmt,0);
    int idmeci,idcamp,scor1,scor2,jucat,day,month,year;
    const unsigned char *user1,*user2,*email1,*email2,*descriere;
    strcpy(campionateAdmin,"\n");
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        idmeci=sqlite3_column_int(stmt,0);
        user1=sqlite3_column_text(stmt,1);
        user2=sqlite3_column_text(stmt,2);
        scor1=sqlite3_column_int(stmt,3);
        scor2=sqlite3_column_int(stmt,4);
        jucat=sqlite3_column_int(stmt,5);
        email1=sqlite3_column_text(stmt,6);
        email2=sqlite3_column_text(stmt,7);
        descriere=sqlite3_column_text(stmt,8);
        idcamp=sqlite3_column_int(stmt,9);
        day=sqlite3_column_int(stmt,10);
        month=sqlite3_column_int(stmt,11);
        year=sqlite3_column_int(stmt,12);
        // CREATE MESAJ
        inttochar(idmeci);
        strcat(campionateAdmin,sir);
        strcat(campionateAdmin,". Campionat ");
        inttochar(idcamp);
        strcat(campionateAdmin,sir);
        strcat(campionateAdmin,": ");
        strcat(campionateAdmin,user1);
        strcat(campionateAdmin," vs ");
        strcat(campionateAdmin,user2);
        strcat(campionateAdmin," - ");
        inttochar(day);
        strcat(campionateAdmin,sir);
        strcat(campionateAdmin,".");
        inttochar(month);
        strcat(campionateAdmin,sir);
        strcat(campionateAdmin,".");
        inttochar(year);
        strcat(campionateAdmin,sir);
        strcat(campionateAdmin,"\n");
        printf("\nADMIN:%s \n",campionateAdmin);
        // printf("%d.%d- %s vs %s - %d-%d  -  jucat:%d, email1: %s, email2:%s, descriere:%s\n",idmeci,idcamp,user1,user2,scor1,scor2,jucat,email1,email2,descriere);
    }
}


void editScores(int idmeci,int scor1,int scor2){
    char sql[100]="UPDATE admin SET scor1=";//scor1+1 WHERE idmeci=";
    inttochar(scor1);
    strcat(sql,sir);
    strcat(sql,", scor2=");
    inttochar(scor2);
    strcat(sql,sir);
    strcat(sql,", jucat=1");
    strcat(sql," WHERE id=");
    inttochar(idmeci);
    strcat(sql,sir);
    printf("\n%s\n",sql);
    int rc=sqlite3_exec(db2,sql,NULL,NULL,&err);
    if(rc!=SQLITE_OK)
    printf("Update admin error:%s\n",err);
}

void adminPanel(){
    int maxid=getMaxID();
    char sql[100]="";
    for (int i=1;i<=maxid;i++)
    {
        strcpy(sql,"SELECT * FROM admin WHERE idcamp=");
        inttochar(i);
        strcat(sql,sir);
        sqlite3_prepare_v2(db2,sql,-1,&stmt,0);
    int idmeci,idcamp,scor1,scor2,jucat,day,month,year;
    const unsigned char *user1,*user2,*email1,*email2,*descriere;
    int jucate=1;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        idmeci=sqlite3_column_int(stmt,0);
        user1=sqlite3_column_text(stmt,1);
        user2=sqlite3_column_text(stmt,2);
        scor1=sqlite3_column_int(stmt,3);
        scor2=sqlite3_column_int(stmt,4);
        jucat=sqlite3_column_int(stmt,5);
        email1=sqlite3_column_text(stmt,6);
        email2=sqlite3_column_text(stmt,7);
        descriere=sqlite3_column_text(stmt,8);
        day=sqlite3_column_int(stmt,10);
        month=sqlite3_column_int(stmt,11);
        year=sqlite3_column_int(stmt,12);

        if(jucat==0){
        jucate=0;
        break;
        }
        // printf("%d.%d- %s vs %s - %d-%d  -  jucat:%d, email1: %s, email2:%s, descriere:%s\n",idmeci,idcamp,user1,user2,scor1,scor2,jucat,email1,email2,descriere);
    }
    if(jucate==1)
    {
        strcpy(sql,"SELECT * FROM admin WHERE idcamp=");
        inttochar(i);
        strcat(sql,sir);
        sqlite3_prepare_v2(db2,sql,-1,&stmt,0);
    int idmeci,idcamp,scor1,scor2,jucat;
    const unsigned char *user1,*user2,*email1,*email2,*descriere;
    char details[1000];
    int jucate=1;
    char users[100][100];
    char emails[100][100];
    int usercount=0;
    while(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        idmeci=sqlite3_column_int(stmt,0);
        user1=sqlite3_column_text(stmt,1);
        user2=sqlite3_column_text(stmt,2);
        scor1=sqlite3_column_int(stmt,3);
        scor2=sqlite3_column_int(stmt,4);
        jucat=sqlite3_column_int(stmt,5);
        email1=sqlite3_column_text(stmt,6);
        email2=sqlite3_column_text(stmt,7);
        descriere=sqlite3_column_text(stmt,8);
        day=sqlite3_column_int(stmt,10);
        month=sqlite3_column_int(stmt,11);
        year=sqlite3_column_int(stmt,12);

        strcpy(details,descriere);
        if(scor1>=scor2){
        strcpy(users[usercount],user1);
        strcpy(emails[usercount],email1);
        }
        else{
        strcpy(users[usercount],user2);
        strcpy(emails[usercount],email2);
        }
        usercount++;
        // printf("%d.%d- %s vs %s - %d-%d  -  jucat:%d, email1: %s, email2:%s, descriere:%s\n",idmeci,idcamp,user1,user2,scor1,scor2,jucat,email1,email2,descriere);
    }

    // DELETE OLD MATCHES 
    char sql[500]="DELETE FROM admin WHERE idcamp=";
            inttochar(i);
            strcat(sql,sir);
            int rc;
            rc=sqlite3_exec(db2,sql,NULL,NULL,&err);
            if(rc!=SQLITE_OK)
            printf("Delete matches error:%s\n",err);
    // INSERT NEW ONES IF THE CHAMPIONSHIP IS NOT OVER
    if(usercount!=1){
    int nrmeci=1;
    char runde[1000];
    int hour=10;
    int minutes=30;
    for (int k=0;k<usercount;k=k+2)
            {
                strcpy(sql,"INSERT INTO admin VALUES(");
                inttochar(i);
                strcat(sql,sir);
                strcat(sql,"0");
                inttochar(nrmeci);
                strcat(sql,sir);
                strcat(sql,",'");
                strcat(sql,users[k]);
                strcat(sql,"','");
                strcat(sql,users[k+1]);
                strcat(sql,"',0,0,0,'");
                strcat(sql,emails[k]);
                strcat(sql,"','");
                strcat(sql,emails[k+1]);
                strcat(sql,"','");
                strcat(sql,details);
                strcat(sql,"',");
                inttochar(i);
                strcat(sql,sir);
                strcat(sql,",");
                day=day+1;
                if(day>=29){
                    day=1;
                    month+=1;
                }
                if(month>=13){
                    month=1;
                    year=year+1;
                }
                inttochar(day);
                strcat(sql,sir);
                strcat(sql,",");
                inttochar(month);
                strcat(sql,sir);
                strcat(sql,",");
                inttochar(year);
                strcat(sql,sir);
                strcat(sql,")");
                rc=sqlite3_exec(db2,sql,NULL,NULL,&err);
                if(rc!=SQLITE_OK)
                printf("insert inscrieri error:%s\n",err);
                printf("%s\n",sql);

                strcat(runde,users[k]);
                strcat(runde," vs ");
                strcat(runde,users[k+1]);
                strcat(runde," - ");
                if (minutes==60)
                {minutes=0;
                hour+=1;
                }
                inttochar(hour);
                strcat(runde,sir);
                strcat(runde,":");
                if (minutes==0)
                strcat(runde,"00");
                else{
                inttochar(minutes);
                strcat(runde,sir);
                }
                strcat(runde,"\n");
                minutes+=30;
                
                nrmeci++;
            }
            strcat(details,"\n");
            inttochar(day);
            strcat(details,sir);
            strcat(details,".");
            inttochar(month);
            strcat(details,sir);
            strcat(details,".");
            inttochar(year);
            strcat(details,sir);
            strcat(details,"\n");
            strcat(details,runde);
    for (int k=0;k<usercount;k++){
        FILE *fpmail=fopen("mail.txt","w");
        fprintf(fpmail,"To: %s\n",emails[k]);
        fprintf(fpmail,"From: Championship\n");
        fprintf(fpmail,"Subject: Draw\n\n");
        fprintf(fpmail,"%s\n",details);
        fclose(fpmail);
        system("sendmail -t < mail.txt");
    }
    
    }
    else {
        printf("CASTIGATOR: %s - %s",users[0],emails[0]);
        char winner[100]="Castigator: ";
        strcat(winner,users[0]);
        strcat(winner,"\nFelicitari!");
        FILE *fpmail=fopen("mail.txt","w");
        fprintf(fpmail,"To: %s\n",emails[0]);
        fprintf(fpmail,"From: Championship\n");
        fprintf(fpmail,"Subject: Draw\n\n");
        fprintf(fpmail,"%s\n",winner);
        fclose(fpmail);
        system("sendmail -t < mail.txt");
    }

    }

    }
}



int main ()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;	
    char usernamerasp[100]=" ";        //mesaj de raspuns pentru client
    int sd;			//descriptorul de socket
	int key; 		//valoarea din meniu
	int loginOK=0, registerOK;
    
// 	char *err;
// sqlite3 *db2;
// sqlite3_stmt *stmt;
if (sqlite3_open("Mydb.db", &db2) == SQLITE_OK)
        printf("Opened db successfully\n");
    else
        printf("Failed to open db\n");
rc=sqlite3_exec(db2,"CREATE TABLE IF NOT EXISTS campionat(id INT,nume_joc varchar(100),reguli varchar(100),mod_extragere varchar(100),nr_maxim INT,nr_curent INT,ziua INT,luna INT,an INT)",NULL,NULL,&err);
    if(rc!=SQLITE_OK)
      printf("create error:%s\n",err);
      
      rc=sqlite3_exec(db2,"CREATE TABLE IF NOT EXISTS inscrieri(id INT, username varchar(100),email varchar(100))",NULL,NULL,&err);
      if(rc!=SQLITE_OK)
       printf("create inscrieri error:%s\n",err);
     rc=sqlite3_exec(db2,"CREATE TABLE IF NOT EXISTS admin(id INT, username1 varchar(100),username2 varchar(100),scor1 INT,scor2 INT,jucat INT,email1 varchar(100),email2 varchar(100),descriere varchar(1000),idcamp INT,zi INT, luna INT, an INT)",NULL,NULL,&err);
    if(rc!=SQLITE_OK)
    printf("create admin error:%s\n",err);
    /* crearea unui socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
    	perror ("[server]Eroare la socket().\n");
    	return errno;
    }

    /* pregatirea structurilor de date */
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons (PORT);

    /* atasam socketul */
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
    	perror ("[server]Eroare la bind().\n");
    	return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (sd, 1) == -1)
    {
    	perror ("[server]Eroare la listen().\n");
    	return errno;
    }

    /* servim in mod concurent clientii... */
    while (1)
    {
    	int client;
    	int length = sizeof (from);

    	printf ("[server]Asteptam la portul %d...\n",PORT);
    	fflush (stdout);

    	/* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    	client = accept (sd, (struct sockaddr *) &from, &length);

    	/* eroare la acceptarea conexiunii de la un client */
    	if (client < 0)
    	{
    		perror ("[server]Eroare la accept().\n");
    		continue;
    	}

    	int pid;
    	if ((pid = fork()) == -1) {
    		close(client);
    		continue;
    	} else if (pid > 0) {
    		// parinte
    		close(client);
    		while(waitpid(-1,NULL,WNOHANG));
    		continue;
    	} else if (pid == 0) {
    		// copil
    		close(sd);

    		/* s-a realizat conexiunea, se astepta mesajul */
        while (1)
    	if(loginOK==0)
			{
            bzero(&key,sizeof(int));
            fflush(stdout);
			if (read (client, &key, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
    		else
			{
            if(key==1){
			bzero (username, 100);
    		printf ("[server]Asteptam mesajul...\n");
    		fflush (stdout);

    		/* citirea mesajului */
    		if (read (client, username, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
			else
    		printf ("[server]Mesajul a fost receptionat...%s\n", username);
            strcpy(user,"");
            strncat(user,username,strlen(username)-1);
            printf("\n-USERNAME PRIMIT:%s-\n",user);
            bzero (password, 100);
    		printf ("[server]Asteptam mesajul...\n");
    		fflush (stdout);

    		/* citirea mesajului */
    		if (read (client, password, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
			else
    		printf ("[server]Mesajul a fost receptionat...%s\n", password);
       
	FILE *fpuser;
    FILE *fppass;
    FILE *fpadmin;
    char line_u[256],line_p[256],line_a[256];
    fpuser=fopen("username.txt","a+");
    fppass=fopen("password.txt","a+");
    fpadmin=fopen("admin.txt","a+");
		 
		 
    int ucount=0,pcount=0,acount=0;
    
    while(fgets(line_u,sizeof(line_u),fpuser) )
    {
        if(strncmp(line_u,username,strlen(line_u)-2)==0 )
        {
            while(fgets(line_p,sizeof(line_p),fppass)) {
                if(pcount>ucount) break;
                if(strncmp(line_p,password,strlen(line_p)-2)==0 && pcount==ucount) 
            {
                printf("\nAm gasit combo\n");
                while(fgets(line_a,sizeof(line_a),fpadmin)) {
                    if(acount==pcount){
                if(strncmp(line_a,"0",strlen(line_a)-1)==0) {
                    loginOK=1;
                    // printf("Login user");
                    ucount=0;
                    pcount=0;
                    acount=0;
                    char aux[100];
                    strncpy(aux,username,strlen(username)-1);
                    strcpy(username,aux);
                    // printf("\nUSERNAME:%s\n",username);

                    break;
                }
                else
                {
                    loginOK=2;
                    // printf("Login admin");
                    ucount=0;
                    pcount=0;
                    acount=0;
                    char aux[100];
                    strncpy(aux,username,strlen(username)-1);
                    strcpy(username,aux);
                    // printf("\nUSERNAME:%s\n",username);
                    break;
                }
                    }
                    acount++;
                    }
            }
                    pcount++;
        }
        //printf("%s",line_u);
        }
        ucount++;
    }
    if(loginOK==0) printf("[Server]Login esuat");
    else printf("[Server]Login Reusit!");
    fclose(fpuser);
    fclose(fppass);
    fclose(fpadmin);
			

    		/*pregatim mesajul de raspuns */
    		//bzero(&loginOK,sizeof(int));


    		// strcat(usernamerasp,"Hello ");
    		// strcat(usernamerasp,username);

    		/* returnam mesajul clientului */
    		if (write (client, &loginOK, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf ("[server]Mesajul a fost trasmis cu succes.\n");
			}
    else if(key==2){
                bzero (username, 100);
    		printf ("[server]Asteptam mesajul...\n");
    		fflush (stdout);

    		/* citirea mesajului */
    		if (read (client, username, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
			else
    		printf ("[server]Mesajul a fost receptionat...%s\n", username);

            bzero (password, 100);
    		printf ("[server]Asteptam mesajul...\n");
    		fflush (stdout);

    		/* citirea mesajului */
    		if (read (client, password, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
			else
    		printf ("[server]Mesajul a fost receptionat...%s\n", password);
            FILE *fpuser;
            FILE *fppass;
            FILE *fpadmin;
            char line_u[256],line_p[256],line_a[256];
            fpuser=fopen("username.txt","a+");
            fppass=fopen("password.txt","a+");
            fpadmin=fopen("admin.txt","a+");
            registerOK=1;
            // printf("\nregisterOK:%d\n",registerOK);
            while(fgets(line_u,sizeof(line_u),fpuser))
            {
                if(strncmp(line_u,username,strlen(line_u)-2)==0){
                    // printf("compare users:\n%s-%s\n",line_u,username);
                    registerOK=0;
                }
            }
            if(registerOK==0) printf("Username existent");
            else if(registerOK==1){
            fprintf(fpuser,"%s",username);
            fprintf(fppass,"%s",password);
            fprintf(fpadmin,"0\n");
            }
            fclose(fpuser);
            fclose(fppass);
            fclose(fpadmin);
            printf("\nregisterOK:%d\n",registerOK);
            /* returnam mesajul clientului */
    		if (write (client, &registerOK, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf ("[server]Mesajul a fost trasmis cu succes.\n");
            }
            else if(key==3){
                close(client);
                break;
            }
			/*    key     */
			
			// printf("Am iesit din while");
            }
            }
            else if(loginOK==1){
                // printf("Suntem in while");
            bzero(&key,sizeof(int));
            fflush(stdout);
			if (read (client, &key, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
    		else
			{
                //  printf("SUNTEM IN WHILE,key este %d",key);
				if(key==1)
				{
                    //  printf("SUNTEM IN WHILE,key este %d",key);
                
                //inregistrareCampionat();
				
                //printf("[Server]Campionat inregistrat cu succes");

            //     bzero(&ID,sizeof(int));
            //     fflush(stdout);
            //     if (read (client, &ID, sizeof(int)) <= 0)
    		// {
    		// 	perror ("[server]Eroare la read() de la client.\n");
    		// 	close (client);	/* inchidem conexiunea cu clientul */
    		// 	continue;		/* continuam sa ascultam */
    		// }
            bzero(numejoc,100);
    fflush(stdout);
            if (read (client, numejoc, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
            // printf("NUME JOC:%s\n",numejoc);
            bzero(reguli,100);
    fflush(stdout);
            if (read (client, reguli, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
            if (read (client, modextragere, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
            bzero(&nrmax,sizeof(int));
    fflush(stdout);
            if (read (client, &nrmax, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
             
             bzero(&ziua,sizeof(int));
    fflush(stdout);
            if (read (client, &ziua, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
             bzero(&luna,sizeof(int));
    fflush(stdout);
            if (read (client, &luna, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
             bzero(&an,sizeof(int));
    fflush(stdout);
            if (read (client, &an, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
            ID=genID();
            nrcurent=0;
            // full=0;
    //         bzero(&nrcurent,sizeof(int));
    // fflush(stdout);
    //          if (read (client, &nrcurent, sizeof(int)) <= 0)
    // 		{
    // 			perror ("[server]Eroare la read() de la client.\n");
    // 			close (client);	/* inchidem conexiunea cu clientul */
    // 			continue;		/* continuam sa ascultam */
    // 		}
    //         bzero(&full,sizeof(int));
    // fflush(stdout);
    //          if (read (client, &full, sizeof(int)) <= 0)
    // 		{
    // 			perror ("[server]Eroare la read() de la client.\n");
    // 			close (client);	/* inchidem conexiunea cu clientul */
    // 			continue;		/* continuam sa ascultam */
    // 		}
            
            InregistrareCampionat();
            AfisareCampionate();
            printf("[Server]Campionat inregistrat cu succes\n");
            
                }
            else if(key==2)
            {   
                AfisareCampionate();
                // char campionate[1000];
                // bzero(campionate,1000);
                if (write (client, campionate, 5000) <= 0)
    		{
    			perror ("[server]Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf ("[server]Mesajul a fost trasmis cu succes.\n");
			

                //  printf("SUNTEM IN WHILE,key este %d",key);
                  bzero(&ID,sizeof(int));
                 fflush(stdout);
                 if (read (client, &ID, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
           
                // printf("ID este:%d",ID);
                if(ID!=0){
             bzero(email,100);
            fflush(stdout);
            if (read (client, email, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
                InscriereLaCampionat();
                verificareFull();
                AfisareCampionate();
            if (write (client, &inscris, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf ("[server]Mesajul a fost trasmis cu succes.\n");
                }
        

            }
            else if(key==3) {
                loginOK=0;
                if (write (client, &loginOK, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf ("[server]Mesajul a fost trasmis cu succes.\n");
            }
            else if(key==4) {
                close(client);
                break;
            }
			// 	else if(key==3)
            //     {   close (client);
            //         if (read (client, &key, sizeof(int)) <= 0)
    		// {
    		// 	perror ("[server]Eroare la write() catre client.\n");
    		// 	continue;		/* continuam sa ascultam */
    		// }
            //         //printf("SUNTEM IN WHILE,key este %d",key);
                    
                    
            //     } 
                
			}
            }
            else if(loginOK==2){
                bzero(&key,sizeof(int));
            fflush(stdout);
			if (read (client, &key, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
    		else
			{
                //  printf("SUNTEM IN WHILE,key este %d",key);
				if(key==1)
				{
                    //  printf("SUNTEM IN WHILE,key este %d",key);
                
                //inregistrareCampionat();
				
                //printf("[Server]Campionat inregistrat cu succes");

            //     bzero(&ID,sizeof(int));
            //     fflush(stdout);
            //     if (read (client, &ID, sizeof(int)) <= 0)
    		// {
    		// 	perror ("[server]Eroare la read() de la client.\n");
    		// 	close (client);	/* inchidem conexiunea cu clientul */
    		// 	continue;		/* continuam sa ascultam */
    		// }
            bzero(numejoc,100);
    fflush(stdout);
            if (read (client, numejoc, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
            // printf("NUME JOC:%s\n",numejoc);
            bzero(reguli,100);
    fflush(stdout);
            if (read (client, reguli, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
            if (read (client, modextragere, 100) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
            bzero(&nrmax,sizeof(int));
    fflush(stdout);
            if (read (client, &nrmax, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
             
             bzero(&ziua,sizeof(int));
    fflush(stdout);
            if (read (client, &ziua, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
             bzero(&luna,sizeof(int));
    fflush(stdout);
            if (read (client, &luna, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
             bzero(&an,sizeof(int));
    fflush(stdout);
            if (read (client, &an, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
            ID=genID();
            nrcurent=0;
            // full=0;
    //         bzero(&nrcurent,sizeof(int));
    // fflush(stdout);
    //          if (read (client, &nrcurent, sizeof(int)) <= 0)
    // 		{
    // 			perror ("[server]Eroare la read() de la client.\n");
    // 			close (client);	/* inchidem conexiunea cu clientul */
    // 			continue;		/* continuam sa ascultam */
    // 		}
    //         bzero(&full,sizeof(int));
    // fflush(stdout);
    //          if (read (client, &full, sizeof(int)) <= 0)
    // 		{
    // 			perror ("[server]Eroare la read() de la client.\n");
    // 			close (client);	/* inchidem conexiunea cu clientul */
    // 			continue;		/* continuam sa ascultam */
    // 		}
            
            InregistrareCampionat();
            AfisareCampionate();
            printf("[Server]Campionat inregistrat cu succes\n");
                }
            else if(key==2) {
                sendToAdmin();
                // char campionate[1000];
                // bzero(campionate,1000);
                if (write (client, campionateAdmin, 1000) <= 0)
    		{
    			perror ("[server]Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf ("[server]Mesajul a fost trasmis cu succes.\n");
			

                //  printf("SUNTEM IN WHILE,key este %d",key);
                  bzero(&idmeci,sizeof(int));
                 fflush(stdout);
                 if (read (client, &idmeci, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidmeciem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
                // printf("ID este:%d",idmeci);
                if(idmeci!=0){
                bzero(&scor1,sizeof(int));
                 fflush(stdout);
                 if (read (client, &scor1, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
                // printf("scor1 este:%d",scor1);

                bzero(&scor2,sizeof(int));
                 fflush(stdout);
                 if (read (client, &scor2, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}
                // printf("scor2 este:%d",scor2);
                editScores(idmeci,scor1,scor2);
                adminPanel();
                }
            }
                else if(key==3) {
                loginOK=0;
                if (write (client, &loginOK, sizeof(int)) <= 0)
    		{
    			perror ("[server]Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf ("[server]Mesajul a fost trasmis cu succes.\n");
            }
                else if (key==4) {
                    close(client);
                    break;
                }
                }
            }

    		/* am terminat cu acest client, inchidem conexiunea */
    		//close (client);
    		exit(0);
        }
        
    }				/* while */
}				/* main */