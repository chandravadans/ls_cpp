#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include<dirent.h>
#include<errno.h>
#include<pwd.h>
#include<grp.h>
#include<limits.h>
#include<libgen.h>
#include<list>
#include<algorithm>
#include<iterator>
#include<string>
#include<vector>
#include<math.h>
using namespace std;
#define LONG 0
#define REC 1
#define ALL 2
#define DIRS 3
#define SIZ 4
#define TIM 5
#define TRUE -1
#define FALSE 8
void dome(char *);
void ls_basic(char* fname);
void ls_long(char* fname);
void ls_recursive(char* fname);
void fill_me(char *fname);

int duped=0;
int opt[6];
int f_arg=0;
long sz=0;
char *fpath;
vector<string>months;
struct entity
{
  char *e_name;
  struct entity *next;
}*start=NULL;

/*----------Utility functions-------------------------*/
//Converting std::string to c_strings and vice versa
char* to_cstr(string s)
{
  char *cstr=new char[s.size()+1];
  strcpy(cstr,s.c_str());
  return cstr;
}

string to_stdstr(char *s)
{
  string stdstr=s;
  return stdstr;
}

void insert(char *fname)
{
  if(start!=NULL)
  {
    struct entity (*tmp)=start;
    while(tmp->next!=NULL)
      tmp=tmp->next;

    struct entity *temp = new struct entity;
    temp->e_name=(char *)malloc((strlen(fname)+1)*sizeof(char));
    strcpy(temp->e_name,fname);
    temp->next = NULL;
    tmp->next=temp;
  }
  else
  {
    start = new struct entity;
    start->e_name=(char *)malloc((strlen(fname)+1)*sizeof(char));
    strcpy(start->e_name,fname);
    start->next = NULL;

  }
}

int sortsize(const struct dirent **d1, const struct dirent **d2)
{
  struct stat entry1,entry2;
  char *path1=(char *)calloc(1000,sizeof(char));
  char *path2=(char *)calloc(1000,sizeof(char));

  strcpy(path1,fpath);
  strcat(path1,"/");
  strcat(path1,(*d1)->d_name);

  strcpy(path2,fpath);
  strcat(path2,"/");
  strcat(path2,(*d2)->d_name);


  lstat(path1,&entry1);
  lstat(path2,&entry2);
  if(entry2.st_blocks>entry1.st_blocks)
    return FALSE;
  else
    return TRUE;
}



int sorttime(const struct dirent **d1, const struct dirent **d2)
{
  struct stat entry1;
  char *path1=(char *)calloc(1000,sizeof(char));
  char *path2=(char *)calloc(1000,sizeof(char));

  strcpy(path1,fpath);
  strcat(path1,"/");
  strcat(path1,(*d1)->d_name);

  strcpy(path2,fpath);
  strcat(path2,"/");
  strcat(path2,(*d2)->d_name);


  lstat(path1,&entry1);

  struct stat entry2;
  lstat(path2,&entry2);
  if(entry2.st_mtime>entry1.st_mtime)
    return FALSE;
  else
    return TRUE;
}

int min(int x, unsigned int y)
{
  if(x<y)
    return x;
  if(x>=y)
    return y;
}
void to_human_readable_byte_count(long bytes,double *coeff, char *units)
{
  static char *suffix[] = { "B", "kB", "MB","GB", "TB", "EB", "ZB","YB"};
  int unit = 1024;
  int exp = 0;
  if (bytes > 0) 
  {
    exp = min( (int) (log(bytes) / log(unit)),
        (int) sizeof(suffix) / sizeof(suffix[0]) - 1);
  }
  *coeff = bytes / pow(unit, exp);
  strcpy(units,suffix[exp]);
}

