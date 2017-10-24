#include "bf.h"
#include "heap_file.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
HP_ErrorCode HP_Init() {


	return HP_OK;
}

HP_ErrorCode HP_CreateIndex(const char *filename) {
	if(BF_CreateFile(filename) != BF_OK) return HP_ERROR;

	return HP_OK;
}

HP_ErrorCode HP_OpenFile(const char *fileName, int *fileDesc){
	if(BF_OpenFile(fileName,fileDesc) != BF_OK) return HP_ERROR;
	return HP_OK;
}

HP_ErrorCode HP_CloseFile(int fileDesc) {
	if(BF_CloseFile(fileDesc) != BF_OK) return HP_ERROR;
	return HP_OK;
}


HP_ErrorCode HP_InsertEntry(int fileDesc, Record record) {
	BF_Block *block;
	BF_Block_Init(&block);
	BF_Init(LRU);
	//	BF_AllocateBlock(fileDesc,block);
	int block_num;
	static int count = 0;
	BF_GetBlockCounter(fileDesc,&block_num);
	if(block_num == 0) {
		printf("OOOK \n");
		BF_AllocateBlock(fileDesc,block);
		char *data = BF_Block_GetData(block);
		memset(data,0,BF_BLOCK_SIZE);
	}
	else{
		if(BF_GetBlock(fileDesc,block_num-1,block) != BF_OK) return HP_ERROR;}
	char *data;
	data = BF_Block_GetData(block);
	 int i=0;
	// i = strlen(data);
/*	 static int pp = 0;
	char temp[60];
	sprintf(temp,"%d,",record.id);
	strcat(temp,record.name);
	strcat(temp,",");
	strcat(temp,record.surname);
	strcat(temp,",");
	strcat(temp,record.city);
	strcat(temp,"\n");
	count++;
	i = strlen(data);
	if(i >= 950 || pp == 16) {
		//printf("%d\n",count );
		pp = 0;
		BF_AllocateBlock(fileDesc,block);
		data = BF_Block_GetData(block);
		i=0;// = strlen(data);
	}*/
	//printf("sizeof: %d\n",sizeof(Record	) );
	memcpy(data,&record,sizeof(Record));
//	pp++;

	BF_Block_SetDirty(block);
	BF_UnpinBlock(block);

	return HP_OK;
}

HP_ErrorCode HP_PrintAllEntries(int fileDesc) {
	BF_Block *block;
	BF_Block_Init(&block);
	int block_num;
	char *data;
	BF_GetBlockCounter(fileDesc,&block_num);
	int off = 1000/16;
	int dd = 1000 % 16;
	int j=0;
	int found=0;

	BF_GetBlock(fileDesc,off,block);
	data = BF_Block_GetData(block);
	/*while(data[j] != '\n' ){
		j++;

	}
	int j2 = 0;
	while (data[j2]!='\n') {
		j2++;
	}
	char *k = malloc(j2+1);
	memcpy(k,&data[j],j2);
	//printf("%s\n",k);//&(data[j]));
//	printf("\n", );
	for(int i = 0; i < block_num; i++){
		BF_GetBlock(fileDesc,i,block);

		data = BF_Block_GetData(block);
*/
	//	printf("%s\n",data);
		BF_UnpinBlock(block);




	return HP_OK;
}

HP_ErrorCode HP_GetEntry(int fileDesc, int rowId, Record *record) {
	/*	BF_Block *block;
		BF_Block_Init(&block);
		BF_AllocateBlock(fileDesc,block);
		int block_num;
		BF_GetBlockCounter(fileDesc,&block_num);
		if(block_num > rowId) BF_GetBlock(fileDesc,rowId,block);
		else return HP_ERROR;
		unsigned char *data;
		data = BF_Block_GetData(block);
	//	printf("%s\n",data);
	BF_UnpinBlock(block);*/
	return HP_OK;
}
