#pragma once

#include <map>
#include <string>

namespace fox {
    enum INI_RES
    {
        INI_SUCCESS,            //�ɹ�
        INI_ERROR,              //��ͨ����
        INI_OPENFILE_ERROR,     //���ļ�ʧ��
        INI_NO_ATTR            //�޶�Ӧ�ļ�ֵ
    };


    /*
    * �Ӽ�����:�Ӽ�ֵ
    */
    using KEYMAP = map<std::string, std::string>;


    /*
    * ��������:����ֵ
    */
    using MAINKEYMAP = map<std::string, KEYMAP>;

    /*
    * һ��ֵ����ı�����
    */
    constexpr auto CONFIGLEN = 16 * 1024;
};

using namespace std;
using namespace fox;

/*
* ini �ļ��Ļ��������ࣺ
* ˵�����ڴ��ļ�ʱʱ�򣬽�ini�ļ�����ʽ��������DOM�����浽��ϣ������ȡֵ��ʱ�򣬵���ϣ����ȥȡ����
*/
class FoxIniFile
{
public:
    // ���캯��
    FoxIniFile();
    // ��������
    virtual ~FoxIniFile();

public:
    // ��config �ļ�
    INI_RES openFile(const char* pathName, const char* type);
    // �ر�config �ļ�
    INI_RES closeFile();

public:
    //��ȡ���εļ�ֵ
    int  getInt(const char* mAttr, const char* cAttr);
    //��ȡ��ֵ���ַ���
    const char* getStr(const char* mAttr, const char* cAttr);

protected:
    // ��ȡconfig�ļ�
    INI_RES getKey(const char* mAttr, const char* cAttr, char* value);

protected:
    // ���򿪵��ļ��ֱ�
    FILE*           m_fp;
    char            m_szKey[CONFIGLEN];
    MAINKEYMAP      m_Map;
};


