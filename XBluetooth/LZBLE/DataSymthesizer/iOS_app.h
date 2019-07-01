#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _Included_Test
#define _Included_Test

//typedef struct  {
////	char* name;
//	char name[64];
//	int age;
//
//}UserInfo;

typedef struct {
    uint8_t data[30];
    uint8_t len;
} LZBLEProtocol;

typedef struct{//卡信息
    char cardno[32];
    int isblack;
} CardInfo;

typedef struct{
	int len;
	uint8_t* orders;
}Order;

typedef struct{//app信息
	int hasvalue;//长度
	char version[4];//版本号
	char time[50];//时间
    int cardCapacity;//卡容量
    int cardCount;//卡数量
	int delayTime;//延迟时间 0x00：电控锁0.5秒，0x01-0xFE：电磁锁1-254秒
	int volume;//音量
	int recordCapacity;//记录容量
	int recordCount;//记录数量
}AppInfo;

typedef struct{//记录
	int type;//类型
	int len;//长度
    char cardno[10];//卡号
    char time[16];//时间
}Record;

typedef struct { //记录信息
	int type;//类型
	int len;//长度
	uint8_t packageNo[8];//包号
	char time[20];//时间
	Record record[16];//记录
}RecordResult;

typedef struct {
	int len;//数据长度
	uint8_t data[1024];
}BufData;

/**
 *
 * 加卡
 */
uint8_t* add_card(char* cardno,int cardlen,uint8_t type,uint8_t isblack);

/**
 * 批量加卡
 * packageno 包号
 * card 卡信息
 * len  卡信息长度
 */
uint8_t* batch_add_card(uint8_t* packageno,CardInfo *card,int len);

/**
 * gatt连接模式批量加卡
 * packageno 包号
 * subpackages 子包号
 * card 卡信息
 * len  卡信息长度
 */
uint8_t* gatt_batch_add_card(uint8_t* packageno,uint8_t subpackages,CardInfo *card,int len);

/**
 * 按卡号删除卡
 */
uint8_t* del_card_from_cardno(const char* cardno,int len);

/**
 * 按房间号删除卡
 */
uint8_t* del_card_from_Roomno(const char* roomno,int len);

/**
 * 按卡类型删除卡
 * type : 02 普通卡03  巡更卡 04 管理卡 05保安卡 06保洁卡 07物业工程卡
 */
uint8_t* del_card_from_type(uint8_t type);

/**
 * 全部删除
 */
uint8_t* del_all_card();

/**
 * 加卡时读卡
 */
uint8_t* add_read_card();

/**
 * 恢复出厂设置
 */
uint8_t* reset();

/**
 * 上位机校对时间
 */
uint8_t* pc_checking_time(char* date);

/**
 * 读卡号信息（卡号备份）
 * 读包号
 */
uint8_t* read_card_info_package_no();

/**
 * 读卡号信息（卡号备份）
 * 读单包
 */
uint8_t* read_card_info_single_package(const uint8_t* packageNo,int len);

/**
 * 开启实时记录主动上传
 */
uint8_t* open_real_time_recorde_upload();

/**
 * 关闭实时记录主动上传
 */
uint8_t* close_real_time_recorde_upload();

/**
 * 设备黑名单
 *  00代表白名单(解除黑名单)   01代表黑名单
 */
uint8_t* set_blacklist(char* cardno,int cardlen,uint8_t isblack);

/**
 * 开启住户撤防开关
 */
uint8_t* open_household_disarm();

/**
 * 关闭住户撤防开关
 */
uint8_t* close_household_disarm();

/**
 * 开启门测报警
 */
uint8_t* open_door_alarm();

/**
 * 关闭门测报警
 */
uint8_t* close_door_alarm();

/**
 * 打开电梯控制开关
 */
uint8_t* open_elevator_control();

/**
 * 关闭电梯控制开关
 */
uint8_t* close_elevator_control();

/**
 * 读取卡容量
 */
uint8_t* read_card_capacity();

/**
 * 设置卡号容量
 */
uint8_t* set_card_capacity(uint8_t* cardInfo,int len);

/**
 * 设置门禁类型
 */
uint8_t* set_access_type(uint8_t type);

/**
 * 设置默认接受地址
 */
uint8_t* accept_addr_def(uint8_t* accept);

/**
 * 设置默认发送地址
 */
uint8_t* send_addr_def(uint8_t* send);

/**
 * 设置发送协议
 */
uint8_t* send_agreement(int len,uint8_t* order,int orderLen,uint8_t* sendAddr,uint8_t* acceptAddr);

/**
 * 设置发送协议
 */