int main(int argc,char *argv[])
{
  duped=isatty(1);
  char fname[10000];
  if(argc==1)
  {
    ls_basic(".");//Call ListFunction
    cout<<endl;
    return 0;
  }
  for(int i=1;i<argc;i++)
  {
    if(argv[i][0]=='-')
    {
      for(int j=1;argv[i][j]!='\0';j++)
      {
        char ch=argv[i][j];
        switch(ch)
        {
        case 'l' : opt[LONG]=1;
                   break;
        case 'a' : opt[ALL]=1; 
                   break;
        case 'd' : opt[DIRS]=1;
                   break;
        case 'R' : opt[REC]=1; 
                   break;
        case 'S' : opt[SIZ]=1; 
                   opt[TIM]=0;
                   break;
        case 't' : opt[TIM]=1;
                   opt[SIZ]=0; 
                   break;
        default  : cout<<"Option "<<argv[i][j]<<" not available (yet)";
                   return (0);
        }
      }
      if(argv[i+1])
        continue;
    }
    else
    {
      strcpy(fname,argv[i]);
      f_arg=1;
    }
    if(f_arg==0)
    {
      if(opt[DIRS]==1)
      {
        if(opt[LONG]==1)
        {
          ls_long(".");
          cout<<"."<<endl;
        }
        else
          cout<<"."<<endl;
        return(0);
      }
      else
        ls_basic(".");
    }
    else
    {
      if(opt[DIRS]==1)
      {
        if(opt[LONG]==1)
        {
          ls_long(fname);
          cout<<fname<<endl;
        }
        else
          cout<<fname<<endl;
        return (0);
      }

      struct stat sb;

      if (lstat(fname, &sb) == -1)
      {
        cerr<<"Couldn't stat"<<fname<<endl;
        return (1);
      }

      if((sb.st_mode & S_IFMT)==S_IFDIR)
      {
        cout<<fname<<"/"<<endl;
        ls_basic(fname);
      }
      else
      {
        ls_long(fname);
        if(opt[LONG]==1)
        {
          cout<<fname<<endl;
        }
      }
    }
    if(opt[REC] == 1)
    {
      struct entity *tmp=start;
      opt[REC] =0;
      while(tmp!=NULL)
      {
        //Fancy stuff
        cout<<endl;
        for(int i=0;i<strlen(tmp->e_name);i++)
          cout<<"-";
        cout<<endl;
        cout<<tmp->e_name<<":"<<endl;
        for(int i=0;i<strlen(tmp->e_name);i++)
          cout<<"-";
        cout<<endl;
        //End fancy stuff
        if(opt[LONG]!=1)
          fill_me(tmp->e_name);
        
        ls_basic(tmp->e_name);
        tmp=tmp->next;
        
      }
    }
    if(opt[LONG]==1)
    {
      double *x=(double*)malloc(sizeof(double));
      char *suffix=(char *) malloc(3*sizeof(char));
      to_human_readable_byte_count(sz,x,suffix);
       cout<<"Total : "<<*x<<" "<<suffix<<endl;
    }
  }
 return 0;
}

//Fills the list with all directories
void fill_me(char *fname)
{
  struct dirent **entry;
  char *myname=(char *)malloc(1000*sizeof(char));
  strcpy(myname,fname);
  strcat(myname,"/");
  //cout<<"In fill_me "<<myname<<endl;
 int num_entries;
 if(opt[TIM]==1)
  {
    num_entries=scandir(myname,&entry,0,sorttime);
    if(num_entries<0)
    {
      cout<<"Couldnot scan directory"<<myname<<endl;
      return;
    }
  }
  else if(opt[SIZ]==1)
  {
    num_entries=scandir(myname,&entry,0,sortsize);
    if(num_entries<0)
    {
      cout<<"Couldnot scan directory "<<myname<<endl;
      return;
    }
  }
  else
  {
    num_entries=scandir(myname,&entry,0,alphasort);
    if(num_entries<0)
    {
      cout<<"Couldnot scan directory"<<myname<<endl;
      return;
    }
  }
  for(int i=0;i<num_entries;i++)
  {
    char *concat=(char*)malloc(1000*sizeof(char));
    strcpy(concat,myname);

    strcat(concat,entry[i]->d_name);
    dome(concat);
  }
}

//Print the name, handle time and size sort
void ls_basic(char* fname)
{

  struct dirent **entry;
  int num_entries;
  

  char *name=(char *)malloc(1000*sizeof(char));
  strcpy(name,fname);
  strcat(name,"/");
  fpath=name;

  if(opt[TIM]==1)
  {
    num_entries=scandir(name,&entry,0,sorttime);
    if(num_entries<0)
    {
      cout<<"Couldnot scan directory"<<name<<endl;
      return;
    }
  }
  else if(opt[SIZ]==1)
  {
    num_entries=scandir(name,&entry,0,sortsize);
    if(num_entries<0)
    {
      cout<<"Couldnot scan directory "<<name<<endl;
      return;
    }
  }
  else
  {
    num_entries=scandir(name,&entry,0,alphasort);
    if(num_entries<0)
    {
      cout<<"Couldnot scan directory"<<name<<endl;
      return;
    }
  }
  int count=1;
  for(int i=0;i<num_entries;i++)
  {
    strcpy(name,fname);
    strcat(name,"/");
    strcat(name,entry[i]->d_name);
    char ch=entry[i]->d_name[0];

    if(ch=='.')
    {
      if(opt[ALL]==1)
      {
        if(opt[LONG]==1)
        {
          ls_long(name);
          cout<<endl;
          if(opt[REC]==1)
            ls_recursive(name);

        }
        else
        {
          if(opt[REC]==1)
            ls_recursive(name);
          printf("%-30s",entry[i]->d_name);

          //if( (sb.st_mode & S_IFMT)==S_IFDIR)
          //  cout<<"/";
          if(!duped)
            cout<<endl;
          count++;
          if(count==6)
          {
            count=1;
            cout<<endl;
          }
        }

      }
      else
        continue;
    }
    else
    {
      if(opt[LONG]==1)
      {
        ls_long(name);
        cout<<endl;
      }
      else
      {
        if(opt[REC]==1)
          ls_recursive(name);
        printf("%-30s",entry[i]->d_name);
        if(!duped)
          cout<<endl;
        count++;
        if(count==6)
        {
          count=1;
          cout<<endl;
        }
      }
    }
  }

}
void dome(char *x)
{
  ls_recursive(x);
}

