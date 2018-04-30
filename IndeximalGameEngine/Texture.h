// Created by Indeximal 4/18/2018 3:27:02 PM
#pragma once

#include "IndeximalGameEngine.h"
#include "stb_image.h"

namespace ige {
	enum FormatType {
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RGB16F = GL_RGB16F,
		DEPTH = GL_DEPTH_COMPONENT
	};

	int getTextureTypeChannels(FormatType type) {
		if (type == RGB) return 3;
		if (type == RGBA) return 4;
		if (type == RGB16F) return 6;
		if (type == DEPTH) return 1;
		throw std::invalid_argument("Type doesn't exist!");
	}

	enum DataType {
		UBYTE = GL_UNSIGNED_BYTE,
		FLOAT = GL_FLOAT
	};

	int getDataTypeSize(DataType type) {
		if (type == UBYTE) return 1;
		if (type == FLOAT) return 4;
		throw std::invalid_argument("Type doesn't exist!");
	}

	enum FilterMode {
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	enum WrapMode {
		CLAMP_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_BORDER = GL_CLAMP_TO_BORDER,
		MIRROR = GL_MIRRORED_REPEAT,
		REPEAT = GL_REPEAT
	};

	class Texture {
	private:
		GLuint id;

		int texureWidth;
		int textureHeight;
		FormatType formatType;
		DataType internalDataType;
		bool created = false;
	public:
		Texture() { 
		}

		Texture(std::string path) {
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data = stbi_load(path.c_str(), &texureWidth, &textureHeight, nullptr, 4);

			if (!data) {
				logError(path + ": failed to load texture");
				return;
			}
			load(texureWidth, textureHeight, data);
			logInfo(path + " successfully loaded (" + std::to_string(getSizeInMemory() / 1000) + "KB)");

			stbi_image_free(data);
		}

		Texture(int width, int height, void* data) {
			load(width, height, data);
		}

		void create() {
			glGenTextures(1, &id);
			setFilterMode(LINEAR, LINEAR);
			setWrapMode(REPEAT);
			created = true;
		}

		void initEmpty(int width, int height, FormatType internalFormat, FormatType format, DataType dataType) {
			texureWidth = width;
			textureHeight = height;
			formatType = internalFormat;
			internalDataType = dataType;

			if (!created) create();
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void load(int width, int height, void* data) {
			if (created) {
				logError("Texture has already been created");
				return;
			}
			if (data == nullptr) 
				return;
			
			texureWidth = width;
			textureHeight = height;
			formatType = RGBA;
			internalDataType = UBYTE;
			create();
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void setFilterMode(FilterMode min, FilterMode max) {
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void setWrapMode(WrapMode mode) {
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		GLuint getNativeID() const {
			if (!created) {
				logError("Texture hasn't been created yet");
				return 0;
			}
			return id;
		}

		bool isCreated() {
			return created;
		}

		int getWidth() {
			return texureWidth;
		}

		int getHeight() {
			return textureHeight;
		}

		int getChannels() {
			return getTextureTypeChannels(formatType);
		}

		int getBytesPerChannel() {
			return getDataTypeSize(internalDataType);
		}

		int getSizeInMemory() {
			return getBytesPerChannel() * getChannels() * getHeight() * getWidth();
		}

		void destory() {
			if (created) {
				glDeleteTextures(1, &id);
			}
		}

		~Texture() {
			destory();
		}

		Texture(const Texture& other) = delete; // copy constructor
		Texture(Texture&& other) = default; // move constructor
		Texture& operator=(const Texture& other) = delete; // copy assignment
		Texture& operator=(Texture&& other) = delete; // move assignment
	};
}