uint8_t* new_send_agreement(int len,int order_type,uint8_t* order,int orderLen,uint8_t* sendAddr,uint8_t* acceptAddr);

/**
 * 读取延迟时间
 */
uint8_t* read_time(int type);

/**
 * 设置延迟时间
 */
uint8_t* set_time(int type,int time);

/**
 * 读取系统时间
 */
uint8_t* read_system_time();

/**
 * 读取版本
 */
uint8_t* read_door_version();

/**
 * 开门
 */
uint8_t* open_door(const char* content,int len);

/**
 * 读门禁系列号
 */
uint8_t* read_door_serialno();

/**
 * 所有记录全部读
 */
uint8_t* read_all_record();

/**
 * 通过包号获取记录信息
 */
uint8_t* get_record_from_packageno(uint8_t* packageno,int len);

/**
 * 同时间读取记录信息条数
 */
uint8_t* get_record_from_time(char* time);

/**
 *通过时间读取包号再通过包号获取记录
 */
uint8_t* get_record_time_packageno(uint8_t* packageno,int len);

/**
 * 一键开门
 */
uint8_t* onekey_opendoor(uint8_t* keyArray,int keylen,const char* phone);

/**
 * 按键开门
 * 密钥正确，30秒内按“*”键开门，超时后按键无效”
 */
uint8_t* press_key_opendoor(uint8_t* keyArray,int keylen,const char* phone);

/**
 * 广告机（没有发送地址和接收地址一键开门）
 */
uint8_t* not_addr_onekey_opendoor(uint8_t* keyArray,int keylen,const char* phone);

/**
 * 没有发送地址和接收地址按键开门
 */
uint8_t* not_addr_press_key_opendoor(uint8_t* keyArray,int keylen,const char* phone);

/**
 * 蓝牙密钥匹配
 */
uint8_t* check_keys(uint8_t* keyArray,int keylen);

/**
 * 蓝牙密钥修改
 * uint8_t* oldkeys 旧的密钥
 * uint8_t* newkeys 新的密钥
 */
uint8_t* update_keys(uint8_t* oldkeys,int oldkeylen,uint8_t* newkeys,int newkeylen);

/**
 * 读取卡信息数量
 */
uint8_t* read_cardinfo_count();

/**
 * 删除所有记录
 */
uint8_t* del_all_record();

/**
 * 读取音量
 */
uint8_t* read_volume();

/**
 * 设置音量
 * 0-100%
 */
uint8_t* set_volume(int value);

/**
 * 恢复出厂值
 */
uint8_t* factory_reset();

/**
 * 卡模式复位
 */
uint8_t* card_mode_reset();

/**
 * 访客密码重置
 */
uint8_t* visit_password_reset();

/**
 * APP读取信息
 */
uint8_t* app_read_info();

/**
 * 缓冲池
 */
BufData copy_to_buffer(const uint8_t* value,int len,int type,int part);

/**
 * 更新设备的缓冲池
 */
BufData update_copy_to_buffer(const uint8_t* value,int len,int type);

/**
 * 对讲设备软件升级开始命令
 * packageCount 2个字节包数
 * uint8_t 8字节识别码
 */
uint8_t* prepare_start_update(uint32_t* packageCount,int pcklen,uint8_t* identifier,int len);

/**
 * 设备软件数据升级命令
 * packageNo 包号
 * data 数据
 */
uint8_t* start_update(uint32_t* packageNo,int pcklen,uint8_t* data,int datalen);

/**
 * gatt设备软件数据升级命令
 * packageNo 包号
 * subpackages 子包号
 * data 数据
 */
uint8_t* gatt_start_update(uint8_t* packageNo,int pcklen,uint8_t subpackages,uint8_t* data,int datalen);

/**
 * 启动下载语音文件
 */
uint8_t* start_download_volume_file(uint8_t* pckNo,int len);

/**
 * 下载语音文件数据
 * curPackageCount 当前包数
 * 从0开始，例：0x01 0xFF — 0x01FF — 511包
 *
 * dataFile : 文件数据
 */
uint8_t* download_volume_file(uint8_t* curPackageCount,int pcklen,uint8_t* dataFile,int datalen);

/**
 * gatt下载语音文件数据
 * curPackageCount 当前包数
 * 从0开始，例：0x01 0xFF — 0x01FF — 511包
 *
 * dataFile : 文件数据
 */
uint8_t* gatt_download_volume_file(uint8_t* curPackageCount,int pcklen,uint8_t subpackages,
                                  uint8_t* dataFile,int datalen);

/**
 * 获取语音文件参数
 * volsegment 语音段：0为获取语音段数量，语音段为1 – n;
 */
