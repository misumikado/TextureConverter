#include "TextureConverter.h"
#include <Windows.h>
#include <vector>

using namespace DirectX;

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	// ファイルパスをワイド文字列に変換する
	std::wstring wfilePath = ConvertMultiByteStringToWideString(filePath);

	// WICテクスチャのロード
	HRESULT result = LoadFromWICFile(wfilePath.c_str(), WIC_FLAGS_NONE, &metadata, scratchImage);
	assert(SUCCEEDED(result));

	// フォルダパスとファイル名を分離する
	SeparateFilePath(wfilePath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ワイド文字列に変換した際の文字列バッファサイズを計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	// ワイド文字列
	std::wstring wString;
	wString.resize(filePathBufferSize);

	// ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath_)
{
	size_t pos1;
	std::wstring exceptExt;

	// 区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath_.rfind('.');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos){
		// 区切り文字の後ろをファイル拡張子として保存 
		fileExt = filePath_.substr(pos1 + 1, filePath_.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		exceptExt = filePath_.substr(0, pos1);
	}
	else{
		fileExt = L"";
		exceptExt = filePath_;
	}

	// 区切り文字'\\'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos){
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName = exceptExt.substr(pos1 + 1, filePath_.size() - pos1 - 1);
		return;
	}
	// 区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos){
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName = exceptExt.substr(pos1 + 1, filePath_.size() - pos1 - 1);
		return;
	}
	// 区切り文字がないのでファイル名のみとして扱う
	directoryPath = L"";
	fileName = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile()
{
	ScratchImage mipChain;
	// ミップマップ生成
	HRESULT result = GenerateMipMaps(scratchImage.GetImages(),
		scratchImage.GetImageCount(), scratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)){
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage = std::move(mipChain);
		metadata = scratchImage.GetMetadata();
	}

	// 圧縮形式に変換
	ScratchImage converted;
	result = Compress(scratchImage.GetImages(), scratchImage.GetImageCount(), metadata,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT |
		TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(result)){
		scratchImage = std::move(converted);
		metadata = scratchImage.GetMetadata();
	}

	// 読み込んだテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// 出力ファイル名を設定する
	std::wstring filePath = directoryPath + fileName + L".dds";

	// DDSファイル書き出し
	result = SaveToDDSFile(scratchImage.GetImages(),
		scratchImage.GetImageCount(), metadata, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath)
{
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);

	// DDS形式に変換して書き出す
	SaveDDSTextureToFile();
}
