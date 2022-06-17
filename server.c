#include<stdlib.h>
#include<stdio.h>
#include <dirent.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>

int flag1=0;

//================================   FUNCTION PROTOTYPES   ==============================================


int checker(FILE *fp, char *temp_username,char *username,char *temp_password,char *password);

int  verifier_mod1(FILE *fp1, char *username);

int verifier(char *username);

void fileCreator(char *to, char *subject, char *body, char *from);

char* transform(char *username);

int countfile(char *username);

char* file_name(char *username, int option);









//=======================================================================================================



void main()
{
	
	int sockfd, fd1, length, i, optionw1, flag, optionw2, infow2, option;
	char buf[100];
	char *username, *to, *from, *subject, *body;
	
	username = malloc(50);
	to = malloc(50);
	from = malloc(50);
	subject = malloc(50);
	body = malloc(200);
	
	char *password;
	password = (char*)malloc(sizeof(char)*50);
	
	char *temp_username;
	temp_username = (char*)malloc(sizeof(char)*50);
	
	char *temp_password;
	temp_password = (char*)malloc(sizeof(char)*50);
	
	
	struct sockaddr_in sa_addr,cl_addr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);


   sa_addr.sin_family=AF_INET;
   sa_addr.sin_addr.s_addr=INADDR_ANY;
   sa_addr.sin_port=htons(6010);
   memset(sa_addr.sin_zero, '\0', sizeof sa_addr.sin_zero);


   i=bind(sockfd,(struct sockaddr *)&sa_addr,sizeof(sa_addr));
   printf("test %d%d\n",sockfd,i);

   listen(sockfd,5);


	length=sizeof(cl_addr);
	fd1=accept(sockfd, (struct sockaddr *) &cl_addr,&length);
   recv(fd1, &optionw1, sizeof(int), 0);
		
		switch(optionw1)
		{
			case 1:
			{
				FILE *fp;
				recv(fd1, username, 50, 0);
				recv(fd1, password, 50, 0);
				fp= fopen("log.txt","r");
				
				
				
				flag=checker(fp, temp_username, username, temp_password, password);
				if(flag == 1)
				{
					printf("\nUsername and password has been verified \n");
					infow2=4;
					send(fd1, &infow2, sizeof(int), 0);
					recv(fd1, &optionw2, sizeof(int), 0);
					
					switch(optionw2)
					{
						case 4:                                           //to receive a message
						{
							recv(fd1, to, 50, 0);
							recv(fd1, from, 50, 0);
							recv(fd1, subject, 50, 0);
							recv(fd1, body, 200, 0);
							
							fileCreator(to, subject, body, from);
							break;
						
						}
						
						case 5:
						{
								int count=0;
								FILE *fp;		
								count = countfile(username);
								send(fd1, &count, sizeof(int), 0);
								
								DIR *d;
  
  								struct dirent *dir;
  								char *username1;
  								char *filepath; 
  								char *path;
  								char ch;
 							   path = malloc(sizeof(char)*50);
 							   filepath = malloc(sizeof(char)*50);
  								username1 = malloc(sizeof(char)*100);
  								strcpy(path,"/home/kiit/project/");
  								strcat(path,transform(username));
  
  								
  								int i=1, temp, choice, k;
  								//int count=0;
                        
                        d = opendir(path);
                        
                        if (d== NULL)
  								{
   								printf("could not open current directory");
   								send(fd1, "could not open current directory", 50, 0); 
 								}
  								else
  								{  
    								
    								while ((dir = readdir(d)) != NULL)
    								{
      
     								
      								if(strcmp(dir->d_name,"..")==0|| strcmp(dir->d_name, ".")==0)
      								continue;
      
      
     									send(fd1, dir->d_name, 50, 0);
                              i++;
      
    							}
	 							
    							closedir(d);
  							}
//============================================================================================
  							
  							recv(fd1, &option, sizeof(int), 0);
  							printf("%d", option);
							strcpy(filepath, file_name(username, option));
							
							printf("\n%s", filepath);  							
  							
  								
  							fp = fopen(filepath,"r");
  							
  							while((ch=getc(fp))!=EOF)
  							{
  								count++;
  							}
  							
  							//printf("%d", count);
  							send(fd1, &count, 4, 0);
  							
  							rewind(fp);
  							
  							/*while((ch=getc(fp))!=EOF)
  							{
  								send(fd1, &ch, 1, 0);
  							}*/
  							ch= getc(fp);
  							for(k=0; k<count; k++)
  							{
  								send(fd1, &ch, 1, 0);
  								ch= getc(fp);
  							}
  							
  							fclose(fp);
  						}
						
					}
				}
				else
				{
					printf("\nUsername and password has not been verified \n");
					optionw2=5;
					send(fd1, &optionw2, sizeof(int), 0);
					
					
				}
				fclose(fp);
				break;
			}
			
			case 2:
			{
				int pivot;
				int i,j;
				FILE *fp2;
				recv(fd1, username, 50, 0);
				recv(fd1, password, 50, 0);
				
				pivot = verifier(username);
				fp2= fopen("log.txt","a+");
				if(pivot == 1)
				{
					exit(0);
				}
				else
				{
					i=0;
					j=0;
	
					while(*(username+i) != '\0')
					{
						
						putc(*(username+i), fp2);
						i++;
					}
					putc(':', fp2);
					while(*(password+j)!='\0')
					{
					putc(*(password+j), fp2);
					j++;
					}
					putc('|',fp2);
					printf("\nNew account added\n");
				}
				fclose(fp2);	
			}
}		
	close(fd1);

}