uint8_t* get_volume_file_param(uint8_t volsegment);

/**
 * 播放语音文件
 * volsegment 为所有语音段，语音段为1 – n;
 */
uint8_t* play_volume(uint8_t volsegment);


/**
 * 读取语音文件CHKSUM
 */
uint8_t* read_volume_file_chksum();

/**
 * 命令解析
 * retrun int
 * 00  代表成功   01   失败  02忙 03 系列号错误 04库中无卡
 */
//int parse_order(const uint8_t* value);

/**
 * 命令解析
 * type 解析的类型
 * retrun int
 * 00  代表成功   01   失败  02忙 03 系列号错误 04
 */
int parse_order(const uint8_t* value,int len,int type);

/**
 * 解析加卡
 * value : 数据源
 * len ：数据长度
 * 00 成功   01   失败   02忙   04改卡已经存在
 */
int parse_add_card(const uint8_t* value,int len);

/**
 * 解析删除卡
 * 00  成功   01   失败  02忙   04该卡已删除
 */
int parse_del_card(const uint8_t* value,int len);

/**
 * 解析加卡时读卡
 * 00  代表成功   01   代表失败  02忙
 */
int parse_add_read_card(const uint8_t* value,int len);

/**
 * 解析恢复出厂设置
 * 00  代表成功   01   代表失败 02 忙  03 系列号错误
 */
int parse_reset(const uint8_t* value,int len);

/**
 * 解析上位机校对时间
 * 00  代表成功   01   代表失败 02忙
 */
int parse_pc_checking_time(const uint8_t* value,int len);

/**
 * 解析读取延迟时间
 */
uint8_t* parse_readtime(uint8_t* value,int len);

/**
 * 解析读系统时间
 */
uint8_t* parse_read_system_time(const uint8_t* value,int len);

/**
 * 解析开门
 */
int parse_open_door(const uint8_t* value,int len);

/**
 * 解析读卡号信息（卡号备份）
 * 读包号
 */
uint8_t* parse_read_card_info_package_no(const uint8_t* value,int len);


/**
 * 解析实时记录主动上传开关
 * 00  代表成功   01   代表失败 02忙
 */
int parse_real_time_recorde_upload(const uint8_t* value,int len);

/**
 * 解析黑名单
 * 00  代表成功   01   代表失败  02忙  03密码错误
 */
int parse_set_blacklist(const uint8_t* value,int len);

/**
 * 解析住户撤防开关
 * 00  代表成功   01   代表失败 02忙
 */
int parse_household_disarm(const uint8_t* value,int len);

/**
 * 解析门测报警开关
 * 00  代表成功   01   代表失败 02忙
 */
int parse_door_alarm(const uint8_t* value,int len);

/**
 * 解析读门禁系列号
 * 00  代表成功   01   代表失败  02忙
 */
int parse_read_door_serialno(const uint8_t* value,int len);

/**
 * 解析电梯控制开关
 * 00  代表成功   01   代表失败 02忙
 */
int parse_elevator_control(const uint8_t* value,int len);

/**
 * 解析读卡号容量
 * 卡容量
 */
int parse_read_card_capacity(const uint8_t* value,int len);

/**
 * 解析设置门禁类型
 * 00  代表成功   01   代表失败  02忙  03代表系列号错误
 */
int parse_set_access_type(const uint8_t* value,int len);

/**
 * 解析批量加卡
 * [0] 回复信息位：00  代表成功   01   代表失败   02忙
 * [1]、[2]包号
 */
uint8_t* parse_batch_add_card(const uint8_t* value,int len);

/**
 * 解析gatt连接方式批量加卡
 * [0] 回复信息位：01  代表1小包成功   02代表2小包成功   10代表失败 20忙
 * [1]、[2]包号
 */
uint8_t* parse_gatt_batch_add_card(const uint8_t* value,int len);

/**
 *
 * 解析所有记录全部读
 */
int parse_read_all_record(const uint8_t* value,int len);

/**
 * 解析通过包号获取记录
 */
RecordResult parse_get_record_from_packageno(const uint8_t* value,int len);

/**
 * 部分记录通过时间获取记录
 */
RecordResult parse_get_record_from_time(const uint8_t* value,int len);

/**
 * 解析通过时间读取包号再通过包号获取记录
 */
RecordResult parse_get_record_time_packageno(const uint8_t* value,int len);

/**
 * 解析设置时间
 * 00  代表成功   01   代表失败 02 忙
 */
int parse_set_time(const uint8_t* value,int len);

