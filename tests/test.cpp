#include<iostream>
#include<cstring>
using namespace std;

unsigned int get_degree(char* buffer, unsigned short len){
  unsigned int degree = 0 ;
  unsigned short i = 2;
  while( (buffer[i] - '0') <= 9 && (buffer[i] - '0') >= 0 ){
    cout << "i=" << i << endl;
    degree = degree * 10 + (buffer[i] - '0');
    i++;
  } 
  return degree;
}

unsigned short get_command(char* buffer,unsigned short len){
  unsigned short space_pos = 0 ; 
  while( space_pos < len && buffer[space_pos]!='/')space_pos++;
  return buffer[space_pos+1] - '0';
}


int main(){
  while(1){
    char buffer[200];
    cin>>buffer;
    cout << "degrees = " << get_degree(buffer, strlen(buffer)) << endl;
    cout << "command = " << get_command(buffer, strlen(buffer)) << endl;
  }
}