//======================================  FUNCTION BODY  ================================================


int checker(FILE *fp, char *temp_username,char username[50],char *temp_password,char password[50])
{
	int i=0, j=0,m , k;
	char *temp_username1;
	
	temp_username1= (char*)malloc(sizeof(char)*50);
	
	
	char ch;
	while((ch=getc(fp))!=':')
	{
		
		if(ch!=EOF)
		{
			*(temp_username+i)=ch;
			i++;
			
		}
		else
		{
			
			return 0;
		
		}
	}
		
	while((ch=getc(fp))!='|')
	{
		if(ch!=EOF)
		{
			*(temp_password+j)=ch;
			j++;
			
		}
		else
		{
			return 0;
		}
	}
	
	for(m=0; m<50; m++)
	{
			*(temp_username1+m)= '\0';
	}
	
	
	
	int l=strlen(temp_username);
	
	
	for(m=0; m<=l; m++)
	{
		*(temp_username1+m)=*(temp_username+m);
	}	
		
	printf("%s",temp_username1);
	
	
	if((strcmp(temp_username1, username))==0 && (strcmp(temp_password, password))==0)
	{
		return 1;
		
	}

		
		int temp;
		
		for(temp=0; temp<50; temp++)
		{
			*(temp_username+temp)= '\0';
		}
		for(temp=0; temp<50; temp++)
		{
			*(temp_password+temp)= '\0';
		}
		
		
		k=checker(fp, temp_username, username, temp_password, password);
			
		return k;	

	
	
}

int  verifier_mod1(FILE *fp1, char *username)
{
	int i=0,k,j, length,l;
	char *temp_username;
	temp_username = (char*)malloc(sizeof(char)*50);
	
	char ch;
	
	while((ch=getc(fp1)) != ':')
		{
			if(ch!=EOF)
			{
				*(temp_username+i)= ch;
				i++;
			}
			else
			{
				return 0;
			}
		}
		
		if(strcmp(temp_username,username)==0)
		{
			 printf("-----%d------\n",strcmp(temp_username,username));
			 printf("hell : %s\n", temp_username);
			 return 1;
		}
		
		length=strlen(temp_username);
		
		for(l=0; l<=(length-2); l++)
		{
			*(temp_username+l)=*(temp_username+l);
		}
		
		if(strcmp(temp_username,username)==0)
		{
			 printf("-----%d------\n",strcmp(temp_username,username));
			 printf("hell : %s\n", temp_username);
			 return 1;
		}
		
		while((ch=getc(fp1))!='|')
		{
			if(ch==EOF)
			{
			 	return 0;
			}
		}
		for(i=0; i<50; i++)
		{
			*(temp_username+i)= '\0';
		}
			
		j=verifier_mod1(fp1, username);
		return j;
}