/**
 * 解析一键开门
 * 0x01：成功；  0x02：失败；  0x03：蓝牙密钥错误
 */
int parse_onekey_opendoor(const uint8_t* value,int len);

/**
 * 解析按键开门
 * 0x01：成功；  0x02：失败；  0x03：蓝牙密钥错误
 */
int parse_presskey_opendoor(const uint8_t* value,int len);

/**
 * 解析版本 如：0x10代表V1.0版本。
 */
char* parse_read_door_version(uint8_t* value,int len);

char* door_version(int version);

/**
 * 解析蓝牙密钥匹配
 * 0x01：成功；  0x02：失败；  0x03：蓝牙密钥错误
 */
int parse_check_keys(const uint8_t* value,int len);

/**
 * 读取卡信息数量
 * 0x12 0x34 — 0x1234 — 4660张卡信息
 */
int parse_read_cardinfo_count(const uint8_t* value,int len);

/**
 * 解析删除所有记录
 * 0x01：成功；  0x02：失败
 */
int parse_del_all_record(const uint8_t* value,int len);

/**
 * 解析读取音量
 * 0%－100%
 *
 */
int parse_read_volume(const uint8_t* value,int len);

/**
 * 解析设置音量
 * 0x01：成功；  0x02：失败
 */
int parse_set_volume(const uint8_t* value,int len);

/**
 * 解析恢复出厂值
 * 0x01：成功；  0x02：失败
 */
int parse_factory_reset(const uint8_t* value,int len);

/**
 * 卡模式复位
 * 0x01：成功；  0x02：失败
 */
int parse_card_mode_reset(const uint8_t* value,int len);

/**
 * 蓝牙密钥修改
 * 0x01：成功；  0x02：失败；  0x03：蓝牙密钥错误
 */
int parse_update_keys(const uint8_t* value,int len);

/**
 * 解析APP读取信息
 */
AppInfo parse_app_read_info(const uint8_t* value,int len);

/**
 * 解析访客密码重置
 */
int parse_visit_password_reset(const uint8_t* value,int len);

/**
 * 解析对讲设备软件升级开始命令
 * 00——操作成功 02——忙
 *
 */
uint8_t* parse_prepare_start_update(const uint8_t* value,int len);

/**
 * 解析开始下载
 */
uint32_t* parse_start_update(const uint8_t* value,int len);

/**
 * gatt解析开始下载
 * [0] 信息回复 01—1小包成功	02—2小包成功	 03—模块ID错误
04—无			05—指令错误		06—验证错误		07—写数据错误

 * [1]、[2]当前包
 */
uint8_t* parse_gatt_start_update(const uint8_t* value,int len);

/**
 * 解析启动下载语音文件
 * 0x00：成功；  0x01：失败
 */
int parse_start_download_volume_file(const uint8_t* value,int len);

/**
 * 解析下载语音文件数据
 * [0] 00--操作成功	01--接收数据超时	02—校验和错误	03—模块ID错误
04—无			05—指令错误		06—验证错误		07—写数据错误
 * [1]、[2] 包号
 *
 */
uint8_t* parse_download_volume_file(const uint8_t* value,int len);

/**
 * 解析获取语音文件参数
 */
uint8_t* parse_get_volume_file_param(const uint8_t* value,int len);

/**
 * 解析播放语音文件
 */
uint8_t parse_play_volume(const uint8_t* value,int len);
/**
 * 解析读取语音文件CHKSUM
 * 回复信息 ： 语音文件CHKSUM
 */
uint8_t* parse_read_volume_file_chksum(const uint8_t* value,int len);

/**
 * 解析gatt下载语音文件数据
 * [0] 回复信息 01—1小包成功	02—2小包成功	 03—模块ID错误
04—无			05—指令错误		06—验证错误		07—写数据错误
 * [1]、[2] 当前包
 */
uint8_t* parse_gatt_download_volume_file(const uint8_t* value,int len);

/**
 * 解析没有发送和接收地址一键开门或解析没有发送和接收地址按键键开门
 * [0]类型、[1] 回复信息：0x01：成功；  0x02：失败；  0x03：蓝牙密钥错误
 */
uint8_t* parse_not_addr_onekey_or_press_opendoor(const uint8_t* value,int len);


/**
 * 获取密钥
 * 通过mac 计算前6位加权和（2个字节） 后6位加权和（2个字节）
 * 默认密钥计算   例如蓝牙MAC   C0-31-C1-CD-0E-3B
 *计算就是
 *3+1+C+1=17
 *C+D+0+E+3+B=51
 *默认蓝牙密钥为  1751
 */
uint8_t* and_weighting(const char *mac);



#endif
