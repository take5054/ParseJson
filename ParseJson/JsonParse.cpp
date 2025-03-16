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
    // UTF-8ロケールを設定
    try {
        std::locale::global(std::locale("ja_JP.UTF-8"));
    }
    catch (const std::runtime_error&) {
        std::cerr << "UTF-8ロケールの設定に失敗しました。" << std::endl;
    }

    std::ifstream ifsInputFile(filePath, std::ios::binary);
    if (!ifsInputFile.is_open())
    {
        std::cerr << "ファイルを開けませんでした: " << filePath << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << ifsInputFile.rdbuf();
    std::string JsonContent = RemoveWhitespace(buffer.str());

    if (JsonContent.empty())
    {
        std::cerr << "空のファイルです。" << std::endl;
        return;
    }

    if (JsonContent.front() == '{' && JsonContent.back() == '}')
    {
        // 単一のオブジェクトとしてパースする
        m_JsonMap = ParseObject(JsonContent);
    }
    else if (JsonContent.front() == '[' && JsonContent.back() == ']')
    {
        // 配列形式の場合：配列内のそれぞれのオブジェクトをパース
        size_t ReadPosition = 1; // '['の次
        while (ReadPosition < JsonContent.size() - 1)
        {
            // オブジェクトの開始位置を探す
            size_t objStartPosition = JsonContent.find('{', ReadPosition);
			// オブジェクトが見つからない場合は終了
            if (objStartPosition == std::string::npos) break;

            // オブジェクトの終了位置を探す
            size_t objEndPosition = JsonContent.find('}', objStartPosition);
			// オブジェクトが見つからない場合は終了
            if (objEndPosition == std::string::npos) break;

			// オブジェクトを取り出してパース
            std::string objectStr = JsonContent.substr(objStartPosition, objEndPosition - objStartPosition + 1);
            m_JsonArray.push_back(ParseObject(objectStr));

			// 次のオブジェクトの開始位置を設定
            ReadPosition = ++objEndPosition;
        }
    }
    else
    {
        std::cerr << "無効なJSON形式です。" << std::endl;
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
    size_t ReadPosition = 1; // '{' の次から開始
	size_t objSize = In_strObject.size() - 1;

    while (ReadPosition < objSize)
    {
		// キーの開始位置を探す
        if (In_strObject[ReadPosition] != '"')
        {
            std::cerr << "キーの開始位置に\"がありません。" << std::endl;
            break;
        }
		// キーの終了位置を探す
        size_t keyEnd = In_strObject.find('"', ReadPosition + 1);
        if (keyEnd == std::string::npos)
        {
            std::cerr << "キーの終了\"が見つかりませんでした。" << std::endl;
            break;
        }

        std::string key = In_strObject.substr(ReadPosition + 1, keyEnd - ReadPosition - 1);
        ReadPosition = ++keyEnd;

        if (ReadPosition >= objSize || In_strObject[ReadPosition] != ':')
        {
            std::cerr << "キーと値の区切りに':'がありません。" << std::endl;
            break;
        }
        ++ReadPosition; // ':' をスキップ

        if (ReadPosition >= objSize || In_strObject[ReadPosition] != '"')
        {
            std::cerr << "値の開始位置に\"がありません。" << std::endl;
            break;
        }
        size_t valueEnd = In_strObject.find('"', ReadPosition + 1);
        if (valueEnd == std::string::npos)
        {
            std::cerr << "値の終了\"が見つかりませんでした。" << std::endl;
            break;
        }
        std::string value = In_strObject.substr(ReadPosition + 1, valueEnd - ReadPosition - 1);
        ReadPosition = ++valueEnd;

        mapParsedResult[key] = value;

        // 次は,またはオブジェクトの終了 '}'
        if (ReadPosition < objSize && In_strObject[ReadPosition] == ',')
            ReadPosition++;
        else if (ReadPosition < objSize && In_strObject[ReadPosition] == '}')
            break;
    }

    return mapParsedResult;
}
