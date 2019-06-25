#include "iOS_app.h"
#include "iOS_app_utils.h"

uint16_t buflen = 0;
uint8_t dataBuf[2048];

uint8_t fileLen = 0;
uint8_t fileDataBuf[2048];

/**
 * 截取字符串
 * ch ： 字符串
 * pos ：位置
 * length ：长度
 */
char* substring(char* ch,int pos,int length){
	//定义一个字符指针，指向传递进来的ch地址。
	char* pch=ch;
	//通过calloc来分配一个length长度的字符数组，返回的是字符指针。
	char* subch=(char*)calloc(length+1,sizeof(char));
	//只有在C99下for循环中才可以声明变量，这里写在外面，提高兼容性。
	int i;
	//是pch指针指向pos位置。
	pch=pch+pos;
	for(i=0;i<length;i++)
	{
	   subch[i]=*(pch++);
	}
	//加上字符串结束符。
	subch[length]='\0';
	//返回分配的字符数组地址。
	return subch;
}

/**
 * arr 存储字符数组
 * str 切割的字符串
 * del 字符
 */
void split(char** arr,char *str, const char *del){
	char *s =NULL;
	s=strtok(str,del);
	while(s != NULL)
	{
		*arr++ = s;
		s = strtok(NULL,del);
	}
}


/**
 * 16进制转10进制
 */
int hextoi(const char* str){
	int value;
	if (! str){
		return 0;
	}
	value = 0;
	while (1){
		if ((*str >= '0') && (*str <= '9')){
			value = value*16 + (*str - '0');
		}else if ((*str >= 'A') && (*str <= 'F')){
			value = value*16 + (*str - 'A') + 10;
		}
		else if ((*str >= 'a') && (*str <= 'f')){
			value = value*16 + (*str - 'a') + 10;
		}else{
			break;
		}
		str++;
	}
	return value;
}

/**
 * 字符串替换
 */
void replace(char *pInput, char *pOutput, char *pSrc, char *pDst){
	char *pi, *po, *p;
	int nSrcLen, nDstLen, nLen;

	// 指向输入字符串的游动指针.
	pi = pInput;
	// 指向输出字符串的游动指针.
	po = pOutput;
	// 计算被替换串和替换串的长度.
	nSrcLen = strlen(pSrc);
	nDstLen = strlen(pDst);

	// 查找pi指向字符串中第一次出现替换串的位置,并返回指针(找不到则返回null).
	p = strstr(pi, pSrc);
	if(p) {
		// 找到.
		while(p) {
			// 计算被替换串前边字符串的长度.
			nLen = (int)(p - pi);
			// 复制到输出字符串.
			memcpy(po, pi, nLen);
			memcpy(po + nLen, pDst, nDstLen);
			// 跳过被替换串.
			pi = p + nSrcLen;
			// 调整指向输出串的指针位置.
			po = po + nLen + nDstLen;
			// 继续查找.
			p = strstr(pi, pSrc);
		}
		// 复制剩余字符串.
		strcpy(po, pi);
	}else{
		// 没有找到则原样复制.
		strcpy(po, pi);
	}
}

/**
 * 数字转字符串
 */
void itoa (int val, char *buf, unsigned radix){
	char *p; /* pointer to traverse string */
	char *firstdig;/* pointer to first digit */
	char temp; /* temp char */
	unsigned digval; /* value of digit */
	p = buf;
	if(val<0)
	{
		/* negative, so output '-' and negate */
		*p++= '-';
		val = (unsigned long)(-(long)val);
	}
	firstdig = p;/* save pointer to first digit */
	do {
		digval = (unsigned)(val%radix);
		val /=radix; /* get next digit */
		/* convert to ascii and store */
		if (digval > 9)
			*p++ = (char) (digval - 10 + 'a'); /* a letter */
		else
			*p++ = (char) (digval + '0'); /* a digit */
	} while(val > 0);
	/* We now have the digit of the number in the buffer, but in reverse
	 order. Thus we reverse them now. */
	*p-- = '\0'; /* terminate string; p points to last digit */
	do
	{
		temp = *p;
		*p =*firstdig;
		*firstdig= temp; /* swap *p and *firstdig */
		--p;
		++firstdig;     /* advance to next two digits */
	} while (firstdig < p); /* repeat until halfway */
}

void reversestr(char *source,char target[],unsigned int length)
{
    int i;
    for(i=0;i<length;i++)
        target[i]=source[length-1-i];
    target[i]=0;
}

/**
 * 10进制转16进制
 */
void tohex(unsigned long num,char *hexStr)
{
    unsigned long n = num;
    char hextable[]="0123456789ABCDEF";
    char temphex[16],hex[16];
    int i=0;
    while(n){
        temphex[i++]=hextable[n%16];
        n /= 16;
    }
    temphex[i]=0;
    reversestr(temphex,hex,i);
    if(num == 0){
		strcpy(hexStr,"0");
	}else{
		strcpy(hexStr,hex);
	}
}

int tobcd(int a,int b){
	return ((a&0x0f)<<4)+(b&0x0f);
}

/**
 * 接收的地址
 * 默认是 0x00,0x00,0x10,0x01
 */
uint8_t * accept_addr_def(uint8_t *accept){
    if (accept == NULL) {
    	uint8_t *temp;
        temp = malloc(sizeof(uint8_t)*4);
        temp[0] = 0x00;
        temp[1] = 0x00;
        temp[2] = 0x10;
        temp[3] = 0x01;
        return temp;
    }
    return accept;
}

/**
 * 发送地址
 * 默认是 0x00,0x00,0x00,0x00
 */
uint8_t* send_addr_def(uint8_t *send){
	if (send == NULL) {
		uint8_t *tmp;
	        tmp = malloc(sizeof(uint8_t)*4);
	        tmp[0] = 0x00;
	        tmp[1] = 0x00;
	        tmp[2] = 0x00;
	        tmp[3] = 0x00;
	        return tmp;
	 }
	return send;
}

/**
 * 帧校验和
 * 用于校验数据链路上的传输错误，校验方式为：该字节之前的所有数据之无进位算术和
 */
int calculate_fcs(int sum){
	int value = sum % 256;
	return value;
}

/**
 * 组装发送协议
 * len : 表示从命令字开始到帧校验和之前的字节数，数值范围1—256(0代表256)
 * int* 命令集合
 * orderLen 命令集合长度
 */
uint8_t* send_agreement(int len,uint8_t* order,int orderLen,uint8_t* sendAddr,uint8_t* acceptAddr){
	//返回int数组长度
	int size = len + 3;
	printf("size = %d\n",size);
	int index = 2;
	uint8_t* result = malloc(sizeof(uint8_t)*size);
	result[0] = 0xBD;
	result[1] = len;
	result[2] = 0x20;
	//发送地址
	uint8_t* send_addr = send_addr_def(sendAddr);
	int i;
	for(i = 0; i < 4; i++){
		index ++;
		result[index] = send_addr[i];

	}
//	free(send_addr);
	uint8_t* accept_addr = accept_addr_def(acceptAddr);
	int j;
	for(j = 0; j < 4; j++){
		index ++ ;
		result[index] = accept_addr[j];
	}
//	free(accept_addr);
	int k;
	for(k = 0; k < orderLen ; k++){
		index ++;
		result[index] = order[k];
	}

	uint8_t sum = 0;
	int n;
	for(n = 0; n < (size - 1); n++){
		sum += result[n];
//		printf("result[%d] = %d\n",i,result[i]);
		printf("sum = %d\n",sum);
	}

	result[size - 1] = calculate_fcs(sum);
//	free(order);
	free(send_addr);
	free(accept_addr);

	return result;
}

uint8_t* new_send_agreement(int len,int order_type,uint8_t* order,int orderLen,uint8_t* sendAddr,uint8_t* acceptAddr){
	//返回int数组长度
	int size = len + 3;
	printf("size = %d\n",size);
	int index = 2;
	uint8_t* result = malloc(sizeof(uint8_t)*size);
	result[0] = 0xBD;
	result[1] = len;
	result[2] = order_type;
	//发送地址
	uint8_t* send_addr = send_addr_def(sendAddr);
	int i;
	for(i = 0; i < 4; i++){
		index ++;
		result[index] = send_addr[i];

	}

	uint8_t* accept_addr = accept_addr_def(acceptAddr);
	int j;
	for(j = 0; j < 4; j++){
		index ++ ;
		result[index] = accept_addr[j];
	}
	int k;
	for(k = 0; k < orderLen ; k++){
		index ++;
		result[index] = order[k];
	}

	uint8_t sum = 0;
	int n;
	for(n = 0; n < (size - 1); n++){
		sum += result[n];
		printf("sum = %d\n",sum);
	}

	result[size - 1] = calculate_fcs(sum);
	free(send_addr);
	free(accept_addr);
	return result;
}

/**
 * 获取密钥
 */
uint8_t* and_weighting(const char *mac){
	if(mac == NULL){
		return NULL;
	}
	char tempMac[14]; //= calloc(14,sizeof(char));
	char *replaceStr = ":";
	char *replaceEmp = "";
	printf("mac = %s\n",mac);
	replace(mac,tempMac,replaceStr,replaceEmp);
	char *pre6 = substring(tempMac,2,4);
	printf("pre6 = %s\n",pre6);
	int pre6Len = strlen(pre6);
	int i;
	int pre6Sum = 0;
	for(i = 0; i < pre6Len; i++){
		char *cc = substring(pre6,i,1);
		int vv = strtol(cc,NULL,16);
		printf("pre6 vv = %d\n",vv);
		pre6Sum += vv;
		free(cc);
	}
	printf("pre6  = %d\n",pre6);
	free(pre6);

	char *keys = calloc(4,sizeof(char));
	uint8_t *keyArray = calloc(4,sizeof(uint8_t));

	char *pre6SumStr = calloc(4,sizeof(char));
	itoa(pre6Sum,pre6SumStr,10);//转为字符串
	strcpy(keys, pre6SumStr);
	free(pre6SumStr);

	char *last6 = substring(tempMac,6,6);
	printf("last6  = %s\n",last6);
	int last6Len = strlen(last6);
	int last6Sum = 0;
	for(i = 0; i < last6Len; i++){
		char *cc = substring(last6,i,1);
		int vv = strtol(cc,NULL,16);
		printf("last6 vv = %d\n",vv);
		last6Sum += vv;
		free(cc);
	}
	free(last6);
	char *last6SumStr = calloc(4,sizeof(char));
	itoa(last6Sum,last6SumStr,10);//转为字符串
	strcat(keys, last6SumStr);//字符串拼接
	printf("keys = %s\n",keys);
	printf("last6SumStr = %s\n",last6SumStr);
	free(last6SumStr);

	int keysLen = strlen(keys);
	for(i = 0; i < keysLen; i++){
		char *cc = substring(pre6,i,1);
		int vv = strtol(cc,NULL,10);
		printf("keys vv = %d\n",vv);
		keyArray[i] = vv;
		free(cc);
	}

	free(keys);

	return keyArray;
}


