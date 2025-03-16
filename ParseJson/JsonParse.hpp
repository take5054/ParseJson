#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class JsonParse
{
public:
	using JsonMap = std::unordered_map<std::string, std::string>;

	JsonParse() noexcept;
	~JsonParse();

	/// <summary>
	/// JSONファイルを解析して、キーと値を連想配列に格納する
	/// </summary>
	/// <param name="In_strFilePath">JSONファイルのパス</param>
	void ParseFile(const std::string& In_strFilePath);

	inline const JsonMap& GetJsonMap() const noexcept { return m_JsonMap; }
	inline const std::vector<JsonMap>& GetJsonArray() const noexcept { return m_JsonArray; }

private:
	JsonMap m_JsonMap;	// JSONのキーと値を格納する連想配列
	std::vector<JsonMap> m_JsonArray;	// JSONの配列を格納する配列

	// ParseFileの依存関数
	std::string RemoveWhitespace(const std::string& In_string);
	// ParseFileの依存関数
	JsonMap ParseObject(const std::string& In_strObject);
};
