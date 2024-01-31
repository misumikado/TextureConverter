#pragma once
#include <string>
#include <DirectXTex.h>

class TextureConverter
{
private:
	// �摜�̏��
	DirectX::TexMetadata metadata;
	// �摜�C���[�W�̃R���e�i
	DirectX::ScratchImage scratchImage;
	// �f�B���N�g���p�X
	std::wstring directoryPath;
	// �t�@�C����
	std::wstring fileName;
	// �t�@�C���g���q
	std::wstring fileExt;

	/// <summary>
	/// �e�N�X�`���t�@�C���ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="mString">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	/// </summary>
	/// <param name="filePath_">�t�@�C���p�X</param>
	void SeparateFilePath(const std::wstring& filePath_);

	// DDS�e�N�X�`���Ƃ��ăt�@�C�������o��
	void SaveDDSTextureToFile();

public:
	/// <summary>
	/// �e�N�X�`����WIC����DDS�ɕϊ�����
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void ConvertTextureWICToDDS(const std::string& filePath);
};