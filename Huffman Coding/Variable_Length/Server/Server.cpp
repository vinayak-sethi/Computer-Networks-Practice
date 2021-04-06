#include<iostream>
#include<algorithm>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

using namespace std;

struct node
{
	char c;
	int freq;
	struct node *lcptr;
	struct node *rcptr;
	int encode[100];
	
}n[100],*hptr;

struct LT
{
	char var;
	int freqi;
}l[100];

int endd=-1;
int beg=0;
int dict[128][100];
int eTTop=1000;
int ToTSize;
int eIn[1000];

//*******    FREQSORT   *********************
bool freqsort(node n1,node n2)
{
	return (n1.freq < n2.freq);
}

//*********   Class  ***********************
class Huffman
{
	private:
		char text[100];
		
		
	public:
		
		int eInput[1000];
		void read();
		void Huffmantree();
		void code(struct node *ptr,int arr[],int top);
		void decode(struct node *ptr);
};

//*******   Func Defn. ********************

void Huffman::read()
{
	for(int ia=0;ia<ToTSize;ia++)
	{
		endd++;
		n[endd].c=l[ia].var;
		n[endd].freq=l[ia].freqi;
		n[endd].lcptr=NULL;
		n[endd].rcptr=NULL;
		for(int j=0;j<10;j++)
			n[endd].encode[j]=-666;
	}
			

	for(int i=0;i<=endd;i++)
	{
		cout<<n[i].c<<"-->"<<n[i].freq<<endl;
	}
	cout<<"INPUT IS DONE ABOVE\n";
}

void Huffman::Huffmantree()
{
	while(endd-beg>0)
	{
		sort(n+beg,n+(endd+1),freqsort);   // Sorted in desc. order
		endd++;
		n[endd].freq=n[beg].freq+n[beg+1].freq;
		n[endd].c='\0';
		n[endd].lcptr=&n[beg];
		n[endd].rcptr=&n[beg+1];
		for(int i=0;i<10;i++)
			n[endd].encode[i]=-666;
				
		beg+=2;
	}
	
	hptr=&n[beg];	
}

void Huffman::code(struct node *ptr,int arr[],int top)
{
	if(ptr->lcptr)
	{	arr[top]=0;
		code(ptr->lcptr,arr,top+1);
	}
	
	if(ptr->rcptr)
	{
		arr[top]=1;
		code(ptr->rcptr,arr,top+1);
	}
	
	if(ptr->lcptr==NULL && ptr->rcptr==NULL)
	{
		for(int i=0;i<top;i++)
			ptr->encode[i]=arr[i];		
	}
}

void Huffman::decode(struct node *ptr)
{
	for(int i=0;i<eTTop;i++)
	{
		if(eInput[i]==0)
		{
			ptr=ptr->lcptr;
		}
		else
		{
			ptr=ptr->rcptr;
		}
		if(ptr->lcptr==NULL && ptr->rcptr==NULL)
		{
			cout<<ptr->c;
			ptr=hptr;
		}
	}
}

int pos=0;

void print(struct node *ptr)
{
	if(ptr!=NULL)
	{
		print(ptr->lcptr);
		cout<<ptr->c<<"-->"<<ptr->freq;
		cout<<" ENCODE :";
		for(int i=0;i<10;i++)
		{
			dict[ptr->c][i]=ptr->encode[i];
			if(ptr->encode[i]!=-666)
			{
				cout<<ptr->encode[i];
			}
		}
		pos++;		
		cout<<endl;
		print(ptr->rcptr);
	}
}

void look()
{
	for(int i=0;i<128;i++)
	{
		for(int j=0;j<10;j++)
		if(dict[i][j]!=-666)
			cout<<dict[i][j];
		cout<<" ";
	}
	cout<<endl;
}

int main()
{
	for(int i=0;i<128;i++)
	for(int j=0;j<10;j++)
	dict[i][j]=-666;
	
	int s_socket, s_server;
	
	s_socket = socket(AF_INET, SOCK_STREAM,0); // AF_INET - consists all the ipv4 and ipv6
	struct sockaddr_in server,other;
	memset(&server,0,sizeof(server));
	memset(&other,0,sizeof(other));
	server.sin_family=AF_INET;
	server.sin_port=htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;

	bind(s_socket,(struct sockaddr*)&server, sizeof(server));
	listen(s_socket,5);
	socklen_t add;
	add = sizeof(other);
	s_server = accept(s_socket,(struct sockaddr*)&other,&add);

	int i=0;
	recv(s_server,&ToTSize,sizeof(ToTSize),0);
	recv(s_server,&i,sizeof(i),0);
	while(i<ToTSize)
	{
		recv(s_server,&l[i].var,sizeof(l[i].var),0);
		recv(s_server,&l[i].freqi,sizeof(l[i].freqi),0);
		i++;	
	}
	recv(s_server,eIn,sizeof(eIn),0);
	// *************************************
	for(int i=0;i<1000;i++)
	{
		cout<<eIn[i];
		if(eIn[i]==-666)
		{	
			eTTop=i;
			break;
		}
	}
	// ***********************************
	cout<<"ETTOP "<<eTTop<<endl;
	// ********* COMPUTATION *****************

	int a[10];
	Huffman h;
	h.read();
	h.Huffmantree();
	h.code(hptr,a,0);
	print(hptr);
	for (int ij = 0; ij < 1000; ij++)
	{
		h.eInput[ij] = eIn[ij];
	}
	cout<<"\n-----LOOKUP--------";
	look();
	cout<<"------------------\n\n";

	cout<<"------Decoded Output--------\n";
	cout<<"The Decoded format for the given input is: ";
	h.decode(hptr);
	cout<<endl;
	cout<<"----------------------------\n\n";

	close(s_server);
	close(s_socket);

	return 0;
}


