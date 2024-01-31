#pragma once
#include <string>
#include <DirectXTex.h>

class TextureConverter
{
private:
	// 画像の情報
	DirectX::TexMetadata metadata;
	// 画像イメージのコンテナ
	DirectX::ScratchImage scratchImage;
	// ディレクトリパス
	std::wstring directoryPath;
	// ファイル名
	std::wstring fileName;
	// ファイル拡張子
	std::wstring fileExt;

	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// フォルダパスとファイル名を分離する
	/// </summary>
	/// <param name="filePath_">ファイルパス</param>
	void SeparateFilePath(const std::wstring& filePath_);

	// DDSテクスチャとしてファイル書き出し
	void SaveDDSTextureToFile();

public:
	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void ConvertTextureWICToDDS(const std::string& filePath);
};