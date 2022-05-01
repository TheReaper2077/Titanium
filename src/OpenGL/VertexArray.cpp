#pragma once

#include <iostream>
#include <OpenGL.h>

extern std::shared_ptr<OpenGLContext> gl_context;

VertexArray* VertexArray_Create(std::vector<VertexArrayAttribDescriptor> layouts) {
	GL_ASSERT(gl_context != nullptr);

	auto vertexarray = std::make_shared<VertexArray>();
	gl_context->vertex_array_store.push_back(vertexarray);

	glGenVertexArrays(1, &vertexarray->id);
	vertexarray->Bind();

	uint32_t relativeoffset = 0;
	uint32_t elem_relativeoffset = 0;

	for (auto& attrib: layouts) {
		GL_ASSERT(attrib.attr_type < VertexArrayAttrib_COUNT);

		if (attrib.attr_type == position) {
			vertexarray->position_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_position);
			vertexarray->has_position = true;
		}
		if (attrib.attr_type == normal) {
			vertexarray->normal_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_normal);
			vertexarray->has_normal = true;
		}
		if (attrib.attr_type == color) {
			vertexarray->color_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_color);
			vertexarray->has_color = true;
		}
		if (attrib.attr_type == uv0) {
			vertexarray->uv0_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_uv0);
			vertexarray->has_uv0 = true;
		}
		if (attrib.attr_type == uv1) {
			vertexarray->uv1_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_uv1);
			vertexarray->has_uv1 = true;
		}
		if (attrib.attr_type == uv2) {
			vertexarray->uv2_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_uv2);
			vertexarray->has_uv2 = true;
		}
		if (attrib.attr_type == uv3) {
			vertexarray->uv3_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_uv3);
			vertexarray->has_uv3 = true;
		}
		if (attrib.attr_type == uv4) {
			vertexarray->uv4_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_uv4);
			vertexarray->has_uv4 = true;
		}
		if (attrib.attr_type == uv5) {
			vertexarray->uv5_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_uv5);
			vertexarray->has_uv5 = true;
		}
		if (attrib.attr_type == uv6) {
			vertexarray->uv6_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_uv6);
			vertexarray->has_uv6 = true;
		}
		if (attrib.attr_type == uv7) {
			vertexarray->uv7_offset = elem_relativeoffset;
			GL_ASSERT(!vertexarray->has_uv7);
			vertexarray->has_uv7 = true;
		}

		glVertexArrayAttribFormat(vertexarray->id, attrib.idx, attrib.size, attrib.type, false, relativeoffset);
		glVertexArrayAttribBinding(vertexarray->id, attrib.idx, 0);
		glEnableVertexArrayAttrib(vertexarray->id, attrib.idx);

		std::cout << relativeoffset << '\n';

		switch(attrib.type) {
			case GL_FLOAT:
				relativeoffset += sizeof(GLfloat)*attrib.size;
				break;
			
			// case GL_FIXED:
			// 	relativeoffset += sizeof(GLfixed)*attrib.size;
			// 	break;
			
			// case GL_BYTE:
			// 	relativeoffset += sizeof(GLbyte)*attrib.size;
			// 	break;
			
			// case GL_UNSIGNED_BYTE:
			// 	relativeoffset += sizeof(GLubyte)*attrib.size;
			// 	break;
			
			default:
				GL_ASSERT(false);
		}
		
		elem_relativeoffset += attrib.size;
	}

	vertexarray->stride = relativeoffset;
	vertexarray->elem_stride = elem_relativeoffset;

	return vertexarray.get();
}

void VertexArray::Bind() {
	if (gl_context->binding_vertexarray == id) return;
	gl_context->binding_vertexarray = id;
	glBindVertexArray(id);
}

void VertexArray::UnBind() {
	gl_context->binding_vertexarray = 0;
	glBindVertexArray(0);
}

void VertexArray::BindVertexBuffer(VertexBuffer *vertexbuffer, std::size_t stride, std::size_t offset) {
	glVertexArrayVertexBuffer(id, 0, vertexbuffer->id, offset, (stride == 0) ? stride : stride);
}

void VertexArray::BindIndexBuffer(IndexBuffer *indexbuffer) {
	glVertexArrayElementBuffer(id, indexbuffer->id);
}