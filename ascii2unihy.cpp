/**********************************************************************

    This file is part of ascii2unihy Armenian ASCII to Unicode converter
    Copyright (C) 2008-2014 Eduard Babayan <edo248[at]gmail.com>    

    ascii2unihy is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ascii2unihy is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ascii2unihy.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************/

#include "ascii2unihy.h"

//Unicode Functions
 
short table(unsigned char index)
{
  static short table[257]={0};
  if (!table[257]){
		for(int i=0;i<256;i++){table[i]=i;}
		table[168]=1415;
		table[175]=1371;
		table[177]=1374;
		
		//defining start points for upper and lower case letters 
		table[178]=1329;
		table[179]=1377;
		
		for(int i=180;i<254;i++)
		{
		// moving from defined starting points
			if(i%2)
			table[i]=table[i-2]+1;
			else;
			table[i]=table[i-2]+1;

		}
		table[256]=1;
  }
  return table[index];
}

int armscii2unicode (char* filename)
{


char msb;
char lsb;
unsigned char uch_buf;
unsigned short uni;
unsigned short encoding_code;

int size;
int per;
int sSize;
char html_filename[256]={HTML_OUT};
char txt_filename[256]={TXT_OUT};
char * namepoint;
size_t ext_pos;
long txt_bytes=2;
long html_bytes=0;
/*
ext_pos=strcspn(filename,".txt");
strncpy(html_filename,filename,ext_pos);
strncpy(txt_filename,filename,ext_pos);
strcat(html_filename,HTML_OUT);
strcat(txt_filename,TXT_OUT);//*/

//FILE ASSIGNING
	ifstream myFile(filename,std::ios::binary);
	ofstream outFile(txt_filename,std::ios::binary);
	ofstream outFileHtml(html_filename,std::ios::out);
	if (!myFile)
	{
		printf(FILE_NOT_FOUND,filename);
		return 1;
	}
//CHECK INPUT ENCODING;
	myFile.seekg(0);
	myFile.read(&msb,1);		
	myFile.read(&lsb,1);

	encoding_code=(unsigned char)msb<<8 | (unsigned char)lsb;

	if (encoding_code==UNICODE) 
	{
		myFile >> msb >>lsb;	
		if ((msb*0x0100 | lsb)==UNICODE) encoding_code=UTF8;
	} 	
	

//FILE SIZE
	myFile.seekg(0,ios::end);
	size=myFile.tellg();
	myFile.seekg(0);
    	sSize=size;
	per=0;
	
//OUTPUT
	cout<<"\nInput file:\t"		<<filename<<endl;
	cout<<"Input Encoding:\t"	<<PRINT_ENCODING(encoding_code)<<endl;
	cout<<"Size:   \t"		<< PRINT_BYTES(size)<<endl;
	cout<<"\nProcessing...\t["	<<setw(3)<<per<<"%]";

//WRITING UNICODE ENCODING
	msb=0xff;
	lsb=0xfe;
	switch (OUTPUT_ENCODING){
		case UNICODE_BIG_ENDIAN:	outFile.write(&lsb,1); //FEFF
						outFile.write(&msb,1);
						break;
		case UNICODE :			outFile.write(&msb,1);//FFFE
						outFile.write(&lsb,1);
						break;
		case UTF8 :			lsb=0xef;outFile.write(&lsb,1);
						lsb=0xbb;outFile.write(&lsb,1);
						lsb=0xbf;outFile.write(&lsb,1);
						txt_bytes++;
						break;
			}
//READING OUT ENCODING BYTES	
	
	if (encoding_code==UTF8)
		{
			myFile.read(&msb,1);//ef
			myFile.read(&msb,1);//bb
			myFile.read(&msb,1);//bf
		}
	else{
			myFile.read(&msb,1);//fffe	
			myFile.read(&lsb,1);//feff
		}						

//CONVERSION CYCLE
	while(!myFile.eof()){
		//READING
		switch (encoding_code){
		case UNICODE_BIG_ENDIAN:
					myFile.read(&msb,1);		
					myFile.read(&lsb,1);
					break;
		case UNICODE:
					myFile.read(&lsb,1);		
					myFile.read(&msb,1);
					break;
		case UTF8:		myFile.read(&lsb,1);
					uch_buf=(unsigned char)lsb;
					if ((uch_buf & 0x80) == 0) 
					{
						//SINGLE BYTE
						msb=0;
						size++; //reading one byte instead of 2
							//so we should increase size left
							//after size-=2 it would contain bytes left to convert
					} 
					else if ((uch_buf & 0xE0) == 0xC0) 
						{
							//DOUBLE BYTE
							myFile.read(&msb,1);
							lsb=(msb & 0x3f) | (((unsigned char)uch_buf & 0x03)<<6);
							msb=((unsigned char)uch_buf & 0x1c)>>2;
						}
					else cout <<UTF_ERROR<<endl;
					break;
				}
		
		if(myFile.eof()) break;
		
		//ENCODING
			if (msb==0)	{
						uni=table(lsb);
						msb=uni>>8 & 0x00ff;
						lsb=uni & 0x00ff;
						}
			else {uni=msb*0x0100 | lsb;}

		//WRITING	
		switch (OUTPUT_ENCODING){
		case UNICODE_BIG_ENDIAN:	outFile.write(&msb,1);
						outFile.write(&lsb,1);
						txt_bytes+=2;
						break;
		case UNICODE :			outFile.write(&lsb,1);
						outFile.write(&msb,1);
						txt_bytes+=2;
						break;
		case UTF8:			if (msb==0) 
						{//SINGLE BYTE
							outFile.write(&lsb,1);	
							txt_bytes++;
						}
						else
						{//DOUBLE BYTE
						//shifting and adding utf bits
						//adding lsb's first 2 bits to msb 
						//adding utf bits
						//////////////////////////////////////////
						// NORMAL
						// msb = aaaa aAAA	lsb = BBbb bbbb
						// UTF FORMAT FOR DOUBLE BYTE
						// msb = 110A AABB	lsb = 10bb bbbb
						//////////////////////////////////////////
						msb=(((unsigned char)msb<<2) & 0x1c) | 0xc0;
						msb|= (((unsigned char)lsb >> 6) & 0x03); 	
						lsb=((unsigned char)lsb & 0x3f) | 0x80;		
						outFile.write(&msb,1);
						outFile.write(&lsb,1);	
						txt_bytes+=2;	
						}
						break;
			}			
						
			
		// WRITING HTML
			switch  (uni){
				case 32:	outFileHtml<<" ";break;
				case 13:
				case 10:	outFileHtml<<HTML_LINE_BREAK<<endl;break;
				default:	outFileHtml<<"&#"<<uni <<";";
				}

		//PERCENTAGE		
			size-=2;
			per=(int)((1-float(size/sSize))*100);
			cout <<"\x8\x8\x8\x8\x8"<<setw(3)<<per<<"%]";
			if (per>100) exit(1);
		//
	}

myFile.close();
outFile.close();
outFileHtml.close();
myFile.open(html_filename,std::ios::out);
myFile.seekg(0,ios::end);
html_bytes=myFile.tellg();
myFile.close();

cout <<"\x8\x8\x8\x8\x8"<<setw(3)<<"100%]"<<endl;

cout		<<FILE_CONVERTED
		<<endl<<"---------------------------"	<<endl
		<<OUT_FILES				<<endl
		<<'\t'	<<txt_filename<<"\tSize:"<<PRINT_BYTES(txt_bytes)	<<endl
		<<'\t'	<<html_filename<<"\tSize:"<<PRINT_BYTES(html_bytes)<<endl
		<<"Output encoding: "<< PRINT_ENCODING(OUTPUT_ENCODING)<<endl<<endl
		<<AUTHOR;


return 0;
}

int main(int argc,char * argv[])
{	
if (argc!=2) 
	cout<< USAGE;
else 
	if (armscii2unicode(argv[1])) cout<<"Error during conversion."<<endl;
return 0;

}
