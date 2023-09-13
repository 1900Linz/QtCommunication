#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    //1.初始化连接数据
//    char  *pcom    = "COM1";  //COM口
//    int    Address =  1;      //站号
//    modbus_t *mb;
//          mb = modbus_new_rtu(pcom,9600,'N',8,1);
//          modbus_set_slave(mb, Address);  //从机地址
//          modbus_connect(mb); //

//    //2.设置通讯超时时间
//    struct timeval t;
//          t.tv_sec=0;
//          t.tv_usec=1000000;   //设置modbus超时时间为1000毫秒，注意：经测试，如果没有成功建立tcp连接，则该设置无效。
//          modbus_set_response_timeout(mb,t.tv_sec,t.tv_usec);


//    //3.读数据
//    uint16_t tab_reg[128]={0};
//    int regs=modbus_read_input_registers(mb, 0, 2, tab_reg); //从0开始读2个数
//          if(regs == -1){
//            //读取失败返回-1
//          }
//          else {  //读取成功返回0
//             tab_reg[0];  //input_registers的第1个数
//             tab_reg[1];  //input_registers的第2个数
//          }

//    uint8_t bits[128]={0};
//    int rbits  = modbus_read_bits(mb, 0, 2, bits);
//    if(rbits  == -1){
//            //读取失败返回-1
//          }
//          else {  //读取成功返回0
//             bits[0];  //bits的第1个数
//             bits[1];  //bits的第2个数
//          }
//    //...剩下的两个读取同理


//    //4.写数据
//    uint8_t bitsw[128]={0};
//    bitsw[0]=1;bitsw[1]=1;
//    int wbits = modbus_write_bits(mb, 0, 2, bitsw); //写两个数进去
//    if(wbits  == -1){
//            //写入失败返回-1
//    }
//    else {  //写入成功返回0
//        //
//    }

//    uint16_t registers[128];
//    registers[0] = 200;registers[1] = 203;
//    int wreg = modbus_write_registers(mb,0,2,registers);
//    if(wreg  == -1){
//            //写入失败返回-1
//    }
//    else {  //写入成功返回0
//        //
//    }


//    //5.调用完成后释放资源
//    modbus_close(mb);
//    modbus_free(mb);


    return a.exec();
}
