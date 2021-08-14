#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void merge(int parr[],int l,int m,int r)
{
	int n1=m-l+1;
	int n2=r-m;
	int L[n1],R[n2];
	for(int i=0;i<n1;i++)
		L[i]=parr[l+i];
	for(int j=0;j<n2;j++)
		R[j]=parr[m+1+j];
	int i=0;
	int j=0;
	int k=l;
	while(i<n1 && j<n2)
	{
		if(L[i]<=R[j])
		{
			parr[k]=L[i];
			i++;
		}
		else
		{
			parr[k]=R[j];
			j++;
		}
		k++;
	}
	while(i<n1)
	{
		parr[k]=L[i];
		i++;
		k++;
	}
	while(j<n2)
	{
		parr[k]=R[j];
		j++;
		k++;
	}
}

void Merge_sort(int parr[],int l,int r) //simple merge-sort algorithm
{
	if(l<r)
	{
		int mid =l+(r-l)/2;
		Merge_sort(parr,l,mid);
		Merge_sort(parr,mid+1,r);
		merge(parr,l,mid,r);
	}
	else
		return;
}

void Bubble_sort(int carr[],int n) //simple bubble sort algorithm
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n-i-1;j++)
		{
			if(carr[j]>carr[j+1])
			{
				int temp=carr[j];
				carr[j]=carr[j+1];
				carr[j+1]=temp;
			}
		}
	}
	printf("Output By Child process: ");
	for(int i=0;i<n;i++) // priting output of child process
		printf("%d ",carr[i]);

	printf("\n");
}

int main()
{
	int pid,child_pid;
	int n,status;
	printf("Enter size of array:");
	scanf("%d",&n);
	int parr[n];
	int carr[n];
	int arr[n];
	printf("\nEnter numbers: ");
	for(int i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
		parr[i]=arr[i];
		carr[i]=arr[i];
	}
	printf("\nFork call is executed\n");
	pid=getpid();
	child_pid=fork();
	if(child_pid<0)
	{
		printf("Child process formation Failed\n");
		return 0;
	}
	if(child_pid==0)
	{
		printf("Child process is being executed\n");
		Bubble_sort(carr,n);
		printf("Child process Terminated\n");
		exit(0);
	}
	else
	{ 
		printf("Parent process Waiting\n");
		wait(&status);
		printf("Parent process continued\n");
		Merge_sort(parr,0,n-1);

		printf("Output by Parent process: ");
		for(int i=0;i<n;i++)
			printf("%d ",parr[i]);
		printf("\n");
		printf("Parent process Terminated!!\n");
		exit(0);
	}
}