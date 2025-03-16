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
	/// JSON�t�@�C������͂��āA�L�[�ƒl��A�z�z��Ɋi�[����
	/// </summary>
	/// <param name="In_strFilePath">JSON�t�@�C���̃p�X</param>
	void ParseFile(const std::string& In_strFilePath);

	inline const JsonMap& GetJsonMap() const noexcept { return m_JsonMap; }
	inline const std::vector<JsonMap>& GetJsonArray() const noexcept { return m_JsonArray; }

private:
	JsonMap m_JsonMap;	// JSON�̃L�[�ƒl���i�[����A�z�z��
	std::vector<JsonMap> m_JsonArray;	// JSON�̔z����i�[����z��

	// ParseFile�̈ˑ��֐�
	std::string RemoveWhitespace(const std::string& In_string);
	// ParseFile�̈ˑ��֐�
	JsonMap ParseObject(const std::string& In_strObject);
};
