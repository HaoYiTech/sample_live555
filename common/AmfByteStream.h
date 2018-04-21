/******************************************************
 * 浩一科技，提供云录播、云监控的全平台无插件解决方案。
 * 支持按需直播，多点布控，分布式海量存储，动态扩容；
 * 支持微信扫码登录，全平台帐号统一，关联微信小程序；
 * 支持多种数据输入：摄像头IPC、rtmp、rtsp、MP4文件；
 * 支持全实时、全动态、全网页管理，网页前后台兼容IE8；
 * 支持多终端无插件自适应播放，flvjs/hls/rtmp自动适配；
 ******************************************************
 * 官方网站 => https://myhaoyi.com
 * 技术博客 => http://blog.csdn.net/haoyitech
 * 开源代码 => https://github.com/HaoYiTech/
 * 云监控群 => 483663026（QQ群）
 * 云录播群 => 630379661（QQ群）
 ******************************************************
 * 这个头文件是跟librtmp相关的内存转换函数列表
 ******************************************************/

#ifndef _AMF_BYTE_STREAM_H_
#define _AMF_BYTE_STREAM_H_

#include <cstring>

inline char* UI08ToBytes(char* buf, unsigned char val)
{
    buf[0] = (char)(val) & 0xff;
    return buf + 1;
}

inline char* UI16ToBytes(char* buf, unsigned short val)
{
    buf[0] = (char)(val >> 8) & 0xff;
    buf[1] = (char)(val) & 0xff;
    return buf + 2;
}

inline char* UI24ToBytes(char* buf, unsigned int val)
{
    buf[0] = (char)(val >> 16) & 0xff;
    buf[1] = (char)(val >> 8) & 0xff;
    buf[2] = (char)(val) & 0xff;
    return buf + 3;
}

inline char* UI32ToBytes(char* buf, unsigned int val)
{
    buf[0] = (char)(val >> 24) & 0xff;
    buf[1] = (char)(val >> 16) & 0xff;
    buf[2] = (char)(val >> 8) & 0xff;
    buf[3] = (char)(val) & 0xff;
    return buf + 4;
}

inline char* UI64ToBytes(char* buf, unsigned __int64 val)
{
    buf[0] = (char)(val >> 56) & 0xff;
    buf[1] = (char)(val >> 48) & 0xff;
    buf[2] = (char)(val >> 40) & 0xff;
    buf[3] = (char)(val >> 32) & 0xff;
    buf[4] = (char)(val >> 24) & 0xff;
    buf[5] = (char)(val >> 16) & 0xff;
    buf[6] = (char)(val >> 8) & 0xff;
    buf[7] = (char)(val) & 0xff;
    return buf + 8;
}

inline char* DoubleToBytes(char* buf, double val) 
{
    union {
        unsigned char dc[8];
        double dd;
    } d;
    unsigned char b[8];

    d.dd = val;

    b[0] = d.dc[7];
    b[1] = d.dc[6];
    b[2] = d.dc[5];
    b[3] = d.dc[4];
    b[4] = d.dc[3];
    b[5] = d.dc[2];
    b[6] = d.dc[1];
    b[7] = d.dc[0];
    memcpy(buf, b, 8);
    return buf + 8;
}

inline unsigned char BytesToUI08(const char* buf)
{
    return ( (((unsigned char)buf[0]))	& 0xff );
}

inline unsigned short BytesToUI16(const char* buf)
{
    return ( (((unsigned short)buf[0]) << 8) & 0xff00 )
        | ( (((unsigned short)buf[1]))		 & 0xff );
}

inline unsigned int BytesToUI24(const char* buf)
{
    return ( (((unsigned int)buf[0]) << 16)	& 0xff0000 )
        | ( (((unsigned int)buf[1]) << 8)	& 0xff00 )
        | ( (((unsigned int)buf[2]))		& 0xff );
}

inline unsigned int BytesToUI32(const char* buf)
{
    return ( (((unsigned int)buf[0]) << 24)	& 0xff000000 )
        | ( (((unsigned int)buf[1]) << 16)	& 0xff0000 )
        | ( (((unsigned int)buf[2]) << 8)	& 0xff00 )
        | ( (((unsigned int)buf[3]))		& 0xff );
}

enum
{
    AMF_DATA_TYPE_NUMBER      = 0x00,
    AMF_DATA_TYPE_BOOL        = 0x01,
    AMF_DATA_TYPE_STRING      = 0x02,
    AMF_DATA_TYPE_OBJECT      = 0x03,
    AMF_DATA_TYPE_NULL        = 0x05,
    AMF_DATA_TYPE_UNDEFINED   = 0x06,
    AMF_DATA_TYPE_REFERENCE   = 0x07,
    AMF_DATA_TYPE_MIXEDARRAY  = 0x08,
    AMF_DATA_TYPE_OBJECT_END  = 0x09,
    AMF_DATA_TYPE_ARRAY       = 0x0a,
    AMF_DATA_TYPE_DATE        = 0x0b,
    AMF_DATA_TYPE_LONG_STRING = 0x0c,
    AMF_DATA_TYPE_UNSUPPORTED = 0x0d,
};

enum
{
    FLV_TAG_TYPE_AUDIO = 0x08,
    FLV_TAG_TYPE_VIDEO = 0x09,
    FLV_TAG_TYPE_META  = 0x12,
    TAG_TYPE_SHARED_OBJECT  = 0x13,
};

/* shared object datatypes */
enum
{
    RTMP_SHARED_OBJECT_DATATYPE_CONNECT = 0x01,
    RTMP_SHARED_OBJECT_DATATYPE_DISCONNECT = 0x02,
    RTMP_SHARED_OBJECT_DATATYPE_SET_ATTRIBUTE = 0x03,
    RTMP_SHARED_OBJECT_DATATYPE_UPDATE_DATA = 0x04,
    RTMP_SHARED_OBJECT_DATATYPE_UPDATE_ATTRIBUTE = 0x05,
    RTMP_SHARED_OBJECT_DATATYPE_SEND_MESSAGE = 0x06,
    RTMP_SHARED_OBJECT_DATATYPE_STATUS = 0x07,
    RTMP_SHARED_OBJECT_DATATYPE_CLEAR_DATA = 0x08,
    RTMP_SHARED_OBJECT_DATATYPE_DELETE_DATA = 0x09,
    RTMP_SHARED_OBJECT_DATATYPE_DELETE_ATTRIBUTE = 0x0A,
    RTMP_SHARED_OBJECT_DATATYPE_INITIAL_DATA = 0x0B,
};

inline char* AmfStringToBytes(char* buf, const char *str)
{
    char* pbuf = buf;
    size_t len = strlen(str);
    pbuf = UI16ToBytes(pbuf, len);
    memcpy(pbuf, str, len);
    pbuf += len;
    return pbuf;
}

inline char* AmfDoubleToBytes(char* buf, double d)
{
    char* pbuf = buf;
    pbuf = UI08ToBytes(pbuf, AMF_DATA_TYPE_NUMBER);
    pbuf = DoubleToBytes(pbuf, d);
    return pbuf;
}

inline char* AmfBoolToBytes(char* buf, int b)
{
    char* pbuf = buf;
    pbuf = UI08ToBytes(pbuf, AMF_DATA_TYPE_BOOL);
    pbuf = UI08ToBytes(pbuf, !!b);
    return pbuf;
}

#endif // _AMF_BYTE_STREAM_H_