int verifier(char *username)
{
	FILE *fp1;
	int i=0, j, k;
	char ch;
	int flag;
	fp1=fopen("log.txt", "r");
	
	flag = verifier_mod1(fp1, username);
	if(flag==1)
	{
		printf("\nUsername is in use (change it)\n");
		return 1;
	}
	else
	{
		return 0;
	}
	fclose(fp1);
}

void fileCreator(char *to, char *subject, char *body, char *from)
{
	FILE *fp;
	time_t t;
	char c;
	char  *file_dest, *dir_dest;
	
	file_dest = (char*)malloc(sizeof(char)*100);
	dir_dest= (char*)malloc(sizeof(char)*100);
	
	
	int i,j,k, length;
	char *temp;
	
	time(&t);
	
		
	i=1;
	
	while(*(to+i)!='@')
	{
		i++;
	}
	
	temp=(char*)malloc(sizeof(char)*i);
	i=0;
	
	while(*(to+i)!='@')
	{
		*(temp+i)=*(to+i);
		
		i++;
	}
	
	strcpy(dir_dest, "/home/kiit/project/");
	strcat(dir_dest, temp);
	
	
	strcpy(file_dest, "/home/kiit/project/");
	strcat(file_dest, temp);
	strcat(file_dest, "/");
	strcat(file_dest, subject);
	strcat(file_dest, "@");	
	strcat(file_dest, ctime(&t));
	strcat(file_dest, ".txt");
	
	struct stat st= {0};
	
	if(stat(dir_dest, &st) == -1)
	{
		mkdir(dir_dest, 0700);
	}
	
	fp= fopen(file_dest, "w");
//======================================================	
	fputc('|', fp);
	
	for(i=0; *(from+i)!='\0'; i++)
	{
		fputc(*(from+i),fp);
	}	
	fputc('|', fp);
	
	for(i=0; *(subject+i)!='\0'; i++)
	{
		fputc(*(subject+i),fp);
	}	
	fputc('|', fp);
	
	for(i=0; *(body+i)!='\0'; i++)
	{
	 	fputc(*(body+i),fp);
   }
   fputc('|', fp);
	
	fclose(fp); 
}


char* transform(char *username)
{
	int i;
	char *username1;
	username1=(char*)malloc(sizeof(char)*50);
	memset( username1, '\0', 50 );
	for(i=0; *(username+i)!='@'; i++)
	{
		*(username1+i)=*(username+i);
	}
	return username1;
}

int countfile(char *username)
{
  DIR *d;
  struct dirent *dir;
  char *path;
  int i;
  
  path=malloc(sizeof(char)*50);
  
  strcpy(path,"/home/kiit/project/");
  strcat(path,transform(username));
  
  d = opendir(path);
  if (d == NULL)
  {
  		printf("could not open current directory");
  }
  else
  {
  		
  	 int i=1;	
    while ((dir = readdir(d)) != NULL)
    {
      
      if(strcmp(dir->d_name,"..")==0|| strcmp(dir->d_name, ".")==0)
      continue;
      
      i=i+1;
      
    }

     closedir(d);
     return (i-1);
  }
  
}


char* file_name(char *username, int option)
{
  DIR *d;
  
  struct dirent *dir;
  char *username1; 
  char *path;
  path = malloc(sizeof(char)*50);
  username1 = malloc(sizeof(char)*100);
  strcpy(path,"/home/kiit/project/");
  strcat(path,transform(username));
  strcat(path,"/");
  
  printf("%s\n", path);
  int i=1, temp, choice;
  
  
  //system("clear");
  //printf("-------------------------------INBOX------------------------------------\n");
 // printf("      Mail Subject                               Date & Time             \n");
 // printf("========================================================================\n");
  
  
  d = opendir(path);
  
  
  
  if (d== NULL)
  {
   	printf("could not open current directory"); 
  }
  else
  {  
    
    while ((dir = readdir(d)) != NULL)
    {
      
      
      if(strcmp(dir->d_name,"..")==0|| strcmp(dir->d_name, ".")==0)
      continue;
      
      
     
      
      if(i == option)
      {
     		
     		strcat(path,dir->d_name);
     		
     		closedir(d);
     		return path;
     	}
     	
     i++;
      
    }
	 
    closedir(d);
  }
  
}



























