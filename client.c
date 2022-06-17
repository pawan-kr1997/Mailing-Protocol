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

void modifier(char *filename);

void main()
{
   int i, sockfd, optionw1, infow2, optionw2;
   char buf[100];
   char *username, *password, *from, *to, *subject, *body; 
   struct sockaddr_in sa_addr;
  
   password = malloc(50);
   username = malloc(50);
   from = malloc(50);
   to = malloc(50);
   subject = malloc(50);
   body = malloc(200);





   sockfd=socket(AF_INET,SOCK_STREAM,0);

   sa_addr.sin_family=AF_INET;
   sa_addr.sin_addr.s_addr=inet_addr("127.0.0.1"); 
   sa_addr.sin_port=htons(6010);
   memset(sa_addr.sin_zero, '\0', sizeof sa_addr.sin_zero);


   i=connect(sockfd,(struct sockaddr *)&sa_addr,sizeof(sa_addr));


	
	system("clear");
	printf("\n      Email service      \n");
	printf("===========================\n");
	printf("1.Login\n");
	printf("2.Signup\n");
	printf("3.Exit\n");
	printf("Choose the option: ");
	//getchar();
	scanf("%d",&optionw1);
	switch(optionw1)
	{
		case 1:                                           //For Login purpose
		{
			
			send(sockfd, &optionw1, sizeof(int), 0);
			
			system("clear");
			
		   printf("\nUsername: ");
			scanf("%s",username);
			
			printf("\nPassword: ");
			scanf("%s",password);
			
			send(sockfd, username, 50, 0);
			send(sockfd, password, 50, 0);
			
			recv(sockfd, &infow2, sizeof(int), 0);
			
			switch(infow2)
			{
				case 4:
				{
					system("clear");
					printf("\n   WELCOME TO EMAIL SERVICES\n");
					printf("==================================\n");
					printf("1.Send message\n");
					printf("2.Inbox\n");
					printf("3.Logout\n");
					printf("\nChoose the option: ");
					scanf("%d", &optionw2);
					
					
					
					switch(optionw2)
					{
						case 1:                               //For sending email
						{
							int opt=4;
							send(sockfd, &opt, 4, 0);
							system("clear");
					
							printf("   SEND MESSAGE WINDOW\n");
							printf("===========================\n");
							//printf("From: ");
							//scanf("%s",from);
							printf("\nTo: ");
							scanf("%s",to);
							printf("\nSubject: ");
							scanf("%s",subject);
							printf("\nBody: ");
							scanf("%s",body);
					
							send(sockfd, to, 50, 0);
							send(sockfd, username, 50, 0);
							send(sockfd, subject, 50, 0);
							send(sockfd, body, 200, 0);
							
				
							system("clear");
							printf("\n\n===========****MESSAGE SENT****================\n\n");
							break;						
													
						}
						
						case 2:                           //For Inbox purpose
						{
							int opt=5, count, i, option, j, casecount=0;
							char *filename;
							char ch;
							
							filename=malloc(50);
							
							send(sockfd, &opt, 4, 0);
							recv(sockfd, &count, sizeof(int), 0);
							
						 system("clear");
                   printf("-------------------------------INBOX------------------------------------\n");
                   printf("      Mail Subject                               Date & Time             \n");
                   printf("========================================================================\n");
							
							for(i=0; i<count; i++)
							{
								
								recv(sockfd, filename, 50, 0);
								if((strcmp(filename,"could not open current directory"))==0)
								{
									printf("could not open current directory");
									exit(0);
								}
								else
								{
									printf("%d",i+1);
									
									modifier(filename);
								}								
							
							
							}
							
							printf("\n\nEnter the number of file to be viewed: ");
							scanf("%d", &option);
							
							send(sockfd, &option, 4, 0);
							
							recv(sockfd, &count, sizeof(int), 0);
							//printf("%d", count);
							
							system("clear");
							printf("=========  MESSAGE CONTENT  ===============\n");
							
							for(j=0; j<count-1; j++)
							{
								recv(sockfd, &ch, 1, 0);
								if(ch == '|')
								{
									switch(casecount)
									{
										case 0:
											printf("\nFrom: ");
											casecount++;
											continue;
											break;
										
										case 1:
											printf("\n\nSubject: ");
											casecount++;
											continue;
											break;
										
										case 2:
											printf("\n\nBody: ");
											casecount++;
											continue;
											break;
										
										case 3:
											printf("\n\n\n\n");
											continue;
											break;
										
									}
								}
								printf("%c", ch);
							
							}
							
							
							
						}
						
						
					}
					break;
				}
				
				case 5:
				{
					printf("Username and password has not been verified");
				   break;
				}
			}
			
			break;
	   }
		
		case 2:                                         //For signup purpose
		{
			send(sockfd, &optionw1, sizeof(int), 0);
			system("clear");
			
			printf("=======Sign up window==========");
			printf("\nUsername: ");
			scanf("%s",username);
			
			printf("\nPassword: ");
			scanf("%s",password);
			
			send(sockfd, username, 50, 0);
			send(sockfd, password, 50, 0);
			break;
		
		}
		
		case 3:                                          //For exit purpose
		{
			exit(0);
		}
	
	}
	
	
	
	
   close(sockfd);

 
}


//======================================  FUNCTION BODY  ================================================


void modifier(char *filename)
{
	char *temp_filename;
	int length,i,j;
	
	temp_filename= (char*)malloc(sizeof(int)*100);
	length= strlen(filename);
	
	for(j=0; j<3; j++)
	{
		*(temp_filename + j)=' ';
	}
	for(i=0; *(filename+i)!='@'; i++)
	{
		*(temp_filename + j)=*(filename+i);
		j++;
	}
	
	while(j!=40)
	{
		*(temp_filename + j)=' ';
		j++;
		
	}
	i++;
	while(*(filename+i) !='.')
	{
		*(temp_filename + j)=*(filename+i);
		i++;
		j++;
	}
	
	printf("%s",temp_filename);
}







