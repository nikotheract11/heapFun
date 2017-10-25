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
	int fd;
	/*HP_OpenFile(filename,&fd);
	BF_Block *block;
	BF_Block_Init(&block);
	BF_AllocateBlock(fd,block);
	char *data = BF_Block_GetData(block);
//	memcpy(data,"hp",sizeof("hp"));
	BF_Block_SetDirty(block);
	BF_UnpinBlock(block);
	HP_CloseFile(fd);
*/

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
	/* * Initializations * */
	BF_Block *block;
	BF_Block_Init(&block);
	BF_Init(LRU);
	int block_num=0;
	BF_GetBlockCounter(fileDesc,&block_num);

	/* ============================ */
	//BF_AllocateBlock(fileDesc,block);
	char *data ;//= BF_Block_GetData(block);
	int entries = 0;
	//memcpy(&entries,data,sizeof(int));
	//printf("%d\n",entries );
	int cur_block = 1;	// block 0 contains only metadata
	while(1){
		if(BF_GetBlock(fileDesc,cur_block,block) != BF_OK) BF_AllocateBlock(fileDesc,block);
		data = BF_Block_GetData(block);
		memcpy(&entries,data,sizeof(int));
		if(data != BF_Block_GetData(block)) data = BF_Block_GetData(block);
		if(entries >= 17) {
			cur_block++;
			BF_UnpinBlock(block);
			continue;
		}
		int index = entries * sizeof(Record) + sizeof(int);
		memcpy(&(data[index]),&record,sizeof(Record));
		if(data != BF_Block_GetData(block)) data = BF_Block_GetData(block);
		entries++;
		memcpy(data,&entries,sizeof(int));
		if(data != BF_Block_GetData(block)) data = BF_Block_GetData(block);
		memcpy(&record,&(data[index]),sizeof(Record));
		//printf("%d,\"%s\",\"%s\",\"%s\"\n",record.id, record.name, record.surname, record.city);
		BF_Block_SetDirty(block);
		BF_UnpinBlock(block);
		return HP_OK;
	}
	BF_UnpinBlock(block);
	return HP_ERROR;
}

HP_ErrorCode HP_PrintAllEntries(int fileDesc) {
	BF_Block *block;
	BF_Block_Init(&block);
	Record record;
	int block_num;
	char *data;
	BF_GetBlockCounter(fileDesc,&block_num);
	for(int i = 1;i<block_num;i++){
		BF_GetBlock(fileDesc,i,block);
		data = BF_Block_GetData(block);
		int entries;
		memcpy(&entries,data,sizeof(int));
		if(data != BF_Block_GetData(block)) data = BF_Block_GetData(block);
		int i = 0;
		while(i < entries){

			int index = i * sizeof(Record) + sizeof(int);
			memcpy(&record,&(data[index]),sizeof(Record));
			if(data != BF_Block_GetData(block)) data = BF_Block_GetData(block);
			printf("%d,\"%s\",\"%s\",\"%s\"\n",record.id, record.name, record.surname, record.city);
			//entries--;
			i++;
		}
		BF_UnpinBlock(block);
	}
	BF_UnpinBlock(block);

	return HP_OK;
}

HP_ErrorCode HP_GetEntry(int fileDesc, int rowId, Record *record) {
	BF_Block *block;
	BF_Block_Init(&block);
	int block_num;
	BF_GetBlockCounter(fileDesc,&block_num);
	int off = rowId / 17 ;
	int num = rowId % 17 - 1;
	if(num < 0) {
		off--;
		num = 16 ;
	}
	if(off >= block_num || num > 16) return HP_ERROR;
	BF_GetBlock(fileDesc,off,block);
	char *data = BF_Block_GetData(block);// + sizeof(int);
	//data += sizeof(int) + (num)*sizeof(Record);
	//printf("%d\n",block_num );
	memcpy(record,&(data[(num)*sizeof(Record) + sizeof(int)]),sizeof(Record));
	BF_UnpinBlock(block);

	return HP_OK;
}
