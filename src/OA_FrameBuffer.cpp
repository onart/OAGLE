/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#include "OA_FrameBuffer.h"
#include "OA_Material.h"
#include "externals/gl/glad/glad.h"

#include <type_traits>

namespace onart {

	std::map<const char*, FrameBuffer*> FrameBuffer::fbs;

	FrameBuffer::FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth, ATTACHMENT stencil, unsigned func, int ref, unsigned mask, unsigned sfail, unsigned dpfail, unsigned dppass)
		:width(width), height(height), CDS{ color,depth,stencil }, stFunc(func), stRef(ref), stMask(mask), sfail(sfail), dpfail(dpfail), dppass(dppass) {
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		switch (color)
		{
		case ATTACHMENT::NONE:
			this->color = 0;
			glDrawBuffer(GL_NONE);
			break;
		case ATTACHMENT::W:
			glGenRenderbuffers(1, &(this->color));
			glBindRenderbuffer(GL_RENDERBUFFER, this->color);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->color);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			break;
		case ATTACHMENT::RW:
			glGenTextures(1, &(this->color));
			glBindTexture(GL_TEXTURE_2D, this->color);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;
		default:
			exit(2);
			break;
		}

		switch (depth)
		{
		case ATTACHMENT::W:
			glGenRenderbuffers(1, &(this->depth));
			glBindRenderbuffer(GL_RENDERBUFFER, this->depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			break;
		case ATTACHMENT::RW:
			glGenTextures(1, &(this->depth));
			glBindTexture(GL_TEXTURE_2D, this->depth);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->color, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;
		case ATTACHMENT::NONE:
		{
			switch (stencil)
			{
			case onart::FrameBuffer::ATTACHMENT::RW:
				glGenTextures(1, &this->depth);
				glBindTexture(GL_TEXTURE_2D, this->depth);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->depth, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
				break;
			case onart::FrameBuffer::ATTACHMENT::W:
				glGenRenderbuffers(1, &(this->depth));
				glBindRenderbuffer(GL_RENDERBUFFER, this->depth);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->depth);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				break;
			default:
				break;
			}
		}
			break;
		default:
			break;
		}
	}

	FrameBuffer::FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth) : width(width), height(height), CDS{ color,depth,ATTACHMENT::NONE } {
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		switch (color)
		{
		case ATTACHMENT::NONE:
			this->color = 0;
			glDrawBuffer(GL_NONE);
			break;
		case ATTACHMENT::W:
			glGenRenderbuffers(1, &(this->color));
			glBindRenderbuffer(GL_RENDERBUFFER, this->color);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->color);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			break;
		case ATTACHMENT::RW:
			glGenTextures(1, &(this->color));
			glBindTexture(GL_TEXTURE_2D, this->color);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;
		default:
			exit(2);
			break;
		}

		switch (depth)
		{
		case ATTACHMENT::W:
			glGenRenderbuffers(1, &(this->depth));
			glBindRenderbuffer(GL_RENDERBUFFER, this->depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->depth);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			break;
		case ATTACHMENT::RW:
			glGenTextures(1, &(this->depth));
			glBindTexture(GL_TEXTURE_2D, this->depth);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depth, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			break;
		default:
			break;
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			exit(2);
		}
	}

	FrameBuffer::~FrameBuffer() {
		if (CDS[0] == ATTACHMENT::RW) { glDeleteTextures(1, &color); }
		else if (CDS[0] == ATTACHMENT::W) { glDeleteRenderbuffers(1, &color); }
		if (CDS[1] == ATTACHMENT::RW) { glDeleteTextures(1, &depth); }
		else if (CDS[1] == ATTACHMENT::W) { glDeleteRenderbuffers(1, &depth); }
		else {
			if (CDS[2] == ATTACHMENT::RW) { glDeleteTextures(1, &depth); }
			else if(CDS[2]==ATTACHMENT::W) { glDeleteRenderbuffers(1, &depth); }
		}
		glDeleteFramebuffers(1, &fbo);
	}

	void FrameBuffer::make(const char* name, unsigned width, unsigned height, ATTACHMENT colorBuffer, ATTACHMENT depthBuffer, ATTACHMENT stencilBuffer, unsigned func, int ref, unsigned mask, unsigned sfail, unsigned dpfail, unsigned dppass) {
		{ auto er = fbs.find(name); if (!name || er != fbs.end()) return; }
		if (stencilBuffer == ATTACHMENT::NONE) {
			fbs[name] = new FrameBuffer(width, height, colorBuffer, depthBuffer);
		}
		else {
			fbs[name] = new FrameBuffer(width, height, colorBuffer, depthBuffer, depthBuffer == ATTACHMENT::NONE ? stencilBuffer : depthBuffer, func, ref, mask, sfail, dpfail, dppass);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::drop(const char* name) {
		auto er = fbs.find(name);
		if (er != fbs.end()) {
			delete er->second;
			fbs.erase(er);
		}
	}

	void FrameBuffer::use(const char* name) {
		if (!name) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			void defaultFramebufferViewport();
			defaultFramebufferViewport();
		}
		else {
			auto fb = fbs.find(name);
			if (fb != fbs.end()) {
				fb->second->setViewport();
			}
		}
	}

	void FrameBuffer::setViewport() {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, width, height);
		if (CDS[1] != ATTACHMENT::NONE) { glEnable(GL_DEPTH_TEST); }
		else { glDisable(GL_DEPTH_TEST); }
		if (CDS[2] != ATTACHMENT::NONE) { 
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(stFunc, stRef, stMask);
			glStencilOp(sfail, dpfail, dppass);
		}
		else {
			glDisable(GL_STENCIL_TEST);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	template <class T>
	T* FrameBuffer::readColorValue(unsigned left, unsigned down, unsigned width, unsigned height) {
		// Âü°í: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
		GLenum type;
		if constexpr (std::is_same_v<T, float>) { type = GL_FLOAT; }
		else if constexpr (std::is_same_v<T, unsigned char>) { type = GL_UNSIGNED_BYTE; }
		else if constexpr (std::is_same_v<T, char>) { type = GL_BYTE; }
		else if constexpr (std::is_same_v<T, unsigned short>) { type = GL_UNSIGNED_SHORT; }
		else if constexpr (std::is_same_v<T, short>) { type = GL_SHORT; }
		else if constexpr (std::is_same_v<T, unsigned>) { type = GL_UNSIGNED_INT; }
		else if constexpr (std::is_same_v<T, int>) { type = GL_INT; }
		T* ret = new T[width * height * 4];
		glReadPixels(left, down, width, height, GL_RGBA, type, ret);
		return ret;
	}

	float* FrameBuffer::readDepthValue(unsigned left, unsigned down, unsigned width, unsigned height) {
		float* ret = new float[width * height];
		glReadPixels(left, down, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, ret);
		return ret;
	}

	unsigned char* FrameBuffer::readStencilValue(unsigned left, unsigned down, unsigned width, unsigned height) {
		unsigned char* ret = new unsigned char[width * height];
		glReadPixels(left, down, width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, ret);
		return ret;
	}
}