#include "OA_FrameBuffer.h"
#include "OA_Material.h"
#include "externals/gl/glad/glad.h"

namespace onart {

	std::map<const char*, FrameBuffer*> FrameBuffer::fbs;

	FrameBuffer::FrameBuffer(unsigned width, unsigned height, ATTACHMENT color, ATTACHMENT depth, ATTACHMENT stencil) :width(width), height(height), CDS{ color,depth,stencil } {

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
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			break;
		case ATTACHMENT::RW:
			glGenTextures(1, &(this->depth));
			glBindTexture(GL_TEXTURE_2D, this->depth);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color, 0);
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
		glDeleteFramebuffers(1, &fbo);
	}

	void FrameBuffer::make(const char* name, unsigned width, unsigned height, ATTACHMENT colorBuffer, ATTACHMENT depthBuffer, ATTACHMENT stencilBuffer, unsigned func, unsigned ref, unsigned sfail, unsigned dpfail, unsigned dppass) {
		{ auto er = fbs.find(name); if (!name || er != fbs.end()) return; }
		if (stencilBuffer == ATTACHMENT::NONE) {
			fbs[name] = new FrameBuffer(width, height, colorBuffer, depthBuffer);
		}
		else {

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
	}
}