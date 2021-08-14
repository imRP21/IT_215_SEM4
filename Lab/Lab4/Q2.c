#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int main()
{
	int n;
	scanf("%d",&n);
	int a=1,status;
	int i=1;
	while(i<n)
	{
		int b=a; //b will indicate current node position
		printf("[%d] pid: %d, ppid: %d\n",b,getpid(),getppid());
		if(fork()==0)
		{
			a=a*2; //a will indicate newly formed left child’s position
			printf("[%d] pid: %d created left child with pid: %d\n",b,getppid(),getpid());
		}
		if(a==b)
		{
			if(fork()==0)
			{
				a=(a*2)+1; //a will indicate newly formed right child’s position
				printf("[%d] pid: %d created right child with pid: %d\n",b,getppid(),getpid());
			}
			if(a==b) //Basically parent node will wait here and then terminate and not propogate furthur.only its child node will propogate furthur.
			{
				wait(&status); //waiting for both child to get terminated
				wait(&status);
				if(a%2==1) // if condition is checked to know that current node is weather left or right child of its parent node.
					printf("[%d] right child %d of %d exited with status %d\n",b,getpid(),getppid(),a);
				else
					printf("[%d] left child %d of %d exited with status %d\n",b,getpid(),getppid(),a);
				exit(0); //current node is terminated
			}
		}
	i++;
	}
	printf("[%d] pid: %d, ppid: %d\n",a,getpid(),getppid());
	if(a%2==1)
		printf("[%d] right child %d of %d exited with status %d\n",n,getpid(),getppid(),a);
	else
		printf("[%d] left child %d of %d exited with status %d\n",n,getpid(),getppid(),a);
	exit(0);
	return 0;
}