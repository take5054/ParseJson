#include "JsonParse.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

JsonParse::JsonParse() noexcept
{
	m_JsonMap.clear();
}

JsonParse::~JsonParse()
{
	m_JsonMap.clear();
}

void JsonParse::ParseFile(const std::string& filePath)
{
    // UTF-8���P�[����ݒ�
    try {
        std::locale::global(std::locale("ja_JP.UTF-8"));
    }
    catch (const std::runtime_error&) {
        std::cerr << "UTF-8���P�[���̐ݒ�Ɏ��s���܂����B" << std::endl;
    }

    std::ifstream ifsInputFile(filePath, std::ios::binary);
    if (!ifsInputFile.is_open())
    {
        std::cerr << "�t�@�C�����J���܂���ł���: " << filePath << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << ifsInputFile.rdbuf();
    std::string JsonContent = RemoveWhitespace(buffer.str());

    if (JsonContent.empty())
    {
        std::cerr << "��̃t�@�C���ł��B" << std::endl;
        return;
    }

    if (JsonContent.front() == '{' && JsonContent.back() == '}')
    {
        // �P��̃I�u�W�F�N�g�Ƃ��ăp�[�X����
        m_JsonMap = ParseObject(JsonContent);
    }
    else if (JsonContent.front() == '[' && JsonContent.back() == ']')
    {
        // �z��`���̏ꍇ�F�z����̂��ꂼ��̃I�u�W�F�N�g���p�[�X
        size_t ReadPosition = 1; // '['�̎�
        while (ReadPosition < JsonContent.size() - 1)
        {
            // �I�u�W�F�N�g�̊J�n�ʒu��T��
            size_t objStartPosition = JsonContent.find('{', ReadPosition);
			// �I�u�W�F�N�g��������Ȃ��ꍇ�͏I��
            if (objStartPosition == std::string::npos) break;

            // �I�u�W�F�N�g�̏I���ʒu��T��
            size_t objEndPosition = JsonContent.find('}', objStartPosition);
			// �I�u�W�F�N�g��������Ȃ��ꍇ�͏I��
            if (objEndPosition == std::string::npos) break;

			// �I�u�W�F�N�g�����o���ăp�[�X
            std::string objectStr = JsonContent.substr(objStartPosition, objEndPosition - objStartPosition + 1);
            m_JsonArray.push_back(ParseObject(objectStr));

			// ���̃I�u�W�F�N�g�̊J�n�ʒu��ݒ�
            ReadPosition = ++objEndPosition;
        }
    }
    else
    {
        std::cerr << "������JSON�`���ł��B" << std::endl;
    }
}

std::string JsonParse::RemoveWhitespace(const std::string& In_string)
{
    std::string strOutput;
    for (char ch : In_string)
    {
        if (!std::isspace(static_cast<unsigned char>(ch)))
            strOutput.push_back(ch);
    }
    return strOutput;
}

JsonParse::JsonMap JsonParse::ParseObject(const std::string& In_strObject)
{
    JsonMap mapParsedResult;
    size_t ReadPosition = 1; // '{' �̎�����J�n
	size_t objSize = In_strObject.size() - 1;

    while (ReadPosition < objSize)
    {
		// �L�[�̊J�n�ʒu��T��
        if (In_strObject[ReadPosition] != '"')
        {
            std::cerr << "�L�[�̊J�n�ʒu��\"������܂���B" << std::endl;
            break;
        }
		// �L�[�̏I���ʒu��T��
        size_t keyEnd = In_strObject.find('"', ReadPosition + 1);
        if (keyEnd == std::string::npos)
        {
            std::cerr << "�L�[�̏I��\"��������܂���ł����B" << std::endl;
            break;
        }

        std::string key = In_strObject.substr(ReadPosition + 1, keyEnd - ReadPosition - 1);
        ReadPosition = ++keyEnd;

        if (ReadPosition >= objSize || In_strObject[ReadPosition] != ':')
        {
            std::cerr << "�L�[�ƒl�̋�؂��':'������܂���B" << std::endl;
            break;
        }
        ++ReadPosition; // ':' ���X�L�b�v

        if (ReadPosition >= objSize || In_strObject[ReadPosition] != '"')
        {
            std::cerr << "�l�̊J�n�ʒu��\"������܂���B" << std::endl;
            break;
        }
        size_t valueEnd = In_strObject.find('"', ReadPosition + 1);
        if (valueEnd == std::string::npos)
        {
            std::cerr << "�l�̏I��\"��������܂���ł����B" << std::endl;
            break;
        }
        std::string value = In_strObject.substr(ReadPosition + 1, valueEnd - ReadPosition - 1);
        ReadPosition = ++valueEnd;

        mapParsedResult[key] = value;

        // ����,�܂��̓I�u�W�F�N�g�̏I�� '}'
        if (ReadPosition < objSize && In_strObject[ReadPosition] == ',')
            ReadPosition++;
        else if (ReadPosition < objSize && In_strObject[ReadPosition] == '}')
            break;
    }

    return mapParsedResult;
}