//Add to list
void ls_recursive(char* fname)
{
  struct stat sb;
  if (lstat(fname, &sb) == -1)
  {
    perror(fname);
    cout<<endl;
    return;
  }
  if( (sb.st_mode & S_IFMT)==S_IFDIR)
    insert(fname);
}

//Does the whole hardwork
void ls_long(char* fname)
{
  struct stat stat_temp;
  if(lstat(fname,&stat_temp)==-1)
  {
    cerr<<"Error in stating"<<fname<<endl;

  }
  if ((stat_temp.st_mode & S_IFMT)==S_IFDIR)
  {
    cout<<"d";
    insert(fname);
  }
  else if ((stat_temp.st_mode & S_IFMT)==S_IFLNK)
  {
    cout<<"l";
  }
  else 
    cout<<"-";

  // Take care of permissions
  int val;
  val=(stat_temp.st_mode & ~S_IFMT);
  int perm[3];
  for(int i=0;i<3;i++)
  {
    perm[3-i]=val%8;
    val=val/8;
  }

  for(int i=1;i<=3;i++)
  {
    switch(perm[i])
    {

    case 0: cout<<"---"; 
            break;
    case 1: cout<<"--x"; 
            break;
    case 2: cout<<"-w-"; 
            break;
    case 3: cout<<"-wx"; 
            break;
    case 4: cout<<"r--"; 
            break;
    case 5: cout<<"r-x"; 
            break; 
    case 6: cout<<"rw-"; 
            break; 
    case 7: cout<<"rwx"; 
            break; 
    }
  }	
  //Unames and groups
  struct group *u_group=getgrgid((long)stat_temp.st_gid);
  struct passwd *user=getpwuid((long)stat_temp.st_uid);
  long int linx=(long)stat_temp.st_nlink;
  long fsize=stat_temp.st_size;
  sz+=fsize;
  struct tm *time_stamp=localtime(&stat_temp.st_mtime);

  //Printing data in order
  printf(" %3ld ",linx);
  printf(" %4s %4s ",user->pw_name,u_group->gr_name);
  printf(" %10ld ",fsize);

  //Date
  months.push_back("Jan");
  months.push_back("Feb");
  months.push_back("Mar");
  months.push_back("Apr");
  months.push_back("May");
  months.push_back("Jun");
  months.push_back("Jul");
  months.push_back("Aug");
  months.push_back("Sep");
  months.push_back("Oct");
  months.push_back("Nov");
  months.push_back("Dec");
  printf(" %4d %s %2d ", time_stamp->tm_year+1900, to_cstr(months[time_stamp->tm_mon]),time_stamp->tm_mday);

  //Time  
  printf(" %2d:%2d ",time_stamp->tm_hour,time_stamp->tm_min);

  int islink=((stat_temp.st_mode & S_IFMT )== S_IFLNK);
  if(strcmp(fname,".")!=0 && strcmp(fname,"..")!=0)
  {
    char *base=basename(fname);
    if(base[0]=='.')
    {
      if(opt[ALL]==1)
      {
        cout<<base<<" ";
      }
    }
    else
      cout<<base<<" ";

    //End if its not a link
    if(islink==0);
    else                      
    {
      //Hunt for target link
      char target[512];
      int succ=readlink((const char*)fname,target,512);
      if(succ==-1)
        strcpy(target,"(Broken)");
      else
        target[succ]='\0';
      cout<<" -> "<<target;
    }
  }
}