/**
 * 缓冲池
 */
BufData copy_to_buffer(const uint8_t* value,int len,int type,int part){
	int i;
	for(i = 0; i < len; i++){
		if(buflen >=2048){
			break;
		}
		dataBuf[buflen] = value[i];
		buflen++;
	}

	int datalen = 0;
	int p = 0;
	for(i = 0; i < buflen; i++){
		uint8_t d = dataBuf[i];
//		printf("d = %d\n",d);
		if(d == 0xBD){
			p = i;
			printf("p = %d\n",p);
			if(buflen > (i + 11)){

				datalen = dataBuf[i + 1];
				printf("datalen = %d\n",datalen);
				if(type != dataBuf[i+11]){
					datalen = 0;
					continue;
				}else{
					if(part != -1 && part != dataBuf[i+12]){
						continue;
					}
					if(buflen >= (datalen + 3)){
						 datalen = dataBuf[i + 1];
						break;
					}else{
						datalen = 0;
						break;
					}
				}

			}

		}

	}

	BufData bufData;
	if(datalen > 0){

		bufData.len = datalen + 3;
		uint8_t *data = calloc(datalen + 3, sizeof(uint8_t));
		for(i = 0; i < datalen + 3; i++){
			data[i] =  dataBuf[p + i];
//			printf("data[%d] = %d\n",i,data[i]);
		}
		memcpy(bufData.data,data,datalen + 3);
	}else{
		bufData.len = 0;
	}
	return bufData;
}

/**
 * 更新设备的缓冲池
 */
BufData update_copy_to_buffer(const uint8_t* value,int len,int type){
	int i;
	for(i = 0; i < len; i++){
		if(fileLen >= 255){
			break;
		}
		fileDataBuf[fileLen] = value[i];
		fileLen++;
	}

	int datalen = 0;
	int p = 0;
	int hasBdCount = 0;
	for(i = 0; i < len; i++){
		uint8_t d = value[i];
//		printf("d = %d\n",d);
		if(d == 0xBD){
			hasBdCount ++;
			p = i;
			printf("p = %d\n",p);
			if(fileLen > (i + 2)){

				datalen = value[i + 1];
				printf("datalen = %d\n",datalen);
				int length = 0;
				length = hasBdCount * (datalen + 3);
				if(fileLen < length){
					datalen = 0;
					break;
				}

				if(type != value[i+7]){
					datalen = 0;
					continue;
				}

			}

		}

	}

	BufData bufData;
	if(datalen > 0){

		bufData.len = datalen + 3;
		uint8_t *data = calloc(datalen + 3, sizeof(uint8_t));
		for(i = 0; i < datalen + 3; i++){
			data[i] =  dataBuf[p + i];
//			printf("data[%d] = %d\n",i,data[i]);
		}
		memcpy(bufData.data,data,datalen + 3);
	}else{
		bufData.len = 0;
	}
	return bufData;
}


/**
 * 上位机校对时间
 * params time[]
 * 传入的数组值顺序 秒 ：00~59 、分 ： 00~59、时 ： 00~23
 * 日 ： 01~31 月 ： 01~12 星期 ： 01~07 年 : 00~99
 */
//int* calibration_time(int time[],int len){
//	int size = len + 1;
//    int orderArray[size];
//    orderArray[0] = 0x06;
//    int i;
//    for(i = 0; i < len; i++){
//    	orderArray[i+1] = time[i];
//    }
//    int* send_order = send_agreement(0x11,orderArray,size,NULL,NULL);
//    return send_order;
//}

/**
 * 加卡
 *
 * cardno 卡号
 * cardlen 卡号的长度
 * type 卡类型
 * isblack 是否黑名单
 */
