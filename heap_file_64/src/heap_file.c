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
	int block_num;
	BF_GetBlockCounter(fileDesc,&block_num);

	/* ============================ */

	if(block_num == 0) {
		BF_AllocateBlock(fileDesc,block);
		char *data = BF_Block_GetData(block);
		int entries = 0;
		memcpy(data,&entries,sizeof(int));
	//	if(data[0] == 0) printf("OKKK\n");
	}
	else{
		if(BF_GetBlock(fileDesc,block_num-1,block) != BF_OK) return HP_ERROR;
	}
	char *data;
	data = BF_Block_GetData(block);
	int entries,i ;
	memcpy(&entries,data,sizeof(int));
	if(entries < 17) {

		//if(entries == 1)
		i = (entries * sizeof(Record)) + (int) sizeof(int);
		//else i = (int) sizeof(int);
		memcpy(&(data[i]),&record,sizeof(Record));
		entries++;
		data = BF_Block_GetData(block);
		memcpy(data,&entries,sizeof(int));
	}
	else {
		BF_UnpinBlock(block);
		BF_AllocateBlock(fileDesc,block);
		data = BF_Block_GetData(block);

		 i = (int) sizeof(int);
		//else i = (int) sizeof(int);

		memcpy(&(data[i]),&record,sizeof(Record));
		data = BF_Block_GetData(block);
		entries = 0;
		memcpy(data,&entries,sizeof(int));
		//printf("%d\n",entries );
	}

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
	for(int i = 0;i<block_num;i++){
		BF_GetBlock(fileDesc,i,block);
		data = BF_Block_GetData(block);
		int entries;
		memcpy(&entries,data,sizeof(int));
	//	data;// += sizeof(int);
		int a = 0;
		data = BF_Block_GetData(block);
		data += sizeof(int);
		while(a<entries){
			a++;
			Record record;

			memcpy(&record,data,sizeof(Record));
			printf("%d,\"%s\",\"%s\",\"%s\"\n",record.id, record.name, record.surname, record.city );
			data+=sizeof(Record);
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