uint8_t* add_card(char* cardno,int cardlen,uint8_t type,uint8_t isblack){
	if(cardno == NULL){
		return NULL;
	}
	uint8_t* orderArray = (uint8_t*)calloc(19,sizeof(uint8_t));
	int k = 0;
	orderArray[k] = 0x01;
	k++;
	int i;
	int bcd = 0, v1 = 0,v2 = 0;
	for(i = 0; i < cardlen; i += 2,k++){
		v1 = cardno[i];
		v2 = cardno[i + 1];
		bcd = tobcd(v1,v2);
//		char* str = substring(cardno,i,2);
//		uint8_t num = atoi(str);
		orderArray[k] = bcd;
//		free(str);

	}
	int j;
	for(j = 0; j < 4; j++,k++){
		orderArray[k] = 0x00;
	}
	orderArray[k] = type;
	k++;
	uint8_t blacklist = isblack << 4;
	orderArray[k] = blacklist;
	k++;
	int n;
	for(n = 0; n < 6; n++,k++){
		orderArray[k] = 0x00;
	}

	uint8_t* send_order = send_agreement(0x1A,orderArray,17,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 批量加卡
 * packageno 两个字节的包号 例如：packageno[2] = {0x00,0x01}
 * card 卡的信息 主要字段是卡号和是否黑名单
 * len 卡数量
 */
uint8_t* batch_add_card(uint8_t* packageno,CardInfo *card,int len){
	if(packageno == NULL || card == NULL){
		return NULL;
	}
	int size = 143;
	uint8_t* orderArray = calloc(size,sizeof(uint8_t));
	orderArray[0] = 0xBD;
	orderArray[1] = 0x8C;
	orderArray[2] = 0x20;
	orderArray[3] = 0x00;
	orderArray[4] = 0x00;
	orderArray[5] = 0x00;
	orderArray[6] = 0x00;
	orderArray[7] = 0x00;
	orderArray[8] = 0x00;
	orderArray[9] = 0x10;
	orderArray[10] = 0x01;
	orderArray[11] = 0x15;
	int i, po = 12;
	for(i = 0; i < 2; i++){
		orderArray[po] = packageno[i];
		po++;
	}
	int j;
	for(j = 0; j < len; j++){

		char* cardno = (card + j)->cardno;
		int k;

		for(k = 0; k < 8; k += 2){//卡号
			int v1 = cardno[k] - '0';
			int v2 = cardno[k + 1] - '0';
			int bcd = tobcd(v1,v2);
			orderArray[po] = bcd;
			po++;
		}

		int n;
		for(n = 0; n < 5; n++){//房间号和卡类型
			orderArray[po] = 0x00;
			po++;
		}

		uint8_t black = (card + j)->isblack;
		uint8_t blacklist = black << 4; //卡状态
		orderArray[po] = blacklist;
		po++;
		int m;
		for(m = 0; m < 6; m++){//其他权限
			orderArray[po] = 0x00;
			po++;
		}

	}

	int surplusCardNum = 8 - len;
	if(surplusCardNum != 0){

		int ffSum = 0;
		int k;
		for(k = 0; k < surplusCardNum; k++){
			int n;
			for(n = 0; n < 16; n++){
				ffSum += 0xFF;
				orderArray[po] = 0xFF;
				po++;
			}
		}
	}
	int h,sum = 0;
	for(h = 0; h < size - 1; h++){
		sum += orderArray[h];
	}
	orderArray[po] = sum % 256;
	return orderArray;
}

/**
 * gatt连接模式批量加卡
 * packageno 包号
 * subpackages 子包号
 * card 卡信息
 * len  卡数量
 */
uint8_t* gatt_batch_add_card(uint8_t* packageno,uint8_t subpackages,CardInfo *card,int len){
	if(packageno == NULL || card == NULL){
		return NULL;
	}
	int size = 80;
	uint8_t* orderArray = calloc(size,sizeof(uint8_t));
	orderArray[0] = 0xBD;
	orderArray[1] = 0x4D;
	orderArray[2] = 0x20;
	orderArray[3] = 0x00;
	orderArray[4] = 0x00;
	orderArray[5] = 0x00;
	orderArray[6] = 0x00;
	orderArray[7] = 0x00;
	orderArray[8] = 0x00;
	orderArray[9] = 0x10;
	orderArray[10] = 0x01;
	orderArray[11] = 0x35;
	int i, po = 12;
	for(i = 0; i < 2; i++){
		orderArray[po] = packageno[i];
		po++;
	}

	if(subpackages == 0){
		subpackages = 1;
	}
	orderArray[po] = subpackages;
	po++;

	for(i = 0; i < len; i++){
		char* cardno = (card + i)->cardno;
		int k;
		int v1 = 0,v2 = 0,bcd = 0;
		for(k = 0; k < 8; k += 2){//卡号
			v1 = cardno[k] - '0';
			v2 = cardno[k + 1] - '0';
			bcd = tobcd(v1,v2);
			orderArray[po] = bcd;
			po++;
		}

		int n;
		for(n = 0; n < 5; n++){//房间号和卡类型
			orderArray[po] = 0x00;
			po++;
		}

		uint8_t black = (card + i)->isblack;
		uint8_t blacklist = black << 4; //卡状态
		orderArray[po] = blacklist;
		po++;
		int m;
		for(m = 0; m < 6; m++){//其他权限
			orderArray[po] = 0x00;
			po++;
		}
	}
	int surplusCardNum = 4 - len;
	if(surplusCardNum != 0){
		int k = 0;
		for(; k < surplusCardNum; k++){
			int n = 0;
			for(; n < 16; n++){
				orderArray[po] = 0xFF;
				po++;
			}
		}
	}
	int sum = 0;
	for(i = 0; i < size - 1; i++){
		sum += orderArray[i];
	}
	orderArray[po] = sum % 256;

	return orderArray;
}

/**
 * 读取时间
 * params int type
 *  00   开门检测时间    01   门锁延时    02开门延时时间
 */
uint8_t* read_time(int type){
	int len = 2;
	uint8_t orderArray[len];
	orderArray[0] = 0x07;
	orderArray[1] = type;
	uint8_t* send_order = send_agreement(0x0B,orderArray,len,NULL,NULL);
	return send_order;
}

/**
 * 设置时间
 * params int type
 * 时间类型：00  开门检测时间  01   门锁延时   02 开门延时时间
 * params int time
 * 时间
 */
uint8_t* set_time(int type,int time){
	int len = 3;
	uint8_t orderArray[len];
	orderArray[0] = 0x08;
	orderArray[1] = type;
	orderArray[2] = time;
	uint8_t* send_order = send_agreement(0x0C,orderArray,len,NULL,NULL);
	return send_order;
}

/**
 * 读取系统时间
 */
uint8_t* read_system_time(){
	uint8_t orderArray[] = {0x19};
	uint8_t* send_order = send_agreement(0x0A,orderArray,1,NULL,NULL);
	return send_order;
}

/**
 * 读取门禁版本号
 *
 */
uint8_t* read_door_version(){
	uint8_t orderArray[] = {0x27};
	uint8_t* send_order = send_agreement(0x0A,orderArray,1,NULL,NULL);
	return send_order;
}

/**
 * 开门
 */
uint8_t* open_door(const char* content,int len){

	uint8_t* acceptAddr = calloc(4,sizeof(uint8_t));
	int k;
	for(k = 0; k < 4; k++){
		acceptAddr[k] = 0x0A;
	}
	uint8_t* accept = accept_addr_def(acceptAddr);
	int size = 18;
	uint8_t* orderArray = malloc(sizeof(uint8_t)*len);
	orderArray[0] = 0x50;
	orderArray[1] = 0x00;
	orderArray[2] = 0x10;
	int i;
	for(i = 0; i < len; i++){
		char c = content[i];
		int b = c - '0';
		orderArray[3 + i] = b;
	}
	int c = 16 - len;
	if(c > 0){
		int j;
		for(j = 0; j < c; j++){
			orderArray[3 + len + j] = 0x00;
		}
	}
	uint8_t* send_order = send_agreement(0x1B,orderArray,size,NULL,accept);
	free(orderArray);
	return send_order;
}

/**
 * 按卡号删除卡
 */
uint8_t* del_card_from_cardno(const char* cardno,int len){
	if(cardno != NULL){
		int size = 6;
		uint8_t* orderArray = calloc(size,sizeof(uint8_t));
		orderArray[0] = 0x02;
		orderArray[1] = 0x00;
		int i,p = 2;
		int v1 = 0,v2 = 0,bcd = 0;
		for(i = 0; i < len; i += 2){
			v1 = cardno[i] - '0';
			v2 = cardno[i + 1] - '0';
			bcd = tobcd(v1,v2);
			orderArray[p] = bcd;
			p++;
		}
		uint8_t* send_order = send_agreement(0x0F,orderArray,size,NULL,NULL);
		free(orderArray);
		return send_order;
	}
	return NULL;
}

/**
 * 按房号删除卡
 */
uint8_t* del_card_from_Roomno(const char* roomno,int len){
	if(roomno != NULL){
		int size = 6;
		uint8_t* orderArray = malloc(sizeof(uint8_t)*size);
		orderArray[0] = 0x02;
		orderArray[1] = 0x01;
		int i,p = 2;
		int v1 = 0,v2 = 0,bcd = 0;
		for(i = 0; i < len; i += 2){
			v1 = roomno[i] - '0';
			v2 = roomno[i + 1] - '0';
			bcd = tobcd(v1,v2);
			orderArray[p] = bcd;
			p++;
		}
		uint8_t* send_order = send_agreement(0x0F,orderArray,size,NULL,NULL);
		free(orderArray);
		return send_order;
	}
	return NULL;
}

/**
 * 按类型删除卡
 * type : 02 普通卡03  巡更卡 04 管理卡 05保安卡 06保洁卡 07物业工程卡
 */
uint8_t* del_card_from_type(uint8_t type){
	uint8_t* orderArray = malloc(sizeof(uint8_t)*3);
	orderArray[0] = 0x02;
	orderArray[1] = 0x02;
	orderArray[2] = type;
	uint8_t* send_order = send_agreement(0x0C,orderArray,3,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 全部删除卡
 */
uint8_t* del_all_card(){
	uint8_t* orderArray = malloc(sizeof(uint8_t)*2);
	orderArray[0] = 0x02;
	orderArray[1] = 0x03;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 加卡时读卡
 */
uint8_t* add_read_card(){
	uint8_t* orderArray = malloc(sizeof(uint8_t));
	orderArray[0] = 0x03;
	uint8_t* send_order = send_agreement(0x0A,orderArray,1,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 恢复出厂设置
 */
uint8_t* reset(){
	uint8_t* orderArray = malloc(sizeof(uint8_t));
	orderArray[0] = 0x05;
	uint8_t* send_order = send_agreement(0x0A,orderArray,1,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 上位机校对时间
 */
uint8_t* pc_checking_time(char* date){

	if(date == NULL){
		return NULL;
	}
	int size = 8;
	uint8_t* orderArray = calloc(size,sizeof(uint8_t));
	orderArray[0] = 0x06;
	int len = strlen(date);
	int i;
	int index = 1;
	for(i = 0; i < len; i+=2){
		int v1 = date[i] - '0';
		int v2 = date[i + 1] - '0';
		int bcd = tobcd(v1,v2);
		orderArray[index] = bcd;
		index ++;
	}
	uint8_t* send_order = send_agreement(0x11,orderArray,size,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 读卡号信息（卡号备份）
 * 读包号
 */
uint8_t* read_card_info_package_no(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x0B;
	orderArray[1] = 0x00;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 读卡号信息（卡号备份）
 * 读单包
 */
uint8_t* read_card_info_single_package(const uint8_t* packageNo,int len){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * (len + 2));
	orderArray[0] = 0x0B;
	orderArray[1] = 0x01;
	int i;
	for(i = 0; i < len; i++){
		orderArray[i + 2] = packageNo[i];
	}
	uint8_t* send_order = send_agreement(0x0D,orderArray,len + 2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 开启实时记录主动上传
 */
uint8_t* open_real_time_recorde_upload(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x0E;
	orderArray[1] = 0x01;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 关闭实时记录主动上传
 */
uint8_t* close_real_time_recorde_upload(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x0E;
	orderArray[1] = 0x00;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}


/**
 * 开启住户撤防开关
 */
uint8_t* open_household_disarm(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x11;
	orderArray[1] = 0x01;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 关闭住户撤防开关
 */
uint8_t* close_household_disarm(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x11;
	orderArray[1] = 0x00;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 开启门测报警
 */
uint8_t* open_door_alarm(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x12;
	orderArray[1] = 0x01;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 关闭门测报警
 */
uint8_t* close_door_alarm(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x12;
	orderArray[1] = 0x00;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 开启电梯控制开关
 */
uint8_t* open_elevator_control(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x1E;
	orderArray[1] = 0x01;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 关闭开启电梯控制开关
 */
uint8_t* close_elevator_control(){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x1E;
	orderArray[1] = 0x00;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 读取卡容量
 */
uint8_t* read_card_capacity(){
	uint8_t* orderArray = malloc(sizeof(uint8_t));
	orderArray[0] = 0x16;
	uint8_t* send_order = send_agreement(0x0A,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 设置卡容量
 */
uint8_t* set_card_capacity(uint8_t* cardInfo,int len){
	if(cardInfo == NULL){
		return NULL;
	}

	return NULL;
}

/**
 * 设置门禁类型
 */
uint8_t* set_access_type(uint8_t type){
	uint8_t* orderArray = malloc(sizeof(uint8_t) * 2);
	orderArray[0] = 0x18;
	orderArray[1] = type;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 设置黑名单
 *  00代表白名单(解除黑名单)   01代表黑名单
 */
uint8_t* set_blacklist(char* cardno,int cardlen,uint8_t isblack){
	if(cardno == NULL){
		return NULL;
	}

	uint8_t* orderArray = calloc(24,sizeof(uint8_t));
	int p = 0;
	orderArray[p] = 0x0F;
	p++;

	int i;
	int v1 = 0,v2 = 0,bcd = 0;
	for(i = 0; i < 8; i += 2){//卡号
		v1 = cardno[i] - '0';
		v2 = cardno[i + 1] - '0';
		bcd = tobcd(v1,v2);
		orderArray[p] = bcd;
		p++;
	}


	orderArray[p] = isblack;

	uint8_t* send_order = send_agreement(0x0F,orderArray,6,NULL,NULL);
	free(orderArray);

	return send_order;
}

/**
 * 读门禁系列号
 */
uint8_t* read_door_serialno(){
	uint8_t* orderArray = malloc(sizeof(uint8_t));
	orderArray[0] = 0x13;
	uint8_t* send_order = send_agreement(0x0A,orderArray,1,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 所有记录全部读
 */
uint8_t* read_all_record(){
	uint8_t* orderArray = calloc(2,sizeof(uint8_t));
	orderArray[0] = 0x0C;
	orderArray[1] = 0x00;
	uint8_t* send_order = send_agreement(0x0B,orderArray,2,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 通过包号获取记录信息
 */
uint8_t* get_record_from_packageno(uint8_t* packageno,int len){
	if(packageno == NULL){
		return NULL;
	}
	uint8_t* orderArray = calloc(4,sizeof(uint8_t));
	orderArray[0] = 0x0C;
	orderArray[1] = 0x01;
	int i;
	for(i = 0; i < len; i++){
		orderArray[2+i] = packageno[i];
	}
	int orderlen = len + 2;
	uint8_t* send_order = send_agreement(0x0D,orderArray,orderlen,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 通过时间获取记录
 */
uint8_t* get_record_from_time(char* time){
	if(time == NULL){
		return NULL;
	}
	int orderlen = 6;
	uint8_t* orderArray = calloc(orderlen,sizeof(uint8_t));
	orderArray[0] = 0x0C;
	orderArray[1] = 0x02;
	int len = strlen(time);
	printf("len = %d\n",len);

	int i,index = 2;
	for(i = 0; i < len; i += 2){
		int t1 = time[i] - '0';
		int t2 = time[i + 1] - '0';
		int bcd = tobcd(t1,t2);
		printf("bcd = %d\n",bcd);
		orderArray[index] = bcd;
		index++;
	}

	uint8_t* send_order = send_agreement(0x0F,orderArray,orderlen,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 通过时间读取包号再通过包号获取记录
 */
uint8_t* get_record_time_packageno(uint8_t* packageno,int len){
	if(packageno == NULL){
		return NULL;
	}
	uint8_t* orderArray = calloc(sizeof(uint8_t),4);
	orderArray[0] = 0x0C;
	orderArray[1] = 0x03;
	int i;
	for(i = 0; i < len; i++){
		orderArray[2+i] = packageno[i];
	}
	int orderlen = len + 2;
	uint8_t* send_order = send_agreement(0x0D,orderArray,orderlen,NULL,NULL);
	free(orderArray);
	return send_order;
}


/**
 * 一键开门
 */
uint8_t* onekey_opendoor(uint8_t* keyArray,int keylen,const char* phone){
	if(keyArray == NULL || phone == NULL){
		return NULL;
	}
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}

	char *zero = "0";
	char *tempPhone = calloc(14,sizeof(char));
    
	strcpy(tempPhone, zero);
	strcat(tempPhone, phone);
	int phoneLen = strlen(tempPhone);

	int orderLen = keylen + phoneLen;
	int order_index = 0;
	uint8_t* orderArray = calloc(orderLen,sizeof(uint8_t));
	orderArray[order_index] = 0x01;
	order_index ++;
	for(i = 0; i < keylen; i++){
		orderArray[order_index] = keyArray[i];
		order_index ++;
	}

	int bcd = 0,v1 = 0,v2 = 0;
	for(i= 0; i < phoneLen; i += 2){
		v1 = tempPhone[i] - '0';
		v2 = tempPhone[i + 1] - '0';
		bcd = tobcd(v1,v2);
		printf("bcd = %d\n",bcd);
//		char *cc = substring(tempPhone, i, 2);
//		int vv = strtol(cc, NULL, 16);
		orderArray[order_index] = bcd;
		order_index ++;
//		free(cc);
	}
	orderArray[order_index] = 0x01;
	uint8_t* send_order = new_send_agreement(0x15,0x40,orderArray,orderLen,NULL,accept_addr);
	free(tempPhone);
	free(orderArray);
	return send_order;
}

/**
 * 按键开门
 */
uint8_t* press_key_opendoor(uint8_t* keyArray,int keylen,const char* phone){
	if(keyArray == NULL || phone == NULL){
		return NULL;
	}
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}

	char *zero = "0";
	char *tempPhone = calloc(14,sizeof(char));
	strcpy(tempPhone, zero);
	strcat(tempPhone, phone);
	int phoneLen = strlen(tempPhone);

	int orderLen = keylen + phoneLen;
	int order_index = 0;
	uint8_t* orderArray = calloc(orderLen,sizeof(uint8_t));
	orderArray[order_index] = 0x01;
	order_index ++;
	for(i = 0; i < keylen; i++){
		orderArray[order_index] = keyArray[i];
		order_index ++;
	}

	int bcd = 0,v1 = 0,v2 = 0;
	for(i= 0; i < phoneLen; i += 2){
		v1 = tempPhone[i] - '0';
		v2 = tempPhone[i + 1] - '0';
		bcd = tobcd(v1,v2);
		printf("bcd = %d\n",bcd);
		orderArray[order_index] = bcd;
		order_index ++;
	}
	orderArray[order_index] = 0x02;
	uint8_t* send_order = new_send_agreement(0x15,0x40,orderArray,orderLen,NULL,accept_addr);
	free(tempPhone);
	free(orderArray);
	return send_order;
}

// 广告机
uint8_t* not_addr_onekey_opendoor(uint8_t* keyArray,int keylen,const char* phone){
	if(keyArray == NULL){
		return NULL;
	}
	char *zero = "0";
	char tempPhone[14]; //= calloc(14,sizeof(char));
	strcpy(tempPhone, zero);
	strcat(tempPhone, phone);
	int phoneLen = strlen(tempPhone);
	int size = 16;
    
	uint8_t* orderArray = calloc(size,sizeof(uint8_t));
    orderArray[0] = 0xBD;
    orderArray[1] = 0x0D;
    orderArray[2] = 0x40;
    orderArray[3] = 0x01;
    
	int orderIndex = 4;
	int i;

	for(i = 0; i < keylen; i++){
		orderArray[orderIndex] = keyArray[i];
		printf("keyArray[%d]=%d\n",i,keyArray[i]);
		orderIndex ++;
	}

	int v1 = 0,v2 = 0;
	int bcd = 0;
	for(i = 0; i < phoneLen; i += 2){
		v1 = tempPhone[i] - '0';
		v2 = tempPhone[i + 1] - '0';
		bcd = tobcd(v1,v2);
		orderArray[orderIndex] = bcd;
		orderIndex ++;
	}
	orderArray[orderIndex] = 0x01;
	orderIndex++;
	int sum = 0;
	for(i = 0; i < size - 1; i++){
		sum += orderArray[i];
	}
	orderArray[orderIndex] = sum % 256;
//	free(tempPhone);
	return orderArray;
}

uint8_t* not_addr_press_key_opendoor(uint8_t* keyArray,int keylen,const char* phone){
	if(keyArray == NULL){
		return NULL;
	}
	char *zero = "0";
	char *tempPhone = calloc(14,sizeof(char));
	strcpy(tempPhone, zero);
	strcat(tempPhone, phone);
	int phoneLen = strlen(tempPhone);
	int size = 16;
	uint8_t* orderArray = calloc(size,sizeof(uint8_t));
	orderArray[0] = 0xBD;
	orderArray[1] = 0x0D;
	orderArray[2] = 0x40;
	orderArray[3] = 0x01;
	int orderIndex = 4;
	int i;
	for(i = 0; i < keylen; i++){
		orderArray[orderIndex] = keyArray[i];
		orderIndex ++;
	}

	int v1 = 0,v2 = 0;
	int bcd = 0;
	for(i = 0; i < phoneLen; i += 2){
		v1 = tempPhone[i] - '0';
		v2 = tempPhone[i + 1] - '0';
		bcd = tobcd(v1,v2);
		orderArray[orderIndex] = bcd;
		orderIndex ++;
	}
	orderArray[orderIndex] = 0x02;
	orderIndex++;
	int sum = 0;
	for(i = 0; i < size - 1; i++){
		sum += orderArray[i];
	}
	orderArray[orderIndex] = sum % 256;
	free(tempPhone);
	return orderArray;
}

/**
 * 蓝牙密钥匹配
 */
uint8_t* check_keys(uint8_t* keyArray,int keylen){
	if(keyArray == NULL){
		return NULL;
	}

	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}

	int orderLen = keylen + 1;
	uint8_t* orderArray = calloc(orderLen,sizeof(uint8_t));
	orderArray[0] = 0x02;
	for(i = 0; i < keylen; i++){
		orderArray[i + 1] = keyArray[i];
	}
	uint8_t* send_order = new_send_agreement(0x0E,0x40,orderArray,orderLen,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 蓝牙密钥修改
 *
 *
 */
uint8_t* update_keys(uint8_t* oldkeys,int oldkeylen,uint8_t* newkeys,int newkeylen){
	if(oldkeys == NULL && newkeys == NULL){
		return NULL;
	}

	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}

	int orderLen = oldkeylen + newkeylen + 1;
	uint8_t* orderArray = calloc(orderLen,sizeof(uint8_t));
	orderArray[0] = 0x03;
	int in = 1;
	for(i = 0; i < oldkeylen; i++){
		orderArray[in] = oldkeys[i];
		in++;
	}

	for(i = 0; i < newkeylen; i++){
		orderArray[in] = newkeys[i];
		in++;
	}

	uint8_t* send_order = new_send_agreement(0x12,0x40,orderArray,orderLen,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 读取卡信息数量
 */
uint8_t* read_cardinfo_count(){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(1,sizeof(uint8_t));
	orderArray[0] = 0x04;
	uint8_t* send_order = new_send_agreement(0x0A,0x40,orderArray,1,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 删除所有记录
 */
uint8_t* del_all_record(){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(1,sizeof(uint8_t));
	orderArray[0] = 0x05;
	uint8_t* send_order = new_send_agreement(0x0A,0x40,orderArray,1,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 读取音量
 */
uint8_t* read_volume(){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(1,sizeof(uint8_t));
	orderArray[0] = 0x06;
	uint8_t* send_order = new_send_agreement(0x0A,0x40,orderArray,1,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 设置音量
 */
uint8_t* set_volume(int value){

	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(2,sizeof(uint8_t));
	orderArray[0] = 0x07;
	orderArray[1] = value;
	uint8_t* send_order = new_send_agreement(0x0B,0x40,orderArray,2,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 恢复出厂值
 */
uint8_t* factory_reset(){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(1,sizeof(uint8_t));
	orderArray[0] = 0x08;
	uint8_t* send_order = new_send_agreement(0x0A,0x40,orderArray,1,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * APP与蓝牙门禁通讯协议V0.10 9.测试按钮开关
 */
uint8_t* card_mode_reset(){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(1,sizeof(uint8_t));
	orderArray[0] = 0x09;
	uint8_t* send_order = new_send_agreement(0x0A,0x40,orderArray,1,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 访客密码重置
 */
uint8_t* visit_password_reset(){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(1,sizeof(uint8_t));
	orderArray[0] = 0x0A;
	uint8_t* send_order = new_send_agreement(0x0A,0x40,orderArray,1,NULL,accept_addr);
	free(orderArray);
	return send_order;
}


/**
 * APP读取信息
 */
uint8_t* app_read_info(){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(1,sizeof(uint8_t));
	orderArray[0] = 0x1E;
	uint8_t* send_order = new_send_agreement(0x0A,0x40,orderArray,1,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 对讲设备软件升级开始命令
 */
uint8_t* prepare_start_update(uint32_t* packageCount,int pcklen,uint8_t* identifier,int len){

	uint8_t* orderArray = calloc(44,sizeof(uint8_t));
//	char vstr[8];
//	tohex(packageCount, vstr);

	int i,p = 0;
	for(i = 0; i < pcklen; i++){
		orderArray[p] = packageCount[i];
		printf("packageCount[%d]=%d\n",i,packageCount[i]);
		p++;
	}

//	char* vstr1 = substring(vstr, 0, 2);
//	orderArray[0] = strtol(vstr1, NULL, 16);
//	char* vstr2 = substring(vstr, 2, 2);
//	orderArray[1] = strtol(vstr2, NULL, 16);


	for(i = 0; i < len; i++){
		orderArray[p] = identifier[i];
		p++;
	}
	uint8_t* send_order = new_send_agreement(0x13,0x10,orderArray,10,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 设备软件数据升级命令
 */
uint8_t* start_update(uint32_t* packageNo,int pcklen,uint8_t* data,int datalen){
	if(packageNo == NULL || data == NULL){
		return NULL;
	}
	uint8_t* orderArray = calloc(134,sizeof(uint8_t));
	int i,p = 0;
	for(i = 0; i < pcklen; i++){
		orderArray[p] = packageNo[i];
		p++;
	}
	for(i = 0; i < datalen; i++){
		orderArray[p] = data[i];
		p++;
	}

	uint8_t* send_order = new_send_agreement(0x8B,0x11,orderArray,130,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * gatt设备软件数据升级命令
 */
uint8_t* gatt_start_update(uint8_t* packageNo,int pcklen,uint8_t subpackages,uint8_t* data,int datalen){
	if(packageNo == NULL || data == NULL){
		return NULL;
	}
	int size = 67;
	uint8_t* orderArray = calloc(size,sizeof(uint8_t));
	int i,p = 0;
	for(i = 0; i < pcklen; i++){
		orderArray[p] = packageNo[i];
		p++;
	}

	orderArray[p] = subpackages;
	p++;

	for(i = 0; i < datalen; i++){
		orderArray[p] = data[i];
		p++;
	}

	uint8_t* send_order = new_send_agreement(0x4C,0x12,orderArray,size,NULL,NULL);
	free(orderArray);
	return send_order;
}

/**
 * 启动下载语音文件
 */
uint8_t* start_download_volume_file(uint8_t* pckNo,int len){

	if(pckNo == NULL || len < 2){
		return NULL;
	}

	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(48,sizeof(uint8_t));
	orderArray[0] = 0x20;

	orderArray[1] = pckNo[0];
	orderArray[2] = pckNo[1];

	uint8_t* send_order = new_send_agreement(0x0C,0x40,orderArray,3,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 下载语音文件数据
 */
uint8_t* download_volume_file(uint8_t* curPackageCount,int pcklen,uint8_t* dataFile,int datalen){
	if(curPackageCount == NULL || dataFile == NULL){
		return NULL;
	}
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	int len = 131;
	uint8_t* orderArray = calloc(len * 4,sizeof(uint8_t));
	orderArray[0] = 0x21;
	int p = 1;
	for(i = 0; i < pcklen; i++){
		orderArray[p] = curPackageCount[i];
		p++;
	}
	for(i = 0; i < datalen; i++){
		orderArray[p] = dataFile[i];
		p++;
	}
	uint8_t* send_order = new_send_agreement(0x8C,0x40,orderArray,len,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * gatt下载语音文件数据
 * curPackageCount 当前包数
 * 从0开始，例：0x01 0xFF — 0x01FF — 511包
 *
 * dataFile : 文件数据
 */
uint8_t* gatt_download_volume_file(uint8_t* curPackageCount,int pcklen,uint8_t subpackages,
                                  uint8_t* dataFile,int datalen){
    if(curPackageCount == NULL || dataFile == NULL){
        return NULL;
    }

    uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
    int i;
    for(i = 0; i < 4; i++ ){
        accept_addr[i]=0x0A;
    }

    int size = 68;

    uint8_t* orderArray = calloc(size,sizeof(uint8_t));

    orderArray[0] = 0x23;
    int p = 1;
    for(i = 0; i < pcklen; i++){
        orderArray[p] = curPackageCount[i];
        p++;
    }
    orderArray[p] = subpackages;
    p++;
    for(i = 0 ; i < datalen; i++){
        orderArray[p] = dataFile[i];
        p++;
    }

    uint8_t* send_order = new_send_agreement(0x4D,0x40,orderArray,size,NULL,accept_addr);

    free(orderArray);

    return send_order;

}



/**
 * 获取语音文件参数
 * 语音段：0为获取语音段数量，语音段为1 – n;
 */
uint8_t* get_volume_file_param(uint8_t volsegment){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(2,sizeof(uint8_t));
	orderArray[0] = 0x23;
	orderArray[1] = volsegment;
	uint8_t* send_order = new_send_agreement(0x0B,0x40,orderArray,2,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 播放语音文件
 */
uint8_t* play_volume(uint8_t volsegment){
	uint8_t* accept_addr = calloc(4,sizeof(uint8_t));
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(2,sizeof(uint8_t));
	orderArray[0] = 0x24;
	orderArray[1] = volsegment;
	uint8_t* send_order = new_send_agreement(0x0B,0x40,orderArray,2,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

/**
 * 读取语音文件CHKSUM
 */
uint8_t* read_volume_file_chksum(){
	uint8_t* accept_addr = calloc(sizeof(uint8_t),4);
	int i;
	for(i = 0; i < 4; i++ ){
		accept_addr[i]=0x0A;
	}
	uint8_t* orderArray = calloc(1,sizeof(uint8_t));
	orderArray[0] = 0x22;
	uint8_t* send_order = new_send_agreement(0x0A,0x40,orderArray,1,NULL,accept_addr);
	free(orderArray);
	return send_order;
}

//解析命令
int parse_order(const uint8_t* value,int len,int type){

	if(value == NULL){
		return -1;
	}

	BufData bufData = copy_to_buffer(value,len,type,-1);
	int size =  bufData.len;
	if(size == 0){
		return -1;
	}
	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return -1;
	}

	int i,sum = 0;
	for(i = 0; i < size - 1; i++){
		sum += bufData.data[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != bufData.data[size - 1]){
		printf("%s\n","fcs is error");
		return -1;
	}

	int orderType = bufData.data[11];
	if(orderType != type){
		printf("%s\n","type is error");
		return -1;
	}
	int result = bufData.data[size - 2];

	return result;
}

/**
 * 解析加卡
 */
int parse_add_card(const uint8_t* value,int len){
	return parse_order(value,len,0x01);
}

/**
 * 解析按卡号删除卡
 */
int parse_del_card(const uint8_t* value,int len){
	return parse_order(value,len,0x02);
}

/**
 * 解析加卡时读卡
 */
int parse_add_read_card(const uint8_t* value,int len){
	return parse_order(value,len,0x03);
}

/**
 * 解析恢复出厂设置
 */
int parse_reset(const uint8_t* value,int len){
	return parse_order(value,len,0x05);
}

/**
 * 解析上位机校对时间
 */
int parse_pc_checking_time(const uint8_t* value,int len){
	return parse_order(value,len,0x06);
}

/**
 * 解析读卡号信息（卡号备份）
 * 读包号
 */
uint8_t* parse_read_card_info_package_no(const uint8_t* value,int len){

	if(value == NULL){
		return NULL;
	}

	BufData bufData = copy_to_buffer(value,len,0x0B,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	uint8_t bd = bufData.data[0];
	if(bd != 0xBD){
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != bufData.data[size - 1]){
		return NULL;
	}

	uint8_t orderType = bufData.data[11];
	if(orderType != 0x0B){
		return NULL;
	}

	uint8_t* array = (uint8_t*)calloc(2,sizeof(uint8_t));
	int j,k = 0;
	for(j = 2; j > 0; j--,k++){
		array[k] = bufData.data[(size - 1) - j];
	}

	return array;

}

/**
 * 解析开门
 */
int parse_open_door(const uint8_t* value,int len){
	if(value == NULL){
		return -1;
	}

	BufData bufData = copy_to_buffer(value,len,0x00,-1);
	int size =  bufData.len;
	if(size == 0){
		return -1;
	}

	buflen = 0;
    memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		return -1;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != bufData.data[size - 1]){
		return -1;
	}

	int type = bufData.data[size - 3];
	if(type != 0x00){
		return -1;
	}

	int result = bufData.data[size - 2];
	return result;
}

/**
 * 解析开锁时间 [0] 类型 [1] 时间
 */
uint8_t* parse_readtime(uint8_t* value,int len){

	if(value == NULL){
		return NULL;
	}

	BufData bufData = copy_to_buffer(value,len,0x07,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != bufData.data[size - 1]){
		return NULL;
	}
	int type = bufData.data[size - 4];
	if(type != 0x07){
		return NULL;
	}
	uint8_t* array = calloc(2,sizeof(uint8_t));
	array[0] = bufData.data[size - 3];
	array[1] = bufData.data[size - 2];
	return array;

}

/**
 * 解析系统时间 [0] 年 [1]周 [2]月 [3]日 [4]时 [5]分 [6]秒
 */
uint8_t* parse_read_system_time(const uint8_t* value,int len){

	if(value == NULL){
		return NULL;
	}

	BufData bufData = copy_to_buffer(value,len,0x19,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != bufData.data[size - 1]){
		return NULL;
	}
//		int type = value[size - 9];
//		if(type != 0x19){
//			return NULL;
//		}
	uint8_t* array = calloc(7,sizeof(uint8_t));
	int k;
	for(k =0 ; k < 7; k++){
		int index = size - (8 - k);
		uint8_t v = bufData.data[index];
		array[k] = v;
	}
	return array;

}

/**
 * 解析实时记录主动上传开关
 * 00  代表成功   01   代表失败 02忙
 */
int parse_real_time_recorde_upload(const uint8_t* value,int len){
	return parse_order(value,len,0x0E);
}

/**
 * 解析黑名单
 * 00  代表成功   01   代表失败  02忙  03密码错误
 */
int parse_set_blacklist(const uint8_t* value,int len){
	return parse_order(value,len,0x0F);
}

/**
 * 解析住户撤防开关
 * 00  代表成功   01   代表失败 02忙
 */
int parse_household_disarm(const uint8_t* value,int len){
	return parse_order(value,len,0x11);
}

/**
 * 解析门测报警开关
 * 00  代表成功   01   代表失败 02忙
 */
int parse_door_alarm(const uint8_t* value,int len){
	return parse_order(value,len,0x12);
}

//int parse_read_door_serialno(const uint8_t* value){
//	return parse_order(value,0x12);
//}

/**
 * 解析电梯控制开关
 * 00  代表成功   01   代表失败 02忙
 */
int parse_elevator_control(const uint8_t* value,int len){
	return parse_order(value,len,0x14);
}

/**
 * 解析读卡号容量
 */
int parse_read_card_capacity(const uint8_t* value,int len){

	if(value == NULL){
		return -1;
	}

	BufData bufData = copy_to_buffer(value,len,0x16,-1);
	int size =  bufData.len;
	if(size == 0){
		return -1;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		return -1;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	if(fcs != bufData.data[size - 1]){
		return -1;
	}

	int orderType = bufData.data[11];
	if(orderType != 0x16){
		return -1;
	}

	char *vstr = calloc(4,sizeof(char));
	int v1 = bufData.data[12];
	char *v1str = calloc(2,sizeof(char));
	tohex(v1,v1str);//转为字符串
	strcpy(vstr,v1str);

	int v2 = bufData.data[13];
	char *v2str = calloc(2,sizeof(char));
	tohex(v2,v2str);//转为字符串
	strcat(vstr,v2str);
	int result = strtol(vstr, NULL, 16) * 8;
	free(v1str);
	free(v2str);
	free(vstr);
	return result;

}

/**
 * 解析设置门禁类型 00  代表成功   01   代表失败  02忙  03代表系列号错误
 */
int parse_set_access_type(const uint8_t* value,int len){
	return parse_order(value,len,0x18);
}

/**
 * 解析所有记录全部读
 */
int parse_read_all_record(const uint8_t* value,int len){

	if(value == NULL){
		return -1;
	}

	BufData bufData = copy_to_buffer(value,len,0x0C,0x00);
	int size =  bufData.len;
	if(size == 0){
		return -1;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	uint8_t bd = bufData.data[0];
	if(bd != 0xBD){
		return -1;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	if(fcs != bufData.data[size - 1]){
		return -1;
	}

	uint8_t orderType = bufData.data[11];
	if(orderType != 0x0C){
		return -1;
	}
	uint8_t read_type = bufData.data[12];
	if(read_type != 0x00){
		return -1;
	}

	int result = 0;
	int v1 = bufData.data[size - 3];
	int v2 = bufData.data[size - 2];
	result = v1 * 255 + v2;
	return result;

}

/**
 * 解析通过包号获取记录
 */
RecordResult parse_get_record_from_packageno(const uint8_t* value,int len){

	RecordResult recordResult;
	if(value == NULL){
		return recordResult;
	}

	BufData bufData = copy_to_buffer(value,len,0x0C,0x01);
	int size =  bufData.len;
	recordResult.len = size;
	if(size == 0){
		return recordResult;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	uint8_t bd = bufData.data[0];
	if(bd != 0xBD){
		printf("%s\n","bd is null");
		return recordResult;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	if(fcs != bufData.data[size - 1]){
		printf("%s\n","fcs is error");
		return recordResult;
	}

	uint8_t orderType = bufData.data[11];
	if(orderType != 0x0C){
		printf("%s\n","orderType is error");
		return recordResult;
	}
//	uint8_t read_type = value[12];
//	if(read_type != 0x01){
//		printf("%s\n","read_type is error");
//		return NULL;
//	}

    int count = 128 / 8;//条数
    printf("count = %d\n",count);

    Record *record = calloc(count,sizeof(Record));
        int length = 0;
        char cardnostr[160];
        char timestr[200];
        char tempArray[200];
        for(i = 1; i <= count; i++){
        memset(cardnostr,0,sizeof(cardnostr));
        memset(timestr,0,sizeof(timestr));
        memset(tempArray,0,sizeof(tempArray));
        strcpy(cardnostr,"");
        int j;
        uint8_t cardno = 0;
        for(j = 8 * (i - 1); j < (8 * i - 4) ; j++){

        	cardno = bufData.data[15 + j];
        	if(cardno < 10){
				strcat(cardnostr, "0");
			}
            tohex(cardno,tempArray);
            strcat(cardnostr, tempArray);
        }

        printf("i = %d,cardnostr = %s\n",i,cardnostr);
        char* temp = "FFFFFFFF";
		if(strcmp(cardnostr, temp)==0){
			break;
		}
        strcpy(record[i - 1].cardno, cardnostr);
		strcpy(timestr,"");
        int k;
        int time = 0;
        for(k = (8 * i - 4); k < (8 * i); k++){
//            char ctime[4];
        	time= bufData.data[15 + k];
        	if(time < 10){
        		strcat(timestr, "0");
        	}
            tohex(time,tempArray);
            strcat(timestr, tempArray);
            if(k < 8 * i - 1){
                strcat(timestr, "-");
            }

        }
		strcpy(record[i - 1].time, timestr);
		printf("i = %d,timestr = %s\n",i,timestr);
		length ++;
    }


	recordResult.len = length;
	recordResult.packageNo[0] = bufData.data[13];
	recordResult.packageNo[1] = bufData.data[14];
	for(i = 0; i < length; i++){
		strcpy(recordResult.record[i].cardno,record[i].cardno);
		strcpy(recordResult.record[i].time,record[i].time);
	}
	return recordResult;

}

/**
 * 部分记录通过时间获取记录
 *
 */
RecordResult parse_get_record_from_time(const uint8_t* value,int len){

	RecordResult recordResult;
	if(value == NULL){
		return recordResult;
	}
	BufData bufData = copy_to_buffer(value,len,0x0C,0x02);
	int size =  bufData.len;

	recordResult.len = size;
	if(size == 0){
		return recordResult;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	uint8_t bd = bufData.data[0];
	if(bd != 0xBD){
		printf("%s\n","bd is null");
		return recordResult;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	if(fcs != bufData.data[size - 1]){
		printf("%s\n","fcs is error");
		return recordResult;
	}

	uint8_t orderType = bufData.data[11];
	if(orderType != 0x0C){
		printf("%s\n","orderType is error");
		return recordResult;
	}
//	uint8_t read_type = value[12];
//	if(read_type != 0x01){
//		printf("%s\n","read_type is error");
//		return NULL;
//	}


	char timestr[20];
	char ctime[20];
	strcpy(timestr,"");
	int time = 0,po = 13;
	for(i = 0; i < 4;i++){
		time = bufData.data[po];
		tohex(time,ctime);
		strcat(timestr, ctime);
		if(i < 3){
			strcat(timestr, "-");
		}
		po++;
	}
	printf("timestr = %s\n",timestr);
	strcpy(recordResult.time,timestr);
	for(i = 0; i< 2; i++){
		recordResult.packageNo[i] = bufData.data[po];
		printf("bufData.data[%d]=%d\n",i,bufData.data[po]);
		po++;
	}
	return recordResult;

}

/**
 * 解析通过时间读取包号再通过包号获取记录
 */
RecordResult parse_get_record_time_packageno(const uint8_t* value,int len){
	RecordResult recordResult;
	if(value == NULL){
		return recordResult;
	}

	BufData bufData = copy_to_buffer(value,len,0x0C,0x03);
	int size =  bufData.len;
	recordResult.len = size;
	if(size == 0){
		return recordResult;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	uint8_t bd = bufData.data[0];
	if(bd != 0xBD){
		printf("%s\n","bd is null");
		return recordResult;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	if(fcs != bufData.data[size - 1]){
		printf("%s\n","fcs is error");
		return recordResult;
	}

	uint8_t orderType = bufData.data[11];
	if(orderType != 0x0C){
		printf("%s\n","orderType is error");
		return recordResult;
	}
//	uint8_t read_type = value[12];
//	if(read_type != 0x01){
//		printf("%s\n","read_type is error");
//		return NULL;
//	}

	int count = 128 / 8;//条数
	printf("count = %d\n",count);

	Record *record = calloc(count,sizeof(Record));
		int length = 0;
		char cardnostr[160];
		char timestr[200];
		char tempArray[200];
		for(i = 1; i <= count; i++){
		memset(cardnostr,0,sizeof(cardnostr));
		memset(timestr,0,sizeof(timestr));
		memset(tempArray,0,sizeof(tempArray));
		strcpy(cardnostr,"");
		int j;
		uint8_t cardno = 0;
		for(j = 8 * (i - 1); j < (8 * i - 4) ; j++){
			cardno = bufData.data[15 + j];
			if(cardno < 10){
				strcat(cardnostr, "0");
			}
			tohex(cardno,tempArray);
			strcat(cardnostr, tempArray);
		}

		printf("i = %d,cardnostr = %s\n",i,cardnostr);
		char* temp = "FFFFFFFF";
		if(strcmp(cardnostr, temp)==0){
			break;
		}
		strcpy(record[i - 1].cardno, cardnostr);
		strcpy(timestr,"");
		int k;
		int time = 0;
		for(k = (8 * i - 4); k < (8 * i); k++){
//            char ctime[4];
			time= bufData.data[15 + k];
			if(time < 10){
				strcat(timestr, "0");
			}
			tohex(time,tempArray);
			strcat(timestr, tempArray);
			if(k < 8 * i - 1){
				strcat(timestr, "-");
			}

		}
		strcpy(record[i - 1].time, timestr);
		printf("i = %d,timestr = %s\n",i,timestr);
		length ++;
	}


	recordResult.len = length;
	recordResult.packageNo[0] = bufData.data[13];
	recordResult.packageNo[1] = bufData.data[14];
	for(i = 0; i < length; i++){
		strcpy(recordResult.record[i].cardno,record[i].cardno);
		strcpy(recordResult.record[i].time,record[i].time);
	}
	return recordResult;
}


/**
 * 解析设置时间
 * 00  代表成功   01   代表失败 02 忙
 */
int parse_set_time(const uint8_t* value,int len){
	return parse_order(value,len,0x08);
}

/**
 * 解析一键开门
 */
int parse_onekey_opendoor(const uint8_t* value,int len){
	return parse_order(value,len,0x01);
}

/**
 * 解析按键开门
 */
int parse_presskey_opendoor(const uint8_t* value,int len){
	return parse_order(value,len,0x01);
}


/**
 * 解析批量加卡
 * [0] 回复信息位：00  代表成功   01   代表失败   02忙
 * [1]、[2]包号
 */
uint8_t* parse_batch_add_card(const uint8_t* value,int len){

	if(value == NULL){
		return NULL;
	}

	BufData bufData = copy_to_buffer(value,len,0x15,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}

	buflen = 0;
    memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	int orderType = bufData.data[11];
	if(orderType != 0x15){
		return NULL;
	}

	uint8_t* result = calloc(3,sizeof(uint8_t));
	result[0] = bufData.data[14];
	result[1] = bufData.data[12];
	result[2] = bufData.data[13];
	return result;
}

/**
 * 解析gatt连接方式批量加卡
 * [0] 回复信息位：01  代表1小包成功   02代表2小包成功   10代表失败 20忙
 * [1]、[2]包号
 */
uint8_t* parse_gatt_batch_add_card(const uint8_t* value,int len){

	if(value == NULL){
		return NULL;
	}

	BufData bufData = copy_to_buffer(value,len,0x35,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}

	buflen = 0;
    memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	int orderType = bufData.data[11];
	if(orderType != 0x35){
		return NULL;
	}

	uint8_t* result = calloc(3,sizeof(uint8_t));
	result[0] = bufData.data[14];
	result[1] = bufData.data[12];
	result[2] = bufData.data[13];
	return result;
}

/**
 * 版本
 */
char* door_version(int version){
	char* c = NULL;
	switch(version){
		case 0x10://v1.0
			c = "V1.0";
			break;
		case 0x11://v1.1
			c = "V1.1";
			break;
		case 0x12://v1.2
			c = "V1.2";
			break;
		case 0x13://v1.3
			c = "V1.3";
			break;
		case 0x14://v1.4
			c = "V1.4";
			break;
		case 0x15://v1.5
			c = "V1.5";
			break;
		case 0x16://v1.6
			c = "V1.6";
			break;
		case 0x17://v1.7
			c = "V1.7";
			break;
		case 0x18://v1.8
			c = "V1.8";
			break;
		case 0x19://v1.9
			c = "V1.9";
			break;
		case 0x20://v2.0
			c = "V2.0";
			break;
		case 0x21://v2.1
			c = "V2.1";
			break;
		case 0x22://v2.2
			c = "V2.2";
			break;
		case 0x23://v2.3
			c = "V2.3";
			break;
		case 0x24://v2.4
			c = "V2.4";
			break;
		case 0x25://v2.5
			c = "V2.5";
			break;
		case 0x26://v2.6
			c= "V2.6";
			break;
		case 0x27://v2.7
			c = "V2.7";
			break;
		case 0x28://v2.8
			c = "V2.8";
			break;
		case 0x29://v2.9
			c = "V2.9";
			break;
		case 0x30://v3.0
			c = "V3.0";
			break;
		case 0x31://v3.1
			c = "V3.1";
			break;
		case 0x32://v3.2
			c = "V3.2";
			break;
		case 0x33://v3.3
			c = "V3.3";
			break;
		case 0x34://v3.4
			c = "V3.4";
			break;
		case 0x35://v3.5
			c = "V3.5";
			break;
		case 0x36://v3.6
			c = "V3.6";
			break;
		case 0x37://v3.7
			c = "V3.7";
			break;
		case 0x38://v3.8
			c = "V3.8";
			break;
		case 0x39://v3.9
			c = "V3.9";
			break;
		case 0x40://v4.0
			c = "V4.0";
			break;
		case 0x41://v4.1
			c = "V4.1";
			break;
		case 0x42://v4.2
			c = "V4.2";
			break;
		case 0x43://v4.3
			c = "V4.3";
			break;
		case 0x44://v4.4
			c = "V4.4";
			break;
		case 0x45://v4.5
			c = "V4.5";
			break;
		case 0x46://v4.6
			c = "V4.6";
			break;
		case 0x47://v4.7
			c = "V4.7";
			break;
		case 0x48://v4.8
			c = "V4.8";
			break;
		case 0x49://v4.9
			c = "V4.9";
			break;
		case 0x50://v5.0
			c = "V5.0";
			break;
		case 0x51://v5.1
			c = "V5.1";
			break;
		case 0x52://v5.2
			c = "V5.2";
			break;
		case 0x53://v5.3
			c = "V5.3";
			break;
		case 0x54://v5.4
			c = "V5.4";
			break;
		case 0x55://v5.5
			c = "V5.5";
			break;
		case 0x56://v5.6
			c = "V5.6";
			break;
		case 0x57://v5.7
			c = "V5.7";
			break;
		case 0x58://v5.8
			c = "V5.8";
			break;
		case 0x59://v5.9
			c = "V5.9";
			break;
		case 0x60://v6.0
			c = "V6.0";
			break;
		case 0x61://v6.1
			c = "V6.1";
			break;
		case 0x62://v6.2
			c = "V6.2";
			break;
		case 0x63://v6.3
			c = "V6.3";
			break;
		case 0x64://v6.4
			c = "V6.4";
			break;
		case 0x65://v6.5
			c = "V6.5";
			break;
		case 0x66://v6.6
			c = "V6.6";
			break;
		case 0x67://v6.7
			c = "V6.7";
			break;
		case 0x68://v6.8
			c = "V6.8";
			break;
		case 0x69://v6.9
			c = "V6.9";
			break;
		case 0x70://v7.0
			c = "V7.0";
			break;
		case 0x71://v7.1
			c = "V7.1";
			break;
		case 0x72://v7.2
			c = "V7.2";
			break;
		case 0x73://v7.3
			c = "V7.3";
			break;
		case 0x74://v7.4
			c = "V7.4";
			break;
		case 0x75://v7.5
			c = "V7.5";
			break;
		case 0x76://v7.6
			c = "V7.6";
			break;
		case 0x77://v7.7
			c = "V7.7";
			break;
		case 0x78://v.78
			c = "V7.8";
			break;
		case 0x79://v7.9
			c = "V7.9";
			break;
		case 0x80://v8.0
			c = "V8.0";
			break;
		case 0x81://v8.1
			c = "V8.1";
			break;
		case 0x82://v8.2
			c = "V8.2";
			break;
		case 0x83://v8.3
			c = "V8.3";
			break;
		case 0x84://v8.4
			c = "V8.4";
			break;
		case 0x85://v8.5
			c = "V8.5";
			break;
		case 0x86://v8.6
			c = "V8.6";
			break;
		case 0x87://v8.7
			c = "V8.7";
			break;
		case 0x88://v8.8
			c = "V8.8";
			break;
		case 0x89://v8.9
			c = "V8.9";
			break;
		case 0x90://v9.0
			c = "V9.0";
			break;
		case 0x91://v9.1
			c = "V9.1";
			break;
		case 0x92://v9.2
			c = "V9.2";
			break;
		case 0x93://v9.3
			c = "V9.3";
			break;
		case 0x94://v9.4
			c = "V9.4";
			break;
		case 0x95://v9.5
			c = "V9.5";
			break;
		case 0x96://v9.6
			c = "V9.6";
			break;
		case 0x97://v9.7
			c = "V9.7";
			break;
		case 0x98://v9.8
			c = "V9.8";
			break;
		case 0x99://v9.9
			c = "V9.9";
			break;
	}
	return c;
}

/**
 * 解析版本号
 */
char* parse_read_door_version(uint8_t* value,int len){

	if(value == NULL){
		return NULL;
	}

	BufData bufData = copy_to_buffer(value,len,0x27,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != bufData.data[size - 1]){
		return NULL;
	}
	int type = bufData.data[size - 3];
	if(type != 0x27){
		return NULL;
	}
	int v = bufData.data[size - 2];
	char* c = door_version(v);
	return c;

}


/**
 * 解析蓝牙密钥匹配
 * 0x01：成功；  0x02：失败；  0x03：蓝牙密钥错误
 */
int parse_check_keys(const uint8_t* value,int len){
	return parse_order(value,len,0x02);
}

/**
 * 蓝牙密钥修改
 *  0x01：成功；  0x02：失败；  0x03：蓝牙密钥错误
 */
int parse_update_keys(const uint8_t* value,int len){
	return parse_order(value,len,0x03);
}

/**
 * 读取卡信息数量
 */
int parse_read_cardinfo_count(const uint8_t* value,int len){

	if(value == NULL){
		return -1;
	}

	BufData bufData = copy_to_buffer(value,len,0x04,-1);
	int size =  bufData.len;
	if(size == 0){
		return -1;
	}

	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		return -1;
	}

	int i,sum = 0;
	for(i = 0; i < (size - 1); i++){
		sum += bufData.data[i];
	}
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != bufData.data[size - 1]){
		return -1;
	}

	int orderType = bufData.data[11];
	if(orderType != 0x04){
		return -1;
	}
	int v1 = bufData.data[size - 3];
	int v2 = bufData.data[size - 2];

	char *vstr = calloc(4,sizeof(char));
	char *v1str = calloc(2,sizeof(char));
	tohex(v1,v1str);
	strcpy(vstr,v1str);

	char *v2str = calloc(2,sizeof(char));
	tohex(v2,v2str);//转为字符串
	strcat(vstr,v2str);
	int vv = strtol(vstr, NULL, 16);
	free(v1str);
	free(v2str);
	free(vstr);
	return vv;

}

/**
 * 解析删除所有记录
 */
int parse_del_all_record(const uint8_t* value,int len){
	return parse_order(value,len,0x05);
}

/**
 * 解析读取音量
 * 0%－100%
 *
 */
int parse_read_volume(const uint8_t* value,int len){
	return parse_order(value,len,0x06);
}

/**
 * 解析设置音量
 */
int parse_set_volume(const uint8_t* value,int len){
	return parse_order(value,len,0x07);
}

/**
 * 解析恢复出厂值
 * 0x01：成功；  0x02：失败
 */
int parse_factory_reset(const uint8_t* value,int len){
	return parse_order(value,len,0x08);
}

/**
 * 卡模式复位
 * 0x01：成功；  0x02：失败
 */
int parse_card_mode_reset(const uint8_t* value,int len){
	return parse_order(value,len,0x09);
}

/**
 * 解析读取APP信息
 */
AppInfo parse_app_read_info(const uint8_t* value,int len){

    AppInfo appInfo;
    if(value == NULL){
        return appInfo;
    }
    BufData bufData = copy_to_buffer(value,len,0x1E,-1);
    int size =  bufData.len;
    appInfo.hasvalue = size;
    if(size == 0){
        return appInfo;
    }

    buflen = 0;
    memset(dataBuf,0,sizeof(dataBuf));

    int bd = bufData.data[0];
    if(bd != 0xBD){
        return appInfo;
    }

    int i,sum = 0;
    for(i = 0; i < (size - 1); i++){
        sum += bufData.data[i];
    }
    int fcs = calculate_fcs(sum);
    if(fcs != bufData.data[size - 1]){
        return appInfo;
    }

    int orderType = bufData.data[11];
    if(orderType != 0x1E){
        return appInfo;
    }


    int ver = bufData.data[12];//版本号
    //	char* versionstr =door_version(ver);
    char vstr[4];
    tohex(ver,vstr);
    strcpy(appInfo.version,vstr);

    char timestr[50];
    memset(timestr, 0, strlen(timestr));
    strcpy(timestr,"");
    int po = 13;
    char temp[50];
    int t = 0;
    for(i = 0; i < 7; i++){
        t = bufData.data[po];

        tohex(t,temp);
        strcat(timestr,temp);
        if(i < 6){
            strcat(timestr,"-");
        }
        //		printf("ctime = %s\n",ctime);
        po++;
    }
    strcpy(appInfo.time,timestr);
    	printf("timestr = %s\n",timestr);

    char rlstr[30];
    memset(rlstr, 0, sizeof(rlstr));
    strcpy(rlstr,"");

    memset(temp, 0, sizeof(temp));
    int v = 0;
    for(i = 0; i < 2; i++){
        v = bufData.data[po];

        tohex(v,temp);
        strcat(rlstr,temp);
        //		printf("crl = %s\n",crl);
        po++;
    }
    	printf("rlstr = %s\n",rlstr);
    appInfo.cardCapacity = strtol(rlstr,NULL,16);

    char cslstr[30];
    memset(cslstr, 0, sizeof(cslstr));
    strcpy(cslstr,"");
    memset(temp, 0, sizeof(temp));
    int vv = 0;
    for(i = 0; i < 2; i++){

        vv = bufData.data[po];
        tohex(vv,temp);
        strcat(cslstr,temp);
        po++;
    }

    appInfo.cardCount = strtol(cslstr,NULL,16);

    int dtv = bufData.data[po];
	appInfo.delayTime = dtv;
	po++;
	appInfo.volume = bufData.data[po];
	po++;

    char rcdcapacitystr[30];
    memset(rcdcapacitystr, 0, sizeof(rcdcapacitystr));
    strcpy(rcdcapacitystr,"");
    memset(temp, 0, sizeof(temp));
    int rcd = 0;
    for(i = 0; i < 2; i++){
    	rcd = bufData.data[po];
    	printf("rcd = %d\n",rcd);
		tohex(rcd,temp);
		strcat(rcdcapacitystr,temp);
		po++;
	}
    printf("rcdcapacitystr = %s\n",rcdcapacitystr);
    appInfo.recordCapacity = strtol(rcdcapacitystr,NULL,16);
    printf("appInfo.recordCapacity = %d\n",appInfo.recordCapacity);

    char rcdcountstr[30];
	memset(rcdcountstr, 0, sizeof(rcdcountstr));
	strcpy(rcdcountstr,"");
	memset(temp, 0, sizeof(temp));
	for(i = 0; i < 2; i++){
		vv = bufData.data[po];
		tohex(vv,temp);
		strcat(rcdcountstr,temp);
		po++;
	}
	printf("rcdcountstr = %s\n",rcdcountstr);
	appInfo.recordCount = strtol(rcdcountstr,NULL,16);
	printf("appInfo.recordCoun = %d\n",appInfo.recordCount);

    return appInfo;
}

/**
 * 解析访客密码重置
 */
int parse_visit_password_reset(const uint8_t* value,int len){
	return parse_order(value,len,0x0A);
}


/**
 * 解析没有发送和接收地址一键开门或解析没有发送和接收地址按键键开门
 * [0]类型、[1] 回复信息：0x01：成功；  0x02：失败；  0x03：蓝牙密钥错误
 */
uint8_t* parse_not_addr_onekey_or_press_opendoor(const uint8_t* value,int len){
	if(value == NULL){
		return NULL;
	}
	int bd = value[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < len - 1; i++){
		sum += value[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != value[len - 1]){
		printf("%s\n","fcs is error");
		return NULL;
	}

	int orderType = value[3];
	if(orderType != 0x01){
		printf("%s\n","type is error");
		return NULL;
	}

	uint8_t* result = calloc(2,sizeof(uint8_t));
	result[0] = value[len - 3];
	result[1] = value[len - 2];
	return result;
}

/**
 * 解析对讲设备软件升级开始命令
 */
uint8_t* parse_prepare_start_update(const uint8_t* value,int len){
	if(value == NULL){
		printf("%s\n","value is null");
		return NULL;
	}

//	BufData bufData = update_copy_to_buffer(value,len,0x12);
//	int size =  bufData.len;
//	printf("bufData.len is %d\n",size);
//	if(size == 0){
//
//		return -1;
//	}
//	buflen = 0;
//	memset(dataBuf,0,sizeof(dataBuf));

	int bd = value[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < len - 1; i++){
		sum += value[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != value[len - 1]){
		printf("%s\n","fcs is error");
		return NULL;
	}

	int orderType = value[2];
	if(orderType != 0x12){
		printf("%s\n","type is error");
		return NULL;
	}

	uint8_t* array = calloc(3,sizeof(uint8_t));
//	int result = value[len - 2];
	array[0] = value[len - 4];
	array[1] = value[len - 3];
	array[2] = value[len - 2];
	return array;
}

uint32_t* parse_start_update(const uint8_t* value,int len){
	if(value == NULL){
		return NULL;
	}
//	BufData bufData = update_copy_to_buffer(value,len,0x11);
//	int size =  bufData.len;
//	if(size == 0){
//		return NULL;
//	}
//	buflen = 0;
//	memset(dataBuf,0,sizeof(dataBuf));

	int bd = value[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < len - 1; i++){
		sum += value[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != value[len - 1]){
		printf("%s\n","fcs is error");
		return NULL;
	}

//	for(i = 0; i < len ; i++){
//		printf("value[%d]=%d\n",i,value[i]);
//	}

	uint32_t* result = calloc(3,sizeof(uint32_t));
	result[0] =  value[9];
	result[1] =  value[7];
	result[2] =  value[8];
	return result;
}
/**
 * gatt解析开始下载
 * [0] 信息回复 01—1小包成功	02—2小包成功	 03—模块ID错误
04—无			05—指令错误		06—验证错误		07—写数据错误

 * [1]、[2]当前包
 */
uint8_t* parse_gatt_start_update(const uint8_t* value,int len){
	if(value == NULL){
		return NULL;
	}

	int bd = value[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < len - 1; i++){
		sum += value[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	if(fcs != value[len - 1]){
		printf("%s\n","fcs is error");
		return NULL;
	}


	uint8_t* result = calloc(3,sizeof(uint8_t));
	result[0] =  value[9];
	result[1] =  value[7];
	result[2] =  value[8];
	return result;
}


/**
 * 解析启动下载语音文件
 * 0x00：成功；  0x01：失败
 */
int parse_start_download_volume_file(const uint8_t* value,int len){
	return  parse_order(value,len,0x20);
}

/**
 * 解析下载语音文件数据
 * 00--操作成功	01--接收数据超时	02—校验和错误	03—模块ID错误
04—无			05—指令错误		06—验证错误		07—写数据错误
 *
 */
uint8_t* parse_download_volume_file(const uint8_t* value,int len){
	if(value == NULL){
		return NULL;
	}

	BufData bufData = copy_to_buffer(value,len,0x21,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}
	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < size - 1; i++){
		sum += bufData.data[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	printf("bufData.data[size - 1] = %d\n",bufData.data[size - 1]);
	if(fcs != bufData.data[size - 1]){
		printf("%s\n","fcs is error");
		return NULL;
	}

	int orderType = bufData.data[11];
	if(orderType != 0x21){
		printf("%s\n","type is error");
		return NULL;
	}
	uint8_t* result = calloc(3,sizeof(uint8_t));
	result[0] =  bufData.data[14];
	result[1] =  bufData.data[12];
	result[2] =  bufData.data[13];
	return result;
}

/**
 * 解析下载语音文件数据
 *[0]    00--操作成功	01--接收数据超时	02—校验和错误	03—模块ID错误
 *04—无			05—指令错误		06—验证错误		07—写数据错误
 *
 *[1]、[2] 当前包
 */
uint8_t* parse_gatt_download_volume_file(const uint8_t* value,int len){
	if(value == NULL){
		return NULL;
	}

	BufData bufData = copy_to_buffer(value,len,0x23,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}
	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < size - 1; i++){
		sum += bufData.data[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	printf("bufData.data[size - 1] = %d\n",bufData.data[size - 1]);
	if(fcs != bufData.data[size - 1]){
		printf("%s\n","fcs is error");
		return NULL;
	}

	int orderType = bufData.data[11];
	if(orderType != 0x23){
		printf("%s\n","type is error");
		return NULL;
	}
	uint8_t* result = calloc(3,sizeof(uint8_t));
	result[0] =  bufData.data[14];
	result[1] =  bufData.data[12];
	result[2] =  bufData.data[13];
	return result;
}

uint8_t* parse_get_volume_file_param(const uint8_t* value,int len){
	if(value == NULL){
		return NULL;
	}
	int bd = value[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < len - 1; i++){
		sum += value[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	printf("value[size - 1] = %d\n",value[len - 1]);
	if(fcs != value[len - 1]){
		printf("%s\n","fcs is error");
		return NULL;
	}

	int orderType = value[7];
	if(orderType != 0x23){
		printf("%s\n","type is error");
		return NULL;
	}
	uint8_t* result = calloc(6,sizeof(uint8_t));
	for(i = 0; i < 6; i++){
		result[i] = value[8+i];
	}
	return result;
}

/**
 * 解析播放语音文件
 */
uint8_t parse_play_volume(const uint8_t* value,int len){
	if(value == NULL){
		return -1;
	}
	int bd = value[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return -1;
	}

	int i,sum = 0;
	for(i = 0; i < len - 1; i++){
		sum += value[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	printf("value[size - 1] = %d\n",value[len - 1]);
	if(fcs != value[len - 1]){
		printf("%s\n","fcs is error");
		return -1;
	}
	int orderType = value[7];
	if(orderType != 0x24){
		printf("%s\n","type is error");
		return -1;
	}

	return value[len - 2];
}

/**
 *  解析读取语音文件CHKSUM
 */
uint8_t* parse_read_volume_file_chksum(const uint8_t* value,int len){
	if(value == NULL){
		return NULL;
	}
	BufData bufData = copy_to_buffer(value,len,0x22,-1);
	int size =  bufData.len;
	if(size == 0){
		return NULL;
	}
	buflen = 0;
	memset(dataBuf,0,sizeof(dataBuf));

	int bd = bufData.data[0];
	if(bd != 0xBD){
		printf("%s\n","bd is error");
		return NULL;
	}

	int i,sum = 0;
	for(i = 0; i < size - 1; i++){
		sum += bufData.data[i];
	}
	printf("sum=%d\n",sum);
	int fcs = calculate_fcs(sum);
	printf("fcs=%d\n",fcs);
	printf("bufData.data[size - 1] = %d\n",bufData.data[size - 1]);
	if(fcs != bufData.data[size - 1]){
		printf("%s\n","fcs is error");
		return NULL;
	}

	int orderType = bufData.data[11];
	if(orderType != 0x22){
		printf("%s\n","type is error");
		return NULL;
	}
	uint8_t* result = calloc(4,sizeof(uint8_t));

	for(i = 0; i < 4; i++){
		result[i] = bufData.data[12 + i];
	}
	return result;
}